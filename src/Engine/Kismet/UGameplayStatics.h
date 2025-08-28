#pragma once
#include "UObject/UObject.h"

class UGameplayStatics : public UObject
{
public:
	GENERATED_BODY(UGameplayStatics, UObject, "GameplayStatics", "/Script/Engine")
	
	static UObject* SpawnObject(UClass* ObjectClass, UObject* Outer);

	static class APlayerController* GetPlayerController(UObject* WorldContextObject, int32_t LocalPlayerIndex);
};
