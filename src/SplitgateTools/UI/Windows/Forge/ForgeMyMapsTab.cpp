#include "ForgeMyMapsTab.h"

#include "ForgeUtility.h"

#include "Globals.h"
#include "Globals/FForgeMap.h"

#include "nlohmann/json.hpp"
#include "Strings/Strings.h"

#include <fstream>

void ForgeMyMapsTab::RenderContent()
{
	if (ImGui::Button("Test File Picker"))
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
			if (FileExtension == "JSON")
			{
				std::ifstream JsonFile(CurrentFilePath);
				ForgeUtility::ParseMap(nlohmann::json::parse(JsonFile));
			}
			else if (FileExtension == "FORGE")
			{

			}
		}
	}
}
