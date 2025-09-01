#pragma once
#include "Engine/GameFramework/Game/AGameMode.h"

struct FUserRaceCourseTime
{
	std::string Platform;
	std::string PlatformUserId;
	std::string Map;
	double TimeMs;
	std::string Difficulty;

	// We only currently support steam / pc
	FUserRaceCourseTime()
		: Platform("STEAM")
	{ }

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(FUserRaceCourseTime,
		Platform,
		PlatformUserId,
		Map,
		TimeMs,
		Difficulty);
};

namespace EDifficulty
{
	enum class Type : int
	{
		None = 0,
		Easy = 1,
		Medium = 2,
		Hard = 3
	};

	std::string inline ToString(Type InDifficulty)
	{
		switch (InDifficulty)
		{
		case Type::Easy:
			return "Easy";

		case Type::Medium:
			return "Medium";

		case Type::Hard:
			return "Hard";

		default:
			return "None";
		}
		
		return "None";
	}
}

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

	void SendRaceStatUpdate();
	void HandleMatchHasEnded();

	struct RaceOffsets
	{
		static inline int WorldTime;
		static inline int FinalTime;
		static inline int bNewHighScore;
	};
};