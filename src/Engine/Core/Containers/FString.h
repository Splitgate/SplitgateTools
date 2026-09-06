#pragma once

#include "TArray.h"
#include <string>
#include <locale>
#include <tchar.h>

#include "CoreTypes.h"

// Disable "conversion from 'size_t' to 'unsigned int', possible loss of data"
#pragma warning (disable : 4267 4244 4624)

class FString : public TArray<wchar_t>
{
public:
	// TODO: FString constructors
	FString() {}

	inline bool IsValid() const
	{
		return Data != nullptr;
	}

	FString(const wchar_t* InString)
	{
		Max = Count = *InString ? std::wcslen(InString) + 1 : 0;
		if (Count)
		{
			int32_t ByteLength = (Count * sizeof(wchar_t));
			Data = (wchar_t*)FMemory::Malloc(ByteLength);
			FMemory::Memcpy(Data, InString, ByteLength);
		}
	}

	std::string ToString() const
	{
		if (Count == 0)
			return "";

		std::wstring w = ToWString();
		return std::string(w.begin(), w.end());
	}	

	std::wstring ToWString() const
	{
		if (Count <= 0)
			return L"";
		return Data;
	}

	// TODO: FString operators
	inline const wchar_t* operator*() const
	{
		return Count ? Data : L"";
	}
};