#include "ForgeUtility.h"
#include "Strings/Strings.h"

bool ForgeUtility::ParseMap(nlohmann::json ForgeJson, FForgeMap& OutMap)
{
	nlohmann::json ForgeMap = uppercase_keys(ForgeJson);

	if (ForgeMap.contains("TIMESTAMP"))
		FDateTime::Parse(ForgeMap.at("TIMESTAMP"), OutMap.Timestamp);

	if (ForgeMap.contains("SHARINGCODE"))
	{
		std::string ShareCode = ForgeMap.at("SHARINGCODE");
		OutMap.SharingCode = Strings::StringToWide(ShareCode).c_str();
	}

	if (ForgeMap.contains("NAME"))
	{
		std::string MapName = ForgeMap.at("NAME");
		OutMap.MapName = Strings::StringToWide(MapName).c_str();
	}

	// FText FromString -- L"Failed to read file %s" -> Scan E8 skip 1 ?FromString@FText@@SA?AV1@$$QEAVFString@@@Z

	//if (ForgeMap.contains("displayName"))
	//{
	//	std::string DisplayName = ForgeMap["displayName"];
	//	OutMap.DisplayName.GetStringRef() = StringToWide(DisplayName).c_str();
	//}

	if (ForgeMap.contains("INTERACTABLESSAVEDATA"))
	{
		std::vector<json> Interactables = ForgeMap.at("INTERACTABLESSAVEDATA");
		for (auto& Interactable : Interactables)
		{
			Interactable = uppercase_keys(Interactable);

			FForgeInteractableSaveData NewSaveData{};
			if (Interactable.contains("ID"))
				NewSaveData.ID = Interactable.at("ID");

			if (Interactable.contains("TRANSFORM"))
				NewSaveData.Transform.ParseJson(uppercase_keys(Interactable.at("TRANSFORM")));

			OutMap.InteractablesSaveData.Add(NewSaveData);
		}
	}

	return true;
}
