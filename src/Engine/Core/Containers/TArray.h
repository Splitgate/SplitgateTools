#pragma once
#include "FMemory.h"

template<typename T>
class TArray
{
public:
	T* Data;
	unsigned int Count;
	unsigned int Max;

	TArray()
		: Data(nullptr), Count(0), Max(0)
	{ }

	TArray(TArray<T>& OtherArray)
	{
		Count = OtherArray.Count;
		Max = OtherArray.Max;

		Data = (T*)FMemory::Malloc(Max);
		memcpy(Data, OtherArray.Data, Max);
	}

	~TArray()
	{
		FMemory::Free(Data);
	}

	inline unsigned int Num() const
	{
		return Count;
	}

	inline T& operator[](int i)
	{
		return Data[i];
	}

	inline const T& operator[](int i) const
	{
		return Data[i];
	}

	inline T& At(int i)
	{
		return Data[i];
	}

	inline void Free()
	{
		Count = 0;
		Max = 0;
		if (Data)
			FMemory::Free(Data);
	}

	// TODO: TArray copy constructor
	// TODO: TArray operators
	// TODO: TArray iterators
	// TODO: TArray Add
	// TODO: TArray Remove
};

