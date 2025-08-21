#pragma once
#include "UStruct.h"
#include "Engine/Kismet/UGameplayStatics.h"

class UClass : public UStruct
{
public:
	static void Init_PostEngine();
	
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
	
	UFunction* FindFunction(FName InName);
};

template<typename T>
T& UObject::Get(FName InName)
{
	auto off = Class->FindOffset(InName);
	if (off > 0)
		return Get<T>(off);
	
	// TODO: handle error
}


template<typename T>
T* StaticLoadObject(const wchar_t* InName)
{
	return (T*)StaticLoadObject(T::StaticClass(), InName);
}

template <typename T>
T* NewObject(UObject* Outer, UClass* Class)
{
	return (T*)UGameplayStatics::SpawnObject(Class, Outer);
}
