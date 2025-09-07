#pragma once

#include "UObject/UObject.h"
#include "UObject/UClass.h"

class UPlayer : public UObject
{
public:
	GENERATED_BODY(UPlayer, UObject, "Player", "/Script/Engine")

	UPROPERTY(class APlayerController*, PlayerController);
	UPROPERTY(int32, CurrentNetSpeed);
	UPROPERTY(int32, ConfiguredInternetSpeed);
	UPROPERTY(int32, ConfiguredLanSpeed);
};
