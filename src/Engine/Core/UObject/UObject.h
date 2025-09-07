#pragma once

#include "FName.h"
#include "Containers/TArray.h"
#include "ObjectMacros.h"
#include "CoreTypes.h"

UObject* StaticLoadObject(UClass* InClass, const wchar_t* InName);
template<typename T = UObject>
T* StaticLoadObject(const wchar_t* InName); // Defined in UClass.h

template<typename T = UObject>
T* NewObject(UObject* Outer, UClass* Class = T::StaticClass()); // Defined in UClass.h

class UObject
{
public:
	static void Init_PreEngine();
	
	static UClass* StaticClass();
	virtual ~UObject() = default;
	
	EObjectFlags ObjectFlags;
	int Index;
	UClass* Class;
	FName Name;
	UObject* Outer;

	void ProcessEvent(UFunction* Func, void* Params);
	
	template<typename T = UObject>
	static T* Load(const wchar_t* InName) {
		return StaticLoadObject<T>(InName);
	}

	template<typename T>
	T& Get(int Offset) {
		return *reinterpret_cast<T*>(uintptr_t(this) + Offset);
	}
	template<typename T>
	T& Get(FName InName); // Defined in UClass.h
};
