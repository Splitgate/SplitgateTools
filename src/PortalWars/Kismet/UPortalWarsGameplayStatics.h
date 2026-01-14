#pragma once
#include "UObject/UObject.h"
#include "UObject/FPrimaryAssetId.h"

class UPortalWarsGameplayStatics : public UObject
{
public:
	GENERATED_BODY(UPortalWarsGameplayStatics, UObject, "PortalWarsGameplayStatics", "/Script/PortalWars")

	static class UPortalWarsSaveGame* GetSaveGameForLocalPlayer(class ULocalPlayer* LocalPlayer);
};
