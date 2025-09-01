#include "HttpSystem/HttpSystem.h"

#include "APortalWarsRaceGameMode.h"
#include "Engine/Kismet/UGameplayStatics.h"
#include "Engine/GameFramework/APlayerController.h"
#include "Engine/GameFramework/APlayerState.h"

#include "Memory/Hook.h"
#include "Strings/Strings.h"
#include "Steam/SteamUtilities.h"

#include <algorithm>

static void (*HandleMatchHasEnded)(APortalWarsRaceGameMode*);

void APortalWarsRaceGameMode::Init_PreEngine()
{
	Memory::Address HandleMatchHasEndedStart = Memory::FindPattern("F2 0F 2C E8 FF 90").FuncStart();
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

	Memory::Address InitRace = Memory::FindStringRef("InitRace");
	if (InitRace)
	{
		Memory::Address DifficultyOffset = InitRace.ReverseScan("45 0F B6").Add(5);
		if (DifficultyOffset)
		{
			RaceOffsets::Difficulty = DifficultyOffset.Deref();
		}
	}
}

void APortalWarsRaceGameMode::SetDifficulty(EDifficulty::Type NewDifficulty)
{
	if (RaceOffsets::Difficulty != 0)
	{
		Get<EDifficulty::Type>(RaceOffsets::Difficulty) = NewDifficulty;
	}
}

EDifficulty::Type APortalWarsRaceGameMode::GetDifficulty()
{
	if (RaceOffsets::Difficulty != 0)
	{
		return Get<EDifficulty::Type>(RaceOffsets::Difficulty);
	}

	return EDifficulty::Type::None;
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

void APortalWarsRaceGameMode::SendRaceStatUpdate()
{
	FUserRaceCourseTime RaceEntry = FUserRaceCourseTime();
	RaceEntry.TimeMs = GetFinalTime() * 1000;

	ISteamUser* SteamUser = Steam::User();
	if (!SteamUser)
		return; // Bail no user

	RaceEntry.PlatformUserId = std::to_string(SteamUser->GetSteamID().ConvertToUint64());
	RaceEntry.Map = GWorld->Name.ToStdString();
	RaceEntry.Difficulty = EDifficulty::ToString(GetDifficulty());

	MessageBoxA(0, EDifficulty::ToString(GetDifficulty()).c_str(), 0, 0);

	json RaceJson;
	to_json(RaceJson, RaceEntry);

	httplib::Request RaceReq = httplib::Request();
	RaceReq.path = "https://racertest.vercel.app/api/update-pb";
	RaceReq.method = "POST";
	RaceReq.headers.insert({ "Content-Type", "application/json" });

	uint8 TicketBuffer[1024];
	uint32 TicketSize = 0; // 240 - last test

	if (SteamUser->GetAuthSessionTicket(TicketBuffer, sizeof(TicketBuffer), &TicketSize) == k_HAuthTicketInvalid)
	{
		return; // Failed to get encrypted ticket, do not allow request
	}

	std::string AuthenticationHeader = std::format("Bearer {}", BytesToHex(TicketBuffer, TicketSize));
	RaceReq.headers.insert({ "Authorization", AuthenticationHeader });
	RaceReq.body = RaceJson.dump();

	HttpJob(&HttpSystem::RaceBase, RaceReq, [](httplib::Response Resp)
		{
			// TODO: Show imgui popup bottom right to say stats uploaded successfully or why if not
		});
}

void APortalWarsRaceGameMode::HandleMatchHasEnded()
{
	::HandleMatchHasEnded(this);

	// Ignore sending if offline or running locally
	if (Steam::IsReady())
	{
		SendRaceStatUpdate();
	}
	else // TODO: Offline notification - ruby
	{

	}
}

