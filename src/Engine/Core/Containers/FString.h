#pragma once

#include "TArray.h"
#include <string>
#include <locale>
#include <tchar.h>

// Disable "conversion from 'size_t' to 'unsigned int', possible loss of data"
#pragma warning (disable : 4267)

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
		size_t Length = std::wcslen(Data);
		std::string OutString(Length, '\0');
		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + Length, '?', &OutString[0]);
		return OutString;
	}	

	// TODO: FString operators
	inline const wchar_t* operator*() const
	{
		return Count ? Data : L"";
	}
};