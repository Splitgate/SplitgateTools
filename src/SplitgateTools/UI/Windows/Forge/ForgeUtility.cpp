#include "ForgeUtility.h"
#include "Strings/Strings.h"

#include <fstream>

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

bool ForgeUtility::LoadFromFile(FForgeMap& OutMap)
{
	char SelectedFile[MAX_PATH];

	OPENFILENAMEA OpenFileN{};
	ZeroMemory(&SelectedFile, sizeof(SelectedFile));

	OpenFileN.lStructSize = sizeof(OpenFileN);
	OpenFileN.hwndOwner = NULL;
	OpenFileN.lpstrFile = SelectedFile;
	OpenFileN.lpstrFile[0] = '\0';
	OpenFileN.nMaxFile = sizeof(SelectedFile);
	OpenFileN.lpstrFilter = "Forge Map (.json)\0*.JSON\0Forge Map (.forge)\0*.FORGE\0";
	OpenFileN.nFilterIndex = 0;
	OpenFileN.lpstrFileTitle = NULL;
	OpenFileN.nMaxFileTitle = NULL;
	OpenFileN.lpstrInitialDir = NULL;
	OpenFileN.lpstrTitle = "Choose Forge Map";

	if (GetOpenFileNameA(&OpenFileN))
	{
		// Example - C:\Users\Adam\Downloads\3S72-SV3B-B4E8-7AD0.json
		std::string CurrentFilePath = SelectedFile;
		std::string FileExtension = "JSON";

		int LastSlash = CurrentFilePath.find_last_of('\\') + 1;
		CurrentFilePath = CurrentFilePath.substr(LastSlash, CurrentFilePath.length() - LastSlash);

		int FileExtensionStart = CurrentFilePath.find_last_of('.') + 1;
		FileExtension = Strings::Uppercase(CurrentFilePath.substr(FileExtensionStart, CurrentFilePath.length() - FileExtensionStart));

		FForgeMap MapToLoad{};
		bool bLoadedMap = false;

		if (FileExtension == "JSON")
		{
			std::ifstream JsonFile(CurrentFilePath);
			bLoadedMap = ForgeUtility::ParseMap(nlohmann::json::parse(JsonFile), MapToLoad);

			OutMap = MapToLoad;
			return bLoadedMap;
		}
		else if (FileExtension == "FORGE")
		{
			// Implement later
			bLoadedMap = false;
		}
	}

	return false;
}
