#pragma once
#include "UObject/UObject.h"
#include "Engine/Core/UObject/UClass.h"

#include "Globals/FForgeMap.h"

class UPortalWarsSaveGame : public UObject
{
public:
	GENERATED_BODY(UPortalWarsSaveGame, UObject, "PortalWarsSaveGame", "/Script/PortalWars")

	PROPERTY(UNPACK(TMap<FString, FForgeMap>), ForgeMaps)
};