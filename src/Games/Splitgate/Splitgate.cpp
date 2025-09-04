#include "Splitgate.h"

// Title Bar Entries
#include "UI/World/MapsMenuEntry.h"

// Settings Entries
#include "UI/Settings/SettingsEntry.h"

// Classes
#include "Engine/OnlineSubsystem/FOnlineFactoryEOSPlus.h"
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
}

void Splitgate::ConstructTitleBar()
{
	new MapsMenuEntry();
}

void Splitgate::GatherSettingsEntries()
{
	new SettingsEntry("Race Settings");
	new SettingsEntry("Forge Settings");
}