#pragma once

#include "TSet.h"
#include "TBitArray.h"

template <typename ElementType>
union TSparseArrayElementOrListLink
{
	TSparseArrayElementOrListLink()
	{
	}
	TSparseArrayElementOrListLink(ElementType& InElement)
		: ElementData(InElement)
	{
	}
	TSparseArrayElementOrListLink(ElementType&& InElement)
		: ElementData(InElement)
	{
	}

	TSparseArrayElementOrListLink(int InPrevFree, int InNextFree)
		: PrevFreeIndex(InPrevFree)
		, NextFreeIndex(InNextFree)
	{
	}

	//TSparseArrayElementOrListLink<ElementType> operator=(const TSparseArrayElementOrListLink<ElementType>& Other)
	//{
	//    return TSparseArrayElementOrListLink(Other.NextFreeIndex, Other.PrevFreeIndex);
	//}

	/** If the element is allocated, its value is stored here. */
	ElementType ElementData;

	struct
	{
		/** If the element isn't allocated, this is a link to the previous element in the array's free list. */
		int PrevFreeIndex;

		/** If the element isn't allocated, this is a link to the next element in the array's free list. */
		int NextFreeIndex;
	};
};

template <typename ArrayType>
class TSparseArray
{
private:
	template <typename SetType>
	friend class TSet;

public:
	typedef TSparseArrayElementOrListLink<ArrayType> FSparseArrayElement;

public:

	TArray<FSparseArrayElement> Data;
	TBitArray AllocationFlags;
	int FirstFreeIndex;
	int NumFreeIndices;

	class FBaseIterator
	{
	private:
		TSparseArray<ArrayType>& IteratedArray;
		TBitArray::FSetBitIterator BitArrayIt;

	public:
		FORCEINLINE FBaseIterator(const TSparseArray<ArrayType>& Array, const TBitArray::FSetBitIterator BitIterator)
			: IteratedArray(const_cast<TSparseArray<ArrayType>&>(Array))
			, BitArrayIt(const_cast<TBitArray::FSetBitIterator&>(BitIterator))
		{
		}

		FORCEINLINE explicit operator bool() const
		{
			return (bool)BitArrayIt;
		}
		FORCEINLINE TSparseArray<ArrayType>::FBaseIterator& operator++()
		{
			++BitArrayIt;
			return *this;
		}
		FORCEINLINE ArrayType& operator*()
		{
			return IteratedArray[BitArrayIt.GetIndex()].ElementData;
		}
		FORCEINLINE const ArrayType& operator*() const
		{
			return IteratedArray[BitArrayIt.GetIndex()].ElementData;
		}
		FORCEINLINE ArrayType& operator->()
		{
			return IteratedArray[BitArrayIt.GetIndex()].ElementData;
		}
		FORCEINLINE const ArrayType& operator->() const
		{
			return IteratedArray[BitArrayIt.GetIndex()].ElementData;
		}
		FORCEINLINE bool operator==(const TSparseArray<ArrayType>::FBaseIterator& Other) const
		{
			return BitArrayIt == Other.BitArrayIt;
		}
		FORCEINLINE bool operator!=(const TSparseArray<ArrayType>::FBaseIterator& Other) const
		{
			return BitArrayIt != Other.BitArrayIt;
		}

		FORCEINLINE int GetIndex() const
		{
			return BitArrayIt.GetIndex();
		}
		FORCEINLINE bool IsElementValid() const
		{
			return *BitArrayIt;
		}
	};

public:
	FORCEINLINE TSparseArray<ArrayType>::FBaseIterator begin()
	{
		return TSparseArray<ArrayType>::FBaseIterator(*this, TBitArray::FSetBitIterator(AllocationFlags, 0));
	}
	FORCEINLINE const TSparseArray<ArrayType>::FBaseIterator begin() const
	{
		return TSparseArray<ArrayType>::FBaseIterator(*this, TBitArray::FSetBitIterator(AllocationFlags, 0));
	}
	FORCEINLINE TSparseArray<ArrayType>::FBaseIterator end()
	{
		return TSparseArray<ArrayType>::FBaseIterator(*this, TBitArray::FSetBitIterator(AllocationFlags));
	}
	FORCEINLINE const TSparseArray<ArrayType>::FBaseIterator end() const
	{
		return TSparseArray<ArrayType>::FBaseIterator(*this, TBitArray::FSetBitIterator(AllocationFlags));
	}

	FORCEINLINE FSparseArrayElement& operator[](uint32_t Index)
	{
		return *(FSparseArrayElement*)&Data[Index].ElementData;
	}
	FORCEINLINE const FSparseArrayElement& operator[](uint32_t Index) const
	{
		return *(const FSparseArrayElement*)&Data[Index].ElementData;
	}

	FORCEINLINE int Num() const
	{
		return Data.Num() - NumFreeIndices;
	}
	FORCEINLINE int GetNumFreeIndices() const
	{
		return NumFreeIndices;
	}
	FORCEINLINE int GetFirstFreeIndex() const
	{
		return FirstFreeIndex;
	}
	FORCEINLINE const TArray<FSparseArrayElement>& GetData() const
	{
		return Data;
	}
	FORCEINLINE const TBitArray& GetAllocationFlags() const
	{
		return AllocationFlags;
	}
	FORCEINLINE bool IsIndexValid(int IndexToCheck) const
	{
		return AllocationFlags.IsSet(IndexToCheck);
	}

	FORCEINLINE bool RemoveAt(const int IndexToRemove)
	{
		if (IndexToRemove >= 0 && IndexToRemove < Data.Num() && AllocationFlags.IsSet(IndexToRemove))
		{
			int PreviousFreeIndex = -1;
			int NextFreeIndex = -1;

			if (NumFreeIndices == 0)
			{
				FirstFreeIndex = IndexToRemove;
				Data[IndexToRemove] = { -1, -1 };
			}
			else
			{
				for (auto It = AllocationFlags.begin(); It != AllocationFlags.end(); ++It)
				{
					if (!It)
					{
						if (It.GetIndex() < IndexToRemove)
						{
							Data[IndexToRemove].PrevFreeIndex = It.GetIndex();
						}
						else if (It.GetIndex() > IndexToRemove)
						{
							Data[IndexToRemove].NextFreeIndex = It.GetIndex();
							break;
						}
					}
				}
			}
			AllocationFlags.Set(IndexToRemove, false);
			NumFreeIndices++;

			return true;
		}
		return false;
	}
	FORCEINLINE int Add(ArrayType InElement)
	{
		FSparseArrayElement Element(InElement);

		int NextFree;
		int OutIndex;
		if (FirstFreeIndex >= 1)
		{
			NextFree = Data[FirstFreeIndex].NextFreeIndex;
			Data[FirstFreeIndex] = Element;
			--NumFreeIndices;

			AllocationFlags.Set(FirstFreeIndex, true);

			if (NumFreeIndices >= 1)
			{
				OutIndex = NextFree;
				FirstFreeIndex = NextFree;
				Data[NextFree].PrevFreeIndex = -1;

				return OutIndex;
			}
		}
		else
		{
			Data.Add(Element);
			AllocationFlags.Set(Data.Num() - 1, true);

			return Data.Num() - 1;
		}
	}

	FORCEINLINE void Reset()
	{
		Data.Reset();
		FirstFreeIndex = -1;
		NumFreeIndices = 0;
		AllocationFlags.Reset();
	}
};