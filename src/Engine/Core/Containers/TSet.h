#pragma once
#include "FMemory.h"

#include "CoreTypes.h"
#include "TPair.h"
#include "TSparseArray.h"

template <typename ElementType>
class TSetElement
{
public:
	ElementType Value;
	mutable int32_t HashNextId;
	mutable int32_t HashIndex;

	TSetElement(ElementType InValue, int32_t InHashNextId, int32_t InHashIndex)
		: Value(InValue)
		, HashNextId(InHashNextId)
		, HashIndex(InHashIndex)
	{
	}

	FORCEINLINE TSetElement<ElementType>& operator=(const TSetElement<ElementType>& Other)
	{
		Value = Other.Value;
	}

	FORCEINLINE bool operator==(const TSetElement& Other) const
	{
		return Value == Other.Value;
	}
	FORCEINLINE bool operator!=(const TSetElement& Other) const
	{
		return Value != Other.Value;
	}
};

template <typename SetType>
class TSet
{
public:
	typedef TSetElement<SetType> ElementType;
	typedef TSparseArrayElementOrListLink<ElementType> ArrayElementType;

	TSparseArray<ElementType> Elements;

	TInlineAllocator<1>::ForElementType<int> Hash;
	int HashSize;

public:
	class FBaseIterator
	{
	private:
		TSet<SetType>& IteratedSet;
		TSparseArray<ElementType>::FBaseIterator ElementIt;

	public:
		FORCEINLINE FBaseIterator(const TSet<SetType>& InSet, TSparseArray<TSetElement<SetType>>::FBaseIterator InElementIt)
			: IteratedSet(const_cast<TSet<SetType>&>(InSet))
			, ElementIt(InElementIt)
		{
		}

		FORCEINLINE explicit operator bool() const
		{
			return (bool)ElementIt;
		}
		FORCEINLINE TSet<SetType>::FBaseIterator& operator++()
		{
			++ElementIt;
			return *this;
		}
		FORCEINLINE bool operator==(const TSet<SetType>::FBaseIterator& OtherIt) const
		{
			return ElementIt == OtherIt.ElementIt;
		}
		FORCEINLINE bool operator!=(const TSet<SetType>::FBaseIterator& OtherIt) const
		{
			return ElementIt != OtherIt.ElementIt;
		}
		FORCEINLINE TSet<SetType>::FBaseIterator& operator=(TSet<SetType>::FBaseIterator& OtherIt)
		{
			return ElementIt = OtherIt.ElementIt;
		}
		FORCEINLINE SetType& operator*()
		{
			return (*ElementIt).Value;
		}
		FORCEINLINE const SetType& operator*() const
		{
			return (*ElementIt).Value;
		}
		FORCEINLINE ElementType& operator->()
		{
			return *ElementIt;
		}
		FORCEINLINE const ElementType& operator->() const
		{
			return *ElementIt;
		}
		FORCEINLINE const int GetIndex() const
		{
			return ElementIt.GetIndex();
		}
		FORCEINLINE ElementType& GetSetElement()
		{
			return *ElementIt;
		}
		FORCEINLINE const ElementType& GetSetElement() const
		{
			return *ElementIt;
		}
		FORCEINLINE bool IsElementValid() const
		{
			return ElementIt.IsElementValid();
		}
	};

public:
	FORCEINLINE TSet<SetType>::FBaseIterator begin()
	{
		return TSet<SetType>::FBaseIterator(*this, Elements.begin());
	}
	FORCEINLINE const TSet<SetType>::FBaseIterator begin() const
	{
		return TSet<SetType>::FBaseIterator(*this, Elements.begin());
	}
	FORCEINLINE TSet<SetType>::FBaseIterator end()
	{
		return TSet<SetType>::FBaseIterator(*this, Elements.end());
	}
	FORCEINLINE const TSet<SetType>::FBaseIterator end() const
	{
		return TSet<SetType>::FBaseIterator(*this, Elements.end());
	}

	FORCEINLINE SetType& operator[](int Index)
	{
		return Elements[Index].ElementData.Value;
	}

	FORCEINLINE int Num() const
	{
		return Elements.Num();
	}
	FORCEINLINE bool IsValid() const
	{
		return Elements.Data.Data != nullptr && Elements.AllocationFlags.MaxBits > 0;
	}
	FORCEINLINE TSparseArray<ElementType>& GetElements()
	{
		return Elements;
	}
	FORCEINLINE const TSparseArray<ElementType>& GetElements() const
	{
		return Elements;
	}
	FORCEINLINE const TBitArray& GetAllocationFlags() const
	{
		return Elements.GetAllocationFlags();
	}
	FORCEINLINE bool IsIndexValid(int IndexToCheck) const
	{
		return Elements.IsIndexValid(IndexToCheck);
	}
	FORCEINLINE const bool Contains(const SetType& ElementToLookFor) const
	{
		if (Num() <= 0)
			return false;

		for (SetType Element : *this)
		{
			if (Element == ElementToLookFor)
				return true;
		}
		return false;
	}
	FORCEINLINE const int Find(const SetType& ElementToLookFor) const
	{
		for (auto It = this->begin(); It != this->end(); ++It)
		{
			if (*It == ElementToLookFor)
			{
				return It.GetIndex();
			}
		}
		return -1;
	}
	FORCEINLINE bool Remove(const SetType& ElementToRemove)
	{
		return Elements.RemoveAt(Find(ElementToRemove));
	}
	FORCEINLINE int AddSingle(SetType InElement, int InHashIndex = 0, int InHashNextId = 0)
	{
		if (!this->IsValid())
			return this->Initialize();

		return Elements.AddSingle({ InElement, InHashIndex, InHashNextId });
	}
	FORCEINLINE void Initialize(const int NumElementsToInitWith = 5)
	{
		if (this->IsValid())
			return;

		Elements.Data.MaxElements = NumElementsToInitWith;
		Elements.Data.Count = NumElementsToInitWith;
		Elements.Data.Data = (ArrayElementType*)(FMemory::Malloc(NumElementsToInitWith * sizeof(ElementType)));
		for (int i = 0; i < NumElementsToInitWith; i++)
		{
			Elements.Data.Data[i].PrevFreeIndex = i - 1;
			Elements.Data.Data[i].NextFreeIndex = i + 1;
		}

		Elements.FirstFreeIndex = 0;
		Elements.NumFreeIndices = NumElementsToInitWith;

		Elements.AllocationFlags.MaxBits = 128;
		Elements.AllocationFlags.NumBits = NumElementsToInitWith;
		Elements.AllocationFlags.ZeroAll();

		Hash = FMemory::Malloc(NumElementsToInitWith * sizeof(ElementType));
		HashSize = NumElementsToInitWith * sizeof(ElementType);
	}

	FORCEINLINE void Reset()
	{
		Elements.Reset();
		Hash.InlineData->Pad[0] = 0;
		Hash.InlineData->Pad[1] = 0;
		Hash.InlineData->Pad[2] = 0;
		Hash.InlineData->Pad[3] = 0;
		Hash.SecondaryData = nullptr;
	}
};
