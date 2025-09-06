#include "Splitgate.h"

// Title Bar Entries
#include "UI/World/MapsMenuEntry.h"

// Settings Entries
#include "UI/Settings/SettingsTab.h"

// Classes
#include "Engine/OnlineSubsystem/FOnlineFactoryEOSPlus.h"
#include "PortalWars/UPortalWarsGameEngine.h"
#include "PortalWars/GameModes/APortalWarsGameMode.h"
#include "PortalWars/GameModes/Race/APortalWarsRaceGameMode.h"

Splitgate::Splitgate()
{
	bShouldRenderUI = true;
	
	GSettings.Load();
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
}

void Splitgate::PopulateTitleBarEntries(std::vector<std::unique_ptr<UITitleBarEntry>>& Entries)
{
	Entries.push_back(std::make_unique<MapsMenuEntry>());
}

void Splitgate::PopulateSettingsTabs(std::vector<std::unique_ptr<SettingsTab>>& Tabs)
{
	// todo: seperate classes for each tab instead of using base class
	Tabs.push_back(std::make_unique<SettingsTab>("Race Settings"));
	Tabs.push_back(std::make_unique<SettingsTab>("Misc Settings"));
}