#pragma once
#include "PortalWars/GameModes/APortalWarsGameMode.h"

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
	enum Type : int
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

	Type inline FromString(std::string InDifficulty)
	{
		if (InDifficulty == "Easy")
		{
			return Type::Easy;
		}

		if (InDifficulty == "Medium")
		{
			return Type::Medium;
		}

		if (InDifficulty == "Hard")
		{
			return Type::Hard;
		}

		return Type::Easy;
	}
}

// temp gamemode, use APortalWarsGameMode as parent
class APortalWarsRaceGameMode : public APortalWarsGameMode
{
public:
	GENERATED_BODY(APortalWarsRaceGameMode, APortalWarsGameMode, "PortalWarsRaceGameMode", "/Script/PortalWars")

	static void Init_PreEngine();

	EDifficulty::Type& Difficulty();
	double& FinalTime();
	double& WorldTime();
	bool& bNewHighScore();

private:

	void SendRaceStatUpdate();
	void HandleMatchHasEnded();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	bool LoadSubLevel();

	struct RaceOffsets
	{
		static inline int Difficulty;
		static inline int WorldTime;
		static inline int FinalTime;
		static inline int bNewHighScore;
	};
};