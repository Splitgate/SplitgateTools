#pragma once

#include "FMemory.h"
#include "TInlineAllocater.h"

#include <intrin.h>

#define NumBitsPerDWORD ((int32_t)32)
#define NumBitsPerDWORDLogTwo ((int32_t)5)
static FORCEINLINE uint32_t CountLeadingZeros(uint32_t Value)
{
	unsigned long Log2;
	if (_BitScanReverse(&Log2, Value) != 0)
	{
		return 31 - Log2;
	}

	return 32;
}

class TBitArray
{
private:
	template <typename ArrayType>
	friend class TSparseArray;
	template <typename SetType>
	friend class TSet;

public:
	TInlineAllocator<4>::ForElementType<uint32_t> Data;
	int NumBits;
	int MaxBits;

	struct FRelativeBitReference
	{
	public:
		FORCEINLINE explicit FRelativeBitReference(int BitIndex)
			: DWORDIndex(BitIndex >> NumBitsPerDWORDLogTwo)
			, Mask(1 << (BitIndex & ((NumBitsPerDWORD)-1)))
		{
		}

		int DWORDIndex;
		uint32_t Mask;
	};

	struct FBitReference
	{
		FORCEINLINE FBitReference(uint32_t& InData, uint32_t InMask)
			: Data(InData)
			, Mask(InMask)
		{
		}
		FORCEINLINE FBitReference(const uint32_t& InData, const uint32_t InMask)
			: Data(const_cast<uint32_t&>(InData))
			, Mask(InMask)
		{
		}

		FORCEINLINE void SetBit(const bool Value)
		{
			Value ? Data |= Mask : Data &= ~Mask;

			// 10011101 - Data			 // 10011101 - Data
			// 00000010 - Mask - true |	 // 00000010 - Mask - false
			// 10011111	-  |=			 // 11111101 -  ~
			//							 // 10011111 -  &=
		}

		FORCEINLINE operator bool() const
		{
			return (Data & Mask) != 0;
		}
		FORCEINLINE void operator=(const bool Value)
		{
			this->SetBit(Value);
		}

	private:
		uint32_t& Data;
		uint32_t Mask;
	};

public:
	class FBitIterator : public FRelativeBitReference
	{
	private:
		int Index;
		const TBitArray& IteratedArray;

	public:
		FORCEINLINE FBitIterator(const TBitArray& ToIterate, const int StartIndex) // Begin
			: IteratedArray(ToIterate)
			, Index(StartIndex)
			, FRelativeBitReference(StartIndex)
		{
		}
		FORCEINLINE FBitIterator(const TBitArray& ToIterate) // End
			: IteratedArray(ToIterate)
			, Index(ToIterate.NumBits)
			, FRelativeBitReference(ToIterate.NumBits)
		{
		}

		FORCEINLINE explicit operator bool() const
		{
			return Index < IteratedArray.Num();
		}
		FORCEINLINE FBitIterator& operator++()
		{
			++Index;
			this->Mask <<= 1;
			if (!this->Mask)
			{
				this->Mask = 1;
				++this->DWORDIndex;
			}
			return *this;
		}
		FORCEINLINE bool operator*() const
		{
			// Thesis: Once there are more elements in the BitArray than InlineData can hold it'll just allocate all of
			// them through SecondaryElements, leaving InlineData all true

			if (IteratedArray.NumBits < IteratedArray.Data.NumInlineBits())
			{
				return (bool)FBitReference(IteratedArray.Data.GetInlineElement(this->DWORDIndex), this->Mask);
			}
			else
			{
				return (bool)FBitReference(IteratedArray.Data.GetSecondaryElement(this->DWORDIndex), this->Mask);
			}
		}
		FORCEINLINE bool operator==(const FBitIterator& OtherIt) const
		{
			return Index == OtherIt.Index;
		}
		FORCEINLINE bool operator!=(const FBitIterator& OtherIt) const
		{
			return Index < /*=*/OtherIt.Index;
		}
		FORCEINLINE bool operator<(const int Other) const
		{
			return Index < Other;
		}
		FORCEINLINE bool operator>(const int Other) const
		{
			return Index < Other;
		}

		FORCEINLINE int GetIndex() const
		{
			return Index;
		}
	};

	class FSetBitIterator : public FRelativeBitReference
	{
	private:
		const TBitArray& IteratedArray;

		uint32_t UnvisitedBitMask;
		int CurrentBitIndex;
		int BaseBitIndex;

	public:
		FORCEINLINE FSetBitIterator(const TBitArray& ToIterate, int StartIndex)
			: FRelativeBitReference(StartIndex)
			, IteratedArray(const_cast<TBitArray&>(ToIterate))
			, UnvisitedBitMask((~0U) << (StartIndex & (NumBitsPerDWORD - 1)))
			, CurrentBitIndex(StartIndex)
			, BaseBitIndex(StartIndex & ~(NumBitsPerDWORD - 1))
		{
			if (StartIndex != IteratedArray.NumBits)
			{
				FindNextSetBit();
			}
		}
		FORCEINLINE FSetBitIterator(const TBitArray& ToIterate)
			: FRelativeBitReference(ToIterate.NumBits)
			, IteratedArray(const_cast<TBitArray&>(ToIterate))
			, UnvisitedBitMask(0)
			, CurrentBitIndex(ToIterate.NumBits)
			, BaseBitIndex(ToIterate.NumBits)
		{
		}

		FORCEINLINE FSetBitIterator& operator++()
		{
			UnvisitedBitMask &= ~this->Mask;

			FindNextSetBit();

			return *this;
		}
		FORCEINLINE bool operator*() const
		{
			return true;
		}

		FORCEINLINE bool operator==(const FSetBitIterator& Other) const
		{
			return CurrentBitIndex == Other.CurrentBitIndex;
		}
		FORCEINLINE bool operator!=(const FSetBitIterator& Other) const
		{
			return CurrentBitIndex < /*=*/Other.CurrentBitIndex;
		}

		FORCEINLINE explicit operator bool() const
		{
			return CurrentBitIndex < IteratedArray.NumBits;
		}

		FORCEINLINE int GetIndex() const
		{
			return CurrentBitIndex;
		}

	private:
		void FindNextSetBit()
		{
			// InlineData is the first 16-bytes of TBitArray
			const uint32_t* ArrayData = (IteratedArray.NumBits < IteratedArray.Data.NumInlineBits() ? (uint32_t*)(&IteratedArray) : IteratedArray.Data.SecondaryData);

			if (ArrayData)
			{
				const int ArrayNum = IteratedArray.NumBits;
				const int LastDWORDIndex = (ArrayNum - 1) / NumBitsPerDWORD;

				if (ArrayNum > this->DWORDIndex)
				{
					uint32_t RemainingBitMask = ArrayData[this->DWORDIndex] & UnvisitedBitMask;
					while (!RemainingBitMask)
					{
						++this->DWORDIndex;
						BaseBitIndex += NumBitsPerDWORD;

						if (this->DWORDIndex > LastDWORDIndex)
						{
							CurrentBitIndex += ArrayNum;
							return;
						}

						RemainingBitMask = ArrayData[this->DWORDIndex];
						UnvisitedBitMask = ~0;
					}

					const uint32_t NewRemainingBitMask = RemainingBitMask & (RemainingBitMask - 1);

					this->Mask = NewRemainingBitMask ^ RemainingBitMask;

					CurrentBitIndex = BaseBitIndex + NumBitsPerDWORD - 1 - CountLeadingZeros(this->Mask);

					if (CurrentBitIndex > ArrayNum)
					{
						CurrentBitIndex = ArrayNum;
					}
				}
			}
		}
	};

public:
	FORCEINLINE FBitIterator Iterator(int StartIndex)
	{
		return FBitIterator(*this, StartIndex);
	}
	FORCEINLINE FSetBitIterator SetBitIterator(int StartIndex)
	{
		return FSetBitIterator(*this, StartIndex);
	}

	FORCEINLINE FBitIterator begin()
	{
		return FBitIterator(*this, 0);
	}
	FORCEINLINE const FBitIterator begin() const
	{
		return FBitIterator(*this, 0);
	}
	FORCEINLINE FBitIterator end()
	{
		return FBitIterator(*this);
	}
	FORCEINLINE const FBitIterator end() const
	{
		return FBitIterator(*this);
	}

	FORCEINLINE FSetBitIterator SetBitsItBegin()
	{
		return FSetBitIterator(*this, 0);
	}
	FORCEINLINE const FSetBitIterator SetBitsItBegin() const
	{
		return FSetBitIterator(*this, 0);
	}
	FORCEINLINE const FSetBitIterator SetBitsItEnd()
	{
		return FSetBitIterator(*this);
	}
	FORCEINLINE const FSetBitIterator SetBitsItEnd() const
	{
		return FSetBitIterator(*this);
	}

	FORCEINLINE int Num() const
	{
		return NumBits;
	}
	FORCEINLINE int Max() const
	{
		return MaxBits;
	}
	FORCEINLINE bool IsSet(int Index) const
	{
		return *FBitIterator(*this, Index);
	}
	FORCEINLINE void Set(const int Index, const bool Value, bool bIsSettingAllZero = false)
	{
		const int DWORDIndex = (Index >> ((int)5));
		const int Mask = (1 << (Index & (((int)32) - 1)));

		if (!MaxBits)
			MaxBits = 128;

		if (!bIsSettingAllZero)
			NumBits = Index >= NumBits ? (Index < MaxBits ? Index + 1 : NumBits) : NumBits;

		FBitReference(Data[DWORDIndex], Mask).SetBit(Value);
	}
	FORCEINLINE void ZeroAll()
	{
		for (int i = 0; i < NumBits; i++)
		{
			Set(i, false, true);
		}
	}
	FORCEINLINE void Reset()
	{
		ZeroAll();
		NumBits = 0;
	}
};