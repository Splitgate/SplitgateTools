#include "UWorld.h"
#include "Memory/Memory.h"

static bool (*ServerTravel)(UWorld*, const FString&, bool, bool);
static ENetMode(*InternalGetNetMode)(UWorld*);

void UWorld::Init_PreEngine()
{
	Game->World = Memory::FindStringRef(L"%03d_%s")
		.ReverseScan("48 8B ?? ?? ?? ?? 02").Add(3).Rel32();
	LOG_ADDRESS(Game->World, "GWorld");

	::ServerTravel = Memory::FindPattern("A9 FD FF FF FF 75 ?? 48 8B 03 48 8B CB FF 90 ?? ?? ?? ?? 45").Scan("E8").Add(1).Rel32();
	LOG_ADDRESS(::ServerTravel, "UWorld::ServerTravel");

	::InternalGetNetMode = Memory::FindStringRef(L" (client %d)").ReverseScan("E8").Add(1).Rel32();
	LOG_ADDRESS(::InternalGetNetMode, "UWorld::InternalGetNetMode");
}

bool UWorld::ServerTravel(const FString& InURL, bool bAbsolute, bool bShouldSkipGameNotify)
{
	return ::ServerTravel(this, InURL, bAbsolute, bShouldSkipGameNotify);
}

ENetMode UWorld::GetNetMode()
{
	return ::InternalGetNetMode(this);
}
