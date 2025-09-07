#include "UGameInstance.h"
#include "ULocalPlayer.h"

APlayerController* UGameInstance::GetFirstLocalPlayerController(UWorld* World)
{
	TArray<ULocalPlayer*>& Players = LocalPlayers();
	for (int i = 0; i != Players.Num(); i++)
	{
		if (Players[i] && Players[i]->PlayerController())
		{
			return Players[i]->PlayerController();
		}
	}

    return nullptr;
}