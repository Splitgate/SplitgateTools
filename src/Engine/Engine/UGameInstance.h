#pragma once
#include "UObject/UObject.h"
#include "UObject/UClass.h"

class UGameInstance : public UObject
{
public:
	GENERATED_BODY(UGameInstance, UObject, "GameInstance", "/Script/Engine")

	UPROPERTY(TArray<class ULocalPlayer*>, LocalPlayers);

	class APlayerController* GetFirstLocalPlayerController(class UWorld* World = nullptr);
};
