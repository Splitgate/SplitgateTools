#pragma once
#include "UStruct.h"

class UFunction : public UStruct
{
public:
	static void Init_PostEngine();

	PROPERTY(void*, NativeFunc);
};
