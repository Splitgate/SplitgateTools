#pragma once
#include "UObject.h"
#include "XProperty.h"

class UFunction;
class UStruct : public UObject
{
public:
	static void Init_PostEngine();

	PROPERTY(UStruct*, Next);
	PROPERTY(UStruct*, Super);
	PROPERTY(UFunction*, Children);
	PROPERTY(XProperty*, ChildProperties);
	PROPERTY(int, PropertiesSize);

	UStruct* GetSuperMost()
	{
		UStruct* Most = this;
		while (Most->Super())
			Most = Most->Super();
		return Most;
	}
};
