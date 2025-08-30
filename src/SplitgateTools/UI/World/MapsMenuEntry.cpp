#include "MapsMenuEntry.h"

#include "Engine/UWorld.h"
#include "Engine/Core/UObject/UClass.h"
#include "Engine/Kismet/UGameplayStatics.h"
#include "Engine/Kismet/UKismetSystemLibrary.h"
#include "Engine/GameFramework/APlayerController.h"

#include "Strings/Strings.h"
#include "Globals.h"

void GameMapEntry::OnPressed(const std::wstring& RaceType)
{
	std::wstring TravelURL = InternalName;
		if (RaceType != L"" && HasSelectability(MapSelectability::Race))
	{
		TravelURL += L"?Default?Game=RACE";

	}
	else
	{
		TravelURL += L"?Default";
	}

	GWorld->ServerTravel(TravelURL.c_str());
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
				Map.OnPressed();
			}
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Race"))
	{
		for (auto& Map : AllMaps)
		{
			if (!Map.HasSelectability(MapSelectability::Race)) continue;
	
			if (ImGui::BeginMenu(Map.DisplayName.c_str()))
			{
				if (ImGui::MenuItem("Easy"))
				{
					Map.OnPressed(L"Easy");
				}

				if (ImGui::MenuItem("Medium"))
				{
					Map.OnPressed(L"Medium");
				}

				if (ImGui::MenuItem("Hard"))
				{
					Map.OnPressed(L"Hard");
				}

				ImGui::EndMenu();
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
				Map.OnPressed();
			}
		}
	
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Misc"))
	{
		for (auto& Map : AllMaps)
		{
			if (!Map.HasSelectability(MapSelectability::Misc)) continue;

			if (ImGui::MenuItem(Map.DisplayName.c_str()))
			{
				Map.OnPressed();
			}
		}

		ImGui::EndMenu();
	}
}

void MapsMenuEntry::OnCreate()
{
	// Gather all map entries

	//TArray<FPrimaryAssetId> PrimaryMapAssets{};
	//UKismetSystemLibrary::GetPrimaryAssetIdList(FName("MapData"), PrimaryMapAssets);
	//UE_LOG(LogImGui, Warning, "PrimaryMapAssets.Num() {}", PrimaryMapAssets.Num());

	//for (int i = 0; i != PrimaryMapAssets.Num(); i++)
	//{
		//FPrimaryAssetId AssetId = PrimaryMapAssets[i];

		//UObject* MapData = UKismetSystemLibrary::GetObjectFromPrimaryAssetId(AssetId);
		//if (!MapData) {
		//	UE_LOG(LogImGui, Warning, "MapData Invalid");
		//}

		//UObject* MapData = StaticLoadObject<UObject>(L"/Game/Maps/_DataAssets/Map_Ozone_Blockout");
		//UObject* TestData = StaticLoadObject<UObject>(L"/Game/Maps/Blockouts/Ozone_Blockout_Playtest");
		//
		//if (MapData)
		//{
		//	UE_LOG(LogImGui, Warning, "MapData: {}", MapData->Name.ToString().ToString());
		//}
		//
		//if (TestData)
		//{
		//	UE_LOG(LogImGui, Warning, "TestData: {}", MapData->Name.ToString().ToString());
		//}

		//FString MapName = MapData->Get<FString>("MapPackageName");
		//FString MapPackageName = MapData->Get<FString>("MapPackageName");
		//
		//FString SelectabilityString = L"Default,Race";
		//
		//int SelectedBits = 0;
		//std::vector<std::string> SplitSelections = Split(SelectabilityString.ToString(), ',');
		//for (auto& Selection : SplitSelections)
		//{
		//	if (Selection == "Default")
		//		SelectedBits |= MapSelectability::Playable;
		//
		//	if (Selection == "Race")
		//		SelectedBits |= MapSelectability::Race;
		//
		//	if (Selection == "Forge")
		//		SelectedBits |= MapSelectability::Forge;
		//
		//	if (Selection == "QA")
		//		SelectedBits |= MapSelectability::QA;
		//}
		//
		//AllMaps.push_back(
		//	{
		//		MapName.ToString(),
		//		MapPackageName.ToString(),
		//		SelectedBits
		//	});
	//}

#define ADD_GUI_MAP(Name, Path, Types) \
	AllMaps.push_back( \
		{ \
			Name, \
			TEXT(Path), \
			Types \
		}) \

	ADD_GUI_MAP("Abyss", "/Game/Maps/Abyss", MapSelectability::Playable | MapSelectability::Race);
	ADD_GUI_MAP("Atlantis", "/Game/Maps/Atlantis", MapSelectability::Playable | MapSelectability::Race);
	ADD_GUI_MAP("Crag", "/Game/Maps/Crag", MapSelectability::Playable | MapSelectability::Race);
	ADD_GUI_MAP("Decay (Blockout)", "/Game/Maps/Decay_Blockout_WIP", MapSelectability::Playable);
	ADD_GUI_MAP("Drift (Blockout)", "/Game/Maps/Drift_Blockout_WIP", MapSelectability::Playable);
	ADD_GUI_MAP("Foregone Destruction", "/Game/Maps/Foregone_Destruction", MapSelectability::Playable | MapSelectability::Race);
	ADD_GUI_MAP("Flat Earth", "/Game/Maps/Forge_Flat_Earth", MapSelectability::Forge);
	ADD_GUI_MAP("Wet Ocean", "/Game/Maps/Forge_Island", MapSelectability::Forge);
	ADD_GUI_MAP("Frontier (Blockout)", "/Game/Maps/Blockouts/Frontier_Blockout_WIP_Playtest", MapSelectability::Playable);
	ADD_GUI_MAP("Helix", "/Game/Maps/Helix", MapSelectability::Playable | MapSelectability::Race);
	ADD_GUI_MAP("Highwind", "/Game/Maps/Highwind", MapSelectability::Playable | MapSelectability::Race);
	ADD_GUI_MAP("Impact", "/Game/Maps/Impact", MapSelectability::Playable | MapSelectability::Race);
	ADD_GUI_MAP("Karman Station", "/Game/Maps/Karman_Station", MapSelectability::Playable | MapSelectability::Race);
	ADD_GUI_MAP("Lavawell", "/Game/Maps/Lavawell", MapSelectability::Playable | MapSelectability::Race);
	ADD_GUI_MAP("Lobby", "/Game/Maps/Lobby", MapSelectability::Misc);
	ADD_GUI_MAP("Maya (Blockout)", "/Game/Maps/Maya_Blockout", MapSelectability::Playable);
	ADD_GUI_MAP("Meridian (Blockout)", "/Game/Maps/Blockouts/Meridian_Blockout_Playtest", MapSelectability::Playable);
	ADD_GUI_MAP("Neon Sector (Blockout)", "/Game/Maps/Blockouts/Neon_Sector_Blockout_Playtest", MapSelectability::Playable);
	ADD_GUI_MAP("Noboru Temple (Blockout)", "/Game/Maps/Noboru_Temple_Blockout", MapSelectability::Playable);
	ADD_GUI_MAP("Oasis", "/Game/Maps/Oasis", MapSelectability::Playable | MapSelectability::Race);
	ADD_GUI_MAP("Olympus", "/Game/Maps/Olympus", MapSelectability::Playable | MapSelectability::Race);
	ADD_GUI_MAP("Ozone (Blockout)", "/Game/Maps/Blockouts/Ozone_Blockout_Playtest", MapSelectability::Playable);
	ADD_GUI_MAP("Pantheon", "/Game/Maps/Pantheon", MapSelectability::Playable | MapSelectability::Race);
	ADD_GUI_MAP("Practice Range", "/Game/Maps/PracticeRange", MapSelectability::Misc);
	ADD_GUI_MAP("Sanctum (Blockout)", "/Game/Maps/Blockouts/Sanctum_Blockout_Playtest", MapSelectability::Playable);
	ADD_GUI_MAP("Club Silo", "/Game/Maps/Silo", MapSelectability::Playable | MapSelectability::Race);
	ADD_GUI_MAP("Simulation Alpha", "/Game/Maps/Simulation_Alpha", MapSelectability::Playable);
	ADD_GUI_MAP("Simulation Bravo", "/Game/Maps/Simulation_Bravo", MapSelectability::Playable);
	ADD_GUI_MAP("Simulation Charlie", "/Game/Maps/Simulation_Charlie", MapSelectability::Playable);
	ADD_GUI_MAP("Simulation Delta", "/Game/Maps/Simulation_Delta", MapSelectability::Playable);
	ADD_GUI_MAP("Simulation Echo", "/Game/Maps/Simulation_Echo", MapSelectability::Playable);
	ADD_GUI_MAP("Simulation Foxtrot", "/Game/Maps/Simulation_Foxtrot", MapSelectability::Playable);
	ADD_GUI_MAP("Simulation Golf", "/Game/Maps/Simulation_Golf", MapSelectability::Playable);
	ADD_GUI_MAP("Simulation Hotel", "/Game/Maps/Simulation_Hotel", MapSelectability::Playable);
	ADD_GUI_MAP("Simulation India", "/Game/Maps/Simulation_India", MapSelectability::Playable);
	ADD_GUI_MAP("Simulation Juliet", "/Game/Maps/Simulation_Juliet", MapSelectability::Playable);
	ADD_GUI_MAP("Stadium", "/Game/Maps/Stadium", MapSelectability::Playable | MapSelectability::Race);
	ADD_GUI_MAP("Titan (Blockout)", "/Game/Maps/Titan_Blockout_WIP", MapSelectability::Playable);
	ADD_GUI_MAP("Toxic (Blockout)", "/Game/Maps/Toxic_Blockout_Wip", MapSelectability::Playable);
	ADD_GUI_MAP("Tutorial", "/Game/Maps/Tutorial", MapSelectability::Misc);
	ADD_GUI_MAP("Vessel (Blockout)", "/Game/Maps/Vessel_Blockout_WIP", MapSelectability::Playable);
	ADD_GUI_MAP("Vintage (Blockout)", "/Game/Maps/Vintage_Blockout_WIP", MapSelectability::Playable);

#undef ADD_GUI_MAP
}
