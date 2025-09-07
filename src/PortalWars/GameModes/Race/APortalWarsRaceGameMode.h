#pragma once
#include "PortalWars/GameModes/APortalWarsGameMode.h"
#include "Strings/Strings.h"

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

namespace ERaceDifficulty
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
		case Easy:
			return "Easy";
		case Medium:
			return "Medium";
		case Hard:
			return "Hard";
		default:
			return "None";
		}
	}

	std::wstring inline ToWString(Type InDifficulty)
	{
		switch (InDifficulty)
		{
		case Easy:
			return L"Easy";
		case Medium:
			return L"Medium";
		case Hard:
			return L"Hard";
		default:
			return L"None";
		}
	}
	Type inline FromString(std::string InDifficulty)
	{
		if (Lowercase(InDifficulty) == "easy")
			return Easy;
		if (Lowercase(InDifficulty) == "medium")
			return Medium;
		if (Lowercase(InDifficulty) == "hard")
			return Hard;
		return None;
	}
}

// temp gamemode, use APortalWarsGameMode as parent
class APortalWarsRaceGameMode : public APortalWarsGameMode
{
public:
	GENERATED_BODY(APortalWarsRaceGameMode, APortalWarsGameMode, "PortalWarsRaceGameMode", "/Script/PortalWars")

	static void Init_PreEngine();

	PROPERTY(ERaceDifficulty::Type, Difficulty);
	PROPERTY(double, FinalTime);
	PROPERTY(double, WorldTime);
	PROPERTY(bool, bNewHighScore);

private:

	void SendRaceStatUpdate();
	void HandleMatchHasEnded();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	bool LoadSubLevel();

	static inline ERaceDifficulty::Type OverrideDifficulty;
};