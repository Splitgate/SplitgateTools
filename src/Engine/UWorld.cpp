#include "UWorld.h"

void UWorld::Init_PreEngine()
{
	Game->World = Memory::FindStringRef(L"%03d_%s")
		.ReverseScan("48 8B ?? ?? ?? ?? 02").Add(3).Rel32();
	LOG_ADDRESS(Game->World, "GWorld");
}
