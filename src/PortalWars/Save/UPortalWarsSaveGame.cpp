#include "UPortalWarsSaveGame.h"
#include "Engine/Core/UObject/UClass.h"

#include "Globals.h"
#include "Memory/Memory.h"

void UPortalWarsSaveGame::Init_PostEngine()
{
	int LastGameVersion_Offset = UPortalWarsSaveGame::DefaultObject()->Class->FindOffset("LastGameVersion");

	SlotName_Offset = LastGameVersion_Offset + sizeof(FString);
	LOG_OFFSET(SlotName_Offset, "UPortalWarsSaveGame::SlotName_Offset");
}