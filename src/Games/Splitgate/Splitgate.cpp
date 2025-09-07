#include "Splitgate.h"

// Title Bar Entries
#include "UI/TitleBar/MainTitleBarEntry.h"
#include "UI/TitleBar/MapsTitleBarEntry.h"

// Settings Tabs
#include "UI/Windows/Settings/Tabs/MiscSettingsTab.h"
#include "UI/Windows/Settings/Tabs/RaceSettingsTab.h"

// Classes
#include "Engine/OnlineSubsystem/FOnlineFactoryEOSPlus.h"
#include "PortalWars/UPortalWarsGameEngine.h"
#include "PortalWars/UPortalWarsGameInstance.h"
#include "PortalWars/GameModes/APortalWarsGameMode.h"
#include "PortalWars/GameModes/Race/APortalWarsRaceGameMode.h"

Splitgate::Splitgate()
{
	bShouldRenderUI = true;
}

void Splitgate::Init_PreEngine()
{
	BaseGame::Init_PreEngine();
	
	// Patch for fixing low fps when not launching through Steam
	FOnlineFactoryEOSPlus::Init_PreEngine();
	
	APortalWarsGameMode::Init_PreEngine();
	APortalWarsRaceGameMode::Init_PreEngine();
}

void Splitgate::Init_PostEngine()
{
	BaseGame::Init_PostEngine();
	
	UPortalWarsGameEngine::Init_PostEngine();
	FGameModeConfig::Init_PostEngine();
	
	GSettings.Load();
}

void Splitgate::PopulateTitleBarEntries(std::vector<std::unique_ptr<UITitleBarEntryBase>>& Entries)
{
	Entries.emplace_back(std::make_unique<MainTitleBarEntry>());
	Entries.emplace_back(std::make_unique<MapsTitleBarEntry>());
}

void Splitgate::PopulateSettingsTabs(std::vector<std::unique_ptr<SettingsTabBase>>& Tabs)
{
	// todo: seperate classes for each tab instead of using base class
	Tabs.emplace_back(std::make_unique<RaceSettingsTab>());
	Tabs.emplace_back(std::make_unique<MiscSettingsTab>());
}