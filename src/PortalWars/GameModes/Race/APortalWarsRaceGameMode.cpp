#include "HttpSystem/HttpSystem.h"

#include "APortalWarsRaceGameMode.h"
#include "Engine/Kismet/UGameplayStatics.h"
#include "Engine/GameFramework/APlayerController.h"
#include "Engine/GameFramework/APlayerState.h"

#include "Memory/Hook.h"
#include "Strings/Strings.h"
#include "Steam/SteamUtilities.h"

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

void APortalWarsRaceGameMode::SendRaceStatUpdate()
{
	FUserRaceCourseTime RaceEntry = FUserRaceCourseTime();
	RaceEntry.TimeMs = GetFinalTime() * 1000;

	ISteamUser* SteamUser = Steam::SteamUser();
	if (!SteamUser)
		return; // Bail no user

	RaceEntry.PlatformUserId = std::to_string(SteamUser->GetSteamID().ConvertToUint64());
	RaceEntry.Map = GWorld->Name.ToStdString();
	RaceEntry.Difficulty = EDifficulty::ToString(EDifficulty::Type::Hard);

	json RaceJson;
	to_json(RaceJson, RaceEntry);

	httplib::Request RaceReq = httplib::Request();
	RaceReq.path = "https://racertest.vercel.app/api/update-pb";
	RaceReq.method = "POST";
	RaceReq.headers.insert({ "Content-Type", "application/json" });

	uint8 TicketBuffer[1024];
	uint32 TicketSize = 0;

	if (SteamUser->GetAuthSessionTicket(TicketBuffer, sizeof(TicketBuffer), &TicketSize) == k_HAuthTicketInvalid)
	{
		return; // Failed to get encrypted ticket, do not allow request
	}

	std::string EncryptedTicket = "";
	//for (int i = 0; i < sizeof(TicketBuffer); i++)
	//{
	//
	//}

	MessageBoxA(0, EncryptedTicket.c_str(), 0, 0);

	std::string AuthenticationHeader = "Bearer ";
	RaceReq.headers.insert({ "Authorization", AuthenticationHeader += EncryptedTicket});
	RaceReq.body = RaceJson.dump();

	HttpJob(&HttpSystem::ProxyClient, RaceReq, [](httplib::Response Resp)
		{
			// TODO: Show imgui popup bottom right to say stats uploaded successfully or why if not
		});
}

void APortalWarsRaceGameMode::HandleMatchHasEnded()
{
	::HandleMatchHasEnded(this);

	// Ignore sending if offline or running locally
	if (Steam::IsSteamReady())
		SendRaceStatUpdate();
}

