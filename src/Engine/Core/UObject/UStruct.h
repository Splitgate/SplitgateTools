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
	
	XProperty* FindProperty(FName InName)
	{
		for (UStruct* Class = this; Class; Class = Class->Super())
			for (XProperty* P = Class->ChildProperties(); P; P = P->Next())
				if (P->Name() == InName)
					return P;
		return 0;
	}
	
	int FindOffset(FName InName)
	{
		XProperty* Prop = FindProperty(InName);
		return Prop ? Prop->Offset() : -1;
	}
};
