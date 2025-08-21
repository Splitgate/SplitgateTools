#include "UWorld.h"

void UWorld::Init_PreEngine()
{
	Game->World = Memory::FindStringRef(L"_Leaks.txt")
		.ReverseScan("48 8B ?? ?? ?? ?? ?? 48").Add(3).Rel32();
	LOG_ADDRESS(Game->World, "GWorld");
}
