#pragma once
#include "UStruct.h"
#include "Engine/Kismet/UGameplayStatics.h"

class UClass : public UStruct
{
public:
	static void Init_PostEngine();
	
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
