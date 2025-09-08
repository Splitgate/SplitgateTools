#include "HttpSystem/HttpSystem.h"

#include "APortalWarsRaceGameMode.h"
#include "Engine/Kismet/UGameplayStatics.h"
#include "Engine/GameFramework/APlayerController.h"
#include "Engine/GameFramework/APlayerState.h"

#include "Parse/FParse.h"

#include "Memory/Hook.h"
#include "Strings/Strings.h"
#include "Steam/SteamUtilities.h"

#include "ImGuiNotify/ImGuiNotify.hpp"

static void (*HandleMatchHasEnded)(APortalWarsRaceGameMode*);
static void (*InitGame)(APortalWarsRaceGameMode*, const FString&, const FString&, FString&);
static bool (*LoadSubLevel)(APortalWarsRaceGameMode*);
static void (*InitRace)(APortalWarsRaceGameMode*);

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

	::InitRace = InitGameStart.Scan("5B E9").Add(2).Rel32();
	
	LOG_ADDRESS(::InitRace, "APortalWarsRaceGameMode::InitRace");
	HOOK(InitRace);
	
	if (HandleMatchHasEndedStart)
	{
		auto WorldTimeOffsetPtr = HandleMatchHasEndedStart.Scan("F2").Add(4);
		if (WorldTimeOffsetPtr)
			WorldTime_Offset = WorldTimeOffsetPtr.Deref();

		if (auto Ptr = WorldTimeOffsetPtr.Scan("F2").Add(4))
			FinalTime_Offset = Ptr.Deref();

		if (auto Ptr = WorldTimeOffsetPtr.Scan("80 BB").Add(2))
			bNewHighScore_Offset = Ptr.Deref();
	}

	if (auto InitRaceStr = Memory::FindStringRef("InitRace"))
	{
		if (auto Ptr = InitRaceStr.ReverseScan("45 0F B6").Add(5))
			Difficulty_Offset = Ptr.Deref();
	}
}

void APortalWarsRaceGameMode::SendRaceStatUpdate()
{
	FUserRaceCourseTime RaceEntry = FUserRaceCourseTime();
	RaceEntry.TimeMs = FinalTime() * 1000;

	ISteamUser* SteamUser = Steam::User();
	if (!SteamUser)
	{
		ImGui::InsertNotification({ ImGuiToastType::Error, 5000, "Uploading stats failed. No Steam Authentication."});
		return; // Bail no user
	}

	RaceEntry.PlatformUserId = std::to_string(SteamUser->GetSteamID().ConvertToUint64());
	RaceEntry.Map = GWorld->Name.ToStdString();

	RaceEntry.Difficulty = ERaceDifficulty::ToString(Difficulty());

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
		ImGui::InsertNotification({ ImGuiToastType::Error, 5000, "Uploading stats failed. No Steam Authentication."});

		return; // Failed to get encrypted ticket, do not allow request
	}

	std::string AuthenticationHeader = std::format("Bearer {}", BytesToHex(TicketBuffer, TicketSize));
	RaceReq.headers.insert({ "Authorization", AuthenticationHeader });
	RaceReq.body = RaceJson.dump();

	ImGui::InsertNotification({ ImGuiToastType::Info, 4000, "Attempting to upload race stats" });
	HttpJob(&HttpSystem::RaceBase, RaceReq, [RaceEntry](httplib::Response Resp, httplib::Error Err)
	{
		if (Err == httplib::Error::Success)
		{
			ImGui::InsertNotification({
				ImGuiToastType::Success, 5000, "Uploaded new record, %s on %s (%s)",
				std::to_string(RaceEntry.TimeMs / 1000), RaceEntry.Map.c_str(), RaceEntry.Difficulty
			});
		}
		else
		{
			ImGui::InsertNotification({
				ImGuiToastType::Error, 5000, "Uploading stats failed. %s - %s",
				httplib::to_string(Err).c_str(), Resp.body.c_str()
			});
		}
	});
}

void APortalWarsRaceGameMode::HandleMatchHasEnded()
{
	::HandleMatchHasEnded(this);

	// Ignore sending if offline or running locally, we only want to send if its a new highscore
	if (Steam::IsReady())
	{
		if (bNewHighScore())
		{
			SendRaceStatUpdate();
		}
		//else
		//{
		//	ImGui::InsertNotification({ ImGuiToastType::Success, 3000, "Ah you're well shit mate won't even bother uploading that one" });
		//}
	}
	else
	{
		ImGui::InsertNotification({ ImGuiToastType::Warning, 3000, "Currently running offline, no stats will be uploaded" });
	}
}

void APortalWarsRaceGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	FString NewDifficulty;
	if (FParse::Value(*Options, TEXT("Difficulty="), NewDifficulty))
	{
		OverrideDifficulty = ERaceDifficulty::FromString(NewDifficulty.ToString());
	}
	else
	{
		OverrideDifficulty = ERaceDifficulty::None;
	}

	::InitGame(this, MapName, Options, ErrorMessage);
}

bool APortalWarsRaceGameMode::LoadSubLevel()
{
	if (OverrideDifficulty != ERaceDifficulty::None)
	{
		Difficulty() = OverrideDifficulty;
		UE_LOG(LogRace, Warning, "Overriding difficulty with {}!", ERaceDifficulty::ToString(OverrideDifficulty));
	}

	return ::LoadSubLevel(this);
}

void APortalWarsRaceGameMode::InitRace()
{
	CountdownTime() = GSettings.Race.CountdownLength;
	::InitRace(this);
	//*reinterpret_cast<int*>(__int64(this) + 0x884) = 0;
}
