#pragma once
#include "Engine/GameFramework/UGameInstance.h"
#include "GameModes/Race/APortalWarsRaceGameMode.h"

struct FGameModeConfig
{
	PROPERTY(EDifficulty::Type, RaceDifficulty);
	static void Init_PostEngine();
};

struct FGameConfig
{
	GENERATED_USTRUCT_BODY("GameConfig", "/Script/PortalWarsGlobals")
	UPROPERTY_STRUCT(FGameModeConfig, GameModeConfig);
};

class UPortalWarsGameInstance : public UGameInstance
{
public:
	GENERATED_BODY(UPortalWarsGameInstance, UGameInstance, "PortalWarsGameInstance", "/Script/PortalWars")
	
	UPROPERTY(FGameConfig, GameSettings);
};
