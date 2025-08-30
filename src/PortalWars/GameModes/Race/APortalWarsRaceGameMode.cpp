#include "httplib/httplib.h"


#include "APortalWarsRaceGameMode.h"
#include "Memory/Hook.h"

#include <iostream>
#include <thread>

static void (*HandleMatchHasEnded)(APortalWarsRaceGameMode*);

void APortalWarsRaceGameMode::Init_PreEngine()
{
	Memory::Address HandleMatchHasEndedStart = Memory::FindPattern("F2 0F 10 81 98 08 00 00").FuncStart(); // TO:DO: this wont work before like 6.2 or 7.something, supports 8.0 so whatever
	::HandleMatchHasEnded = HandleMatchHasEndedStart;

	LOG_ADDRESS(::HandleMatchHasEnded, "APortalWarsRaceGameMode::HandleMatchHasEnded");
	HOOK(HandleMatchHasEnded);

	if (HandleMatchHasEndedStart)
	{
		auto WorldTimeOffset = HandleMatchHasEndedStart.Scan("F2").Add(4);
		if (WorldTimeOffset)
		{
			RaceOffsets::WorldTime = WorldTimeOffset.Deref();
		}

		auto FinalTimeOffset = WorldTimeOffset.Scan("F2").Add(4);
		if (FinalTimeOffset)
		{
			RaceOffsets::FinalTime = FinalTimeOffset.Deref();
		}

		auto bNewHighScoreOffset = WorldTimeOffset.Scan("80 BB").Add(2);
		if (bNewHighScoreOffset)
		{
			RaceOffsets::bNewHighScore = bNewHighScoreOffset.Deref();
		}
	}
}

double APortalWarsRaceGameMode::GetFinalTime()
{
	if (RaceOffsets::FinalTime != 0)
	{
		return Get<double>(RaceOffsets::FinalTime);
	}

	return 0;
}

double APortalWarsRaceGameMode::GetWorldTime()
{
	if (RaceOffsets::WorldTime != 0)
	{
		return Get<double>(RaceOffsets::WorldTime);
	}

	return 0;
}

bool APortalWarsRaceGameMode::GetNewHighScore()
{
	if (RaceOffsets::bNewHighScore != 0)
	{
		return Get<bool>(RaceOffsets::bNewHighScore);
	}

	return false;
}

void APortalWarsRaceGameMode::HandleMatchHasEnded()
{
	::HandleMatchHasEnded(this);

	HttpRequestList.push_back([]()
		{
			httplib::Client cli("http://racertest.vercel.app");
			cli.Post("/api/update-pb", "{\"platform\":\"STEAM\",\"platformUserId\":\"19416\",\"map\":\"Splitgate2\",\"timeMs\":1900,\"difficulty\":\"Medium\"}", "text/plain");
		});
}
