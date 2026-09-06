#pragma once
#include "UObject/UObject.h"
#include "Engine/Core/UObject/UClass.h"

#include "Globals/FForgeMap.h"

class UPortalWarsSaveGame : public UObject
{
public:
	GENERATED_BODY(UPortalWarsSaveGame, UObject, "PortalWarsSaveGame", "/Script/PortalWars")

	static void Init_PostEngine();

	UPROPERTY(UNPACK(TMap<FString, FForgeMap>), ForgeMaps)

	PROPERTY(FString, SlotName)
};