#pragma once
#include "FString.h"

class FText
{
	struct FTextData
	{
		uint8_t Pad_0[0x28];
		FString TextSource;
	};

	FTextData* TextData;
	uint8_t Pad_8[0x10];

public:

	const FString& GetStringRef() const
	{
		return TextData->TextSource;
	}
	std::string ToString() const
	{
		return TextData->TextSource.ToString();
	}
};