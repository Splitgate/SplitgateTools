#include "HttpSystem/HttpSystem.h"

#include "APortalWarsRaceGameMode.h"
#include "Engine/Kismet/UGameplayStatics.h"
#include "Engine/GameFramework/APlayerController.h"
#include "Engine/GameFramework/APlayerState.h"

#include "Parse/FParse.h"

#include "Memory/Hook.h"
#include "Strings/Strings.h"
#include "Steam/SteamUtilities.h"

static void (*HandleMatchHasEnded)(APortalWarsRaceGameMode*);
static void (*InitGame)(APortalWarsRaceGameMode*, const FString&, const FString&, FString&);
static bool (*LoadSubLevel)(APortalWarsRaceGameMode*);

void APortalWarsRaceGameMode::Init_PreEngine()
{
	Memory::Address HandleMatchHasEndedStart = Memory::FindPattern("F2 0F 2C E8 FF 90").FuncStart();
	::HandleMatchHasEnded = HandleMatchHasEndedStart;

	LOG_ADDRESS(::HandleMatchHasEnded, "APortalWarsRaceGameMode::HandleMatchHasEnded");
	HOOK(HandleMatchHasEnded);

	Memory::Address InitGameStart = Memory::FindPattern("C7 83 ?? ?? 00 00 10 0E 00 00 C6 83 ?? ?? 00 00 00 C6 83 ?? ?? 00 00 00").FuncStart();
	::InitGame = InitGameStart;

	LOG_ADDRESS(::InitGame, "APortalWarsRaceGameMode::InitGame");
	HOOK(InitGame);

	Memory::Address LoadSubLevelStart = Memory::FindStringRef(L"Race%s").FuncStart();
	::LoadSubLevel = LoadSubLevelStart;

	LOG_ADDRESS(::LoadSubLevel, "APortalWarsRaceGameMode::LoadSubLevel");
	HOOK(LoadSubLevel);

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

EDifficulty::Type& APortalWarsRaceGameMode::Difficulty()
{
	return Get<EDifficulty::Type>(RaceOffsets::Difficulty);
}

double& APortalWarsRaceGameMode::FinalTime()
{
	return Get<double>(RaceOffsets::FinalTime);
}

double& APortalWarsRaceGameMode::WorldTime()
{
	return Get<double>(RaceOffsets::WorldTime);
}

bool& APortalWarsRaceGameMode::bNewHighScore()
{
	return Get<bool>(RaceOffsets::bNewHighScore);
}

void APortalWarsRaceGameMode::SendRaceStatUpdate()
{
	FUserRaceCourseTime RaceEntry = FUserRaceCourseTime();
	RaceEntry.TimeMs = FinalTime() * 1000;

	ISteamUser* SteamUser = Steam::User();
	if (!SteamUser)
		return; // Bail no user

	RaceEntry.PlatformUserId = std::to_string(SteamUser->GetSteamID().ConvertToUint64());
	RaceEntry.Map = GWorld->Name.ToStdString();
	RaceEntry.Difficulty = EDifficulty::ToString(Difficulty());

	json RaceJson;
	to_json(RaceJson, RaceEntry);

	httplib::Request RaceReq = httplib::Request();
	RaceReq.path = "/api/update-pb";
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

	HttpJob(&HttpSystem::RaceBase, RaceReq, [](httplib::Response Resp, std::string Err)
		{
			// TODO: Show imgui popup bottom right to say stats uploaded successfully or why if not
		});
}

void APortalWarsRaceGameMode::HandleMatchHasEnded()
{
	::HandleMatchHasEnded(this);

	// Ignore sending if offline or running locally, we only want to send if its a new highscore
	if (Steam::IsReady() && bNewHighScore())
	{
		SendRaceStatUpdate();
	}
	else // TODO: Offline notification - ruby
	{

	}
}

void APortalWarsRaceGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	FString NewDifficulty;
	if (FParse::Value(*Options, TEXT("Difficulty="), NewDifficulty))
	{
		OverrideDifficulty = EDifficulty::FromString(NewDifficulty.ToString());
	}

	// CountdownTime() = 250;
	::InitGame(this, MapName, Options, ErrorMessage);
}

bool APortalWarsRaceGameMode::LoadSubLevel()
{
	if (OverrideDifficulty != EDifficulty::Type::None)
	{
		Difficulty() = OverrideDifficulty;
		UE_LOG(LogRace, Warning, "Overriding difficulty with {}!", EDifficulty::ToString(OverrideDifficulty));
	}

	return ::LoadSubLevel(this);
}