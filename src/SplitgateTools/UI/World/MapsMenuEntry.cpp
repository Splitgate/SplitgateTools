#include "MapsMenuEntry.h"

#include "Engine/UWorld.h"
#include "Engine/Core/UObject/UClass.h"
#include "Engine/Kismet/UGameplayStatics.h"
#include "Engine/Kismet/UKismetSystemLibrary.h"

#include "Strings/Strings.h"
#include "Globals.h"

void MapsMenuEntry::OnMapPressed(GameMapEntry Map, const char* RaceType, bool bForceForRace)
{
	std::string TravelURL = Map.InternalName;
	
	ENetMode CurrentNetMode = GWorld->GetNetMode();
	if ((CurrentNetMode == ENetMode::NM_Client || CurrentNetMode == ENetMode::NM_Standalone)
		|| bForceForRace)
	{
		TravelURL += "?Default?closed";
	}
	else
	{
		TravelURL += "?Default";
	}
	
	GWorld->ServerTravel(StringToWide(TravelURL).c_str());
}

void MapsMenuEntry::Render()
{
	if (ImGui::BeginMenu("Playable"))
	{
		for (auto& Map : AllMaps)
		{
			if (!Map.HasSelectability(MapSelectability::Playable)) continue;

			if (ImGui::MenuItem(Map.DisplayName.c_str()))
			{
				OnMapPressed(Map);
			}
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Race"))
	{
		for (auto& Map : AllMaps)
		{
			if (!Map.HasSelectability(MapSelectability::Race)) continue;
	
			if (ImGui::MenuItem(Map.DisplayName.c_str()))
			{
				OnMapPressed(Map);
			}
		}
	
		ImGui::EndMenu();
	}
	
	if (ImGui::BeginMenu("Forge"))
	{
		for (auto& Map : AllMaps)
		{
			if (!Map.HasSelectability(MapSelectability::Forge)) continue;
	
			if (ImGui::MenuItem(Map.DisplayName.c_str()))
			{
				OnMapPressed(Map);
			}
		}
	
		ImGui::EndMenu();
	}
}

void MapsMenuEntry::OnCreate()
{
	// Gather all map entries

	return;

	static FName NAME_MapData = "MapData";

	TArray<FPrimaryAssetId> PrimaryMapAssets;
	UKismetSystemLibrary::GetPrimaryAssetIdList(NAME_MapData, &PrimaryMapAssets);

	for (int i = 0; i != PrimaryMapAssets.Num(); i++)
	{
		FPrimaryAssetId AssetId = PrimaryMapAssets[i];
		UObject* MapData = UKismetSystemLibrary::GetObjectFromPrimaryAssetId(AssetId);

		if (!MapData) continue;

		FString MapName = MapData->Get<FString>("MapPackageName");
		FString MapPackageName = MapData->Get<FString>("MapPackageName");
		TArray<int> Selectability = MapData->Get<TArray<int>>("Selectability");
		
		int SelectedBits = 0;
		for (int x = 0; x != Selectability.Num(); x++)
		{
			int GameSelectability = Selectability[x];
		
			switch (GameSelectability)
			{
				// Default
				case 1:
					SelectedBits |= MapSelectability::Playable;
					break;
		
				// Race
				case 2:
					SelectedBits |= MapSelectability::Race;
					break;
		
				// Forge
				case 3:
					SelectedBits |= MapSelectability::Forge;
					break;
		
				// QA
				case 4:
					SelectedBits |= MapSelectability::QA;
					break;
		
				default:
					break;
			}
		}

		AllMaps.push_back(
			{
				MapName.ToString(),
				MapPackageName.ToString(),
				SelectedBits
			});
	}

	//AllMaps.push_back(
	//	{
	//		"Stadium",
	//		"/Game/Maps/Stadium",
	//		MapSelectability::Playable | MapSelectability::Race
	//	});
	//AllMaps.push_back(
	//	{
	//		"Highwind",
	//		"/Game/Maps/Highwind",
	//		MapSelectability::Playable | MapSelectability::Race
	//	});
	//
	//AllMaps.push_back(
	//	{
	//		"Wet Ocean",
	//		"/Game/Maps/Forge_Island",
	//		MapSelectability::Forge
	//	});
	//
	//AllMaps.push_back(
	//	{
	//		"Vessel (Blockout)",
	//		"/Game/Maps/Vessel_Blockout_WIP",
	//		MapSelectability::Playable
	//	});
}
