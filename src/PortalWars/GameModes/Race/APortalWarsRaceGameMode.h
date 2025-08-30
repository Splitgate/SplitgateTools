#pragma once
#include "Engine/GameFramework/Game/AGameMode.h"

// temp gamemode, use APortalWarsGameMode as parent
class APortalWarsRaceGameMode : public AGameMode
{
public:
	GENERATED_BODY(APortalWarsRaceGameMode, AGameMode, "PortalWarsRaceGameMode", "/Script/PortalWars")

	static void Init_PreEngine();

	double GetFinalTime();
	double GetWorldTime();
	bool GetNewHighScore();

private:

	void HandleMatchHasEnded();

	struct RaceOffsets
	{
		static inline int WorldTime;
		static inline int FinalTime;
		static inline int bNewHighScore;
	};
};