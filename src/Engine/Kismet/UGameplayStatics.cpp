#include "UGameplayStatics.h"
#include "Engine/Core/UObject/UClass.h"

UObject* UGameplayStatics::SpawnObject(UClass* ObjectClass, UObject* Outer)
{
	struct {
		UClass* ObjectClass;
		UObject* Outer;
		UObject* ReturnValue;
	} p = {
		ObjectClass, Outer
	};
	
	UFunction* Func = StaticClass()->FindFunction(L"SpawnObject");
	DefaultObject()->ProcessEvent(Func, &p);

	return p.ReturnValue;
}

APlayerController* UGameplayStatics::GetPlayerController(UObject* WorldContextObject, int32_t LocalPlayerIndex)
{
	// todo rewrite in cpp
	struct {
		UObject* WorldContextObject;
		int32_t LocalPlayerIndex;
		APlayerController* ReturnValue;
	} p = {
		WorldContextObject, LocalPlayerIndex
	};

	UFunction* Func = StaticClass()->FindFunction(L"GetPlayerController");
	DefaultObject()->ProcessEvent(Func, &p);

	return p.ReturnValue;
}