#pragma once

#include "FMemory.h"
#include "TSet.h"

template<typename KeyType, typename ValueType>
struct TMap
{
public:
	typedef TPair<KeyType, ValueType> ElementType;

public:
	TSet<ElementType> Pairs;

public:
	class FBaseIterator
	{
	public:
		TMap<KeyType, ValueType>& IteratedMap;
		TSet<ElementType>::FBaseIterator SetIt;

	public:
		FBaseIterator(TMap<KeyType, ValueType>& InMap, TSet<ElementType>::FBaseIterator InSet)
			: IteratedMap(InMap)
			, SetIt(InSet)
		{
		}
		FORCEINLINE TMap<KeyType, ValueType>::FBaseIterator operator++()
		{
			++SetIt;
			return *this;
		}
		FORCEINLINE TMap<KeyType, ValueType>::ElementType& operator*()
		{
			return *SetIt;
		}
		FORCEINLINE const TMap<KeyType, ValueType>::ElementType& operator*() const
		{
			return *SetIt;
		}
		FORCEINLINE bool operator==(const TMap<KeyType, ValueType>::FBaseIterator& Other) const
		{
			return SetIt == Other.SetIt;
		}
		FORCEINLINE bool operator!=(const TMap<KeyType, ValueType>::FBaseIterator& Other) const
		{
			return SetIt != Other.SetIt;
		}
		FORCEINLINE bool IsElementValid() const
		{
			return SetIt.IsElementValid();
		}
	};

	FORCEINLINE TMap<KeyType, ValueType>::FBaseIterator begin()
	{
		return TMap<KeyType, ValueType>::FBaseIterator(*this, Pairs.begin());
	}
	FORCEINLINE const TMap<KeyType, ValueType>::FBaseIterator begin() const
	{
		return TMap<KeyType, ValueType>::FBaseIterator(*this, Pairs.begin());
	}
	FORCEINLINE TMap<KeyType, ValueType>::FBaseIterator end()
	{
		return TMap<KeyType, ValueType>::FBaseIterator(*this, Pairs.end());
	}
	FORCEINLINE const TMap<KeyType, ValueType>::FBaseIterator end() const
	{
		return TMap<KeyType, ValueType>::FBaseIterator(*this, Pairs.end());
	}
	FORCEINLINE ValueType& operator[](const KeyType& Key)
	{
		return this->GetByKey(Key);
	}
	FORCEINLINE const ValueType& operator[](const KeyType& Key) const
	{
		return this->GetByKey(Key);
	}
	FORCEINLINE int32_t Num() const
	{
		return Pairs.Num();
	}
	FORCEINLINE bool IsValid() const
	{
		return Pairs.IsValid();
	}
	FORCEINLINE bool IsIndexValid(int32_t IndexToCheck) const
	{
		return Pairs.IsIndexValid(IndexToCheck);
	}
	FORCEINLINE void Initialize(const int32_t NumElementsToInitWith = 5)
	{
		return Pairs.Initialize(NumElementsToInitWith);
	}
	FORCEINLINE bool Contains(KeyType& ElementToLookFor)
	{
		for (auto Element : *this)
		{
			if (Element.Key() == ElementToLookFor)
				return true;
		}
		return false;
	}
	FORCEINLINE bool Contains(const KeyType& ElementToLookFor) const
	{
		for (auto Element : *this)
		{
			if (Element.Key() == ElementToLookFor)
				return true;
		}
		return false;
	}
	FORCEINLINE int32_t AddSingle(KeyType InKey, ValueType InValue)
	{
		return Pairs.AddSingle({ InKey, InValue });
	}
	FORCEINLINE int32_t AddSingle(ElementType InElement)
	{
		return Pairs.AddSingle(InElement);
	}
	FORCEINLINE ValueType& GetByKey(const KeyType& Key)
	{
		for (auto Pair : *this)
		{
			if (Pair.Key() == Key)
			{
				return Pair.Value();
			}
		}
	}
	FORCEINLINE ValueType GetByKeyNoRef(const KeyType& Key)
	{
		for (auto Pair : *this)
		{
			if (Pair.Key() == Key)
			{
				return Pair.Value();
			}
		}
	}
};