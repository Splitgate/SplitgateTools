#pragma once
#include "FMemory.h"

#include "CoreTypes.h"

template <typename KeyType, typename ValueType>
class TPair
{
public:
	KeyType First;
	ValueType Second;

public:
	TPair(KeyType Key, ValueType Value)
		: First(Key)
		, Second(Value)
	{
	}

public:
	FORCEINLINE KeyType& Key()
	{
		return First;
	}
	FORCEINLINE const KeyType& Key() const
	{
		return First;
	}
	FORCEINLINE ValueType& Value()
	{
		return Second;
	}
	FORCEINLINE const ValueType& Value() const
	{
		return Second;
	}
};