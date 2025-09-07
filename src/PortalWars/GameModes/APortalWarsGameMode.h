#pragma once
#include "Engine/GameFramework/Game/AGameMode.h"

class APortalWarsGameMode : public AGameMode
{
public:
	GENERATED_BODY(APortalWarsGameMode, AGameMode, "PortalWarsGameMode", "/Script/PortalWars")

	static void Init_PreEngine();

	PROPERTY(int, CountdownTime);
};