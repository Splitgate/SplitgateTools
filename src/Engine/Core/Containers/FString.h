#pragma once

#include "TArray.h"
#include <string>
#include <locale>

// Disable "conversion from 'size_t' to 'unsigned int', possible loss of data"
#pragma warning (disable : 4267)

class FString : public TArray<wchar_t>
{
public:
	// TODO: FString constructors
	FString() {}

	FString(const wchar_t* InString)
	{
		Max = Num = *InString ? std::wcslen(InString) + 1 : 0;
		if (Num)
		{
			int32_t ByteLength = (Num * sizeof(wchar_t));
			Data = (wchar_t*)FMemory::Malloc(ByteLength);
			FMemory::Memcpy(Data, InString, ByteLength);
		}
	}

	std::string ToString() const
	{
		if (Num == 0)
			return "";
		size_t Length = std::wcslen(Data);
		std::string OutString(Length, '\0');
		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + Length, '?', &OutString[0]);
		return OutString;
	}	

	// TODO: FString operators
	inline const wchar_t* operator*() const
	{
		return Num ? Data : L"";
	}
};
