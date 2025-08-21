#pragma once
#include "FMemory.h"

template<typename T>
class TArray
{
public:
	T* Data;
	unsigned int Num;
	unsigned int Max;

	~TArray()
	{
		if (Data)
			FMemory::Free(Data);
	}

	// TODO: TArray copy constructor
	// TODO: TArray operators
	// TODO: TArray iterators
	// TODO: TArray Add
	// TODO: TArray Remove
};

