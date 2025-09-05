#include "SettingsEntry.h"

#include "Settings.h"
#include "PortalWars/UPortalWarsGameEngine.h"
#include "UI/System/ToolSettings.h"

SettingsEntry::SettingsEntry(const char* InEntryName)
	: EntryName(InEntryName)
{
	ToolSettings::SettingsEntries.push_back(this);
}

SettingsEntry::~SettingsEntry()
{
	auto ElemIndex = std::find(ToolSettings::SettingsEntries.begin(), ToolSettings::SettingsEntries.end(), this);
	ToolSettings::SettingsEntries.erase(ElemIndex);
}

bool SettingsEntry::DrawInt(const char* DisplayName, int* SettingToUpdate, int Min, int Max, int Flags)
{
	char Label[256];
	sprintf(Label, "###%s", DisplayName);

	ImGui::AlignTextToFramePadding();

	ImGui::Text(DisplayName);
	ImGui::SameLine(0, 15);

	bool bReturn = ImGui::DragInt(Label, SettingToUpdate, 1.0f, Min, Max, "%d", Flags);
	if (bReturn)
	{
		GSettings.Save();
	}

	return bReturn;
}

void SettingsEntry::RenderContent()
{
	ImGui::Text("TODO: completely rewrite this class, this is all temporary");

	DrawInt("Countdown Length", &GSettings.Race.CountdownLength, 0, INT_MAX);
	DrawInt("Test", &GSettings.Race.CountdownLength, 0, INT_MAX);

	// TODO: maybe make some imgui funcs that call original and then GSettings.Save() if they return true 
	//if (ImGui::DragInt("Countdown Length", &GSettings.Race.CountdownLength, 0.05f))
	//	GSettings.Save();
	//
	//if (ImGui::Checkbox("Uncap FPS in lobby", &GSettings.Misc.bEnableLobbyFPSPatch))
	//{
	//	UPortalWarsGameEngine::UpdateLobbyFPSPatch();
	//	GSettings.Save();
	//}

	// TODO: THIS DOESNT HANDLE UpdateLobbyFPSPatch
	// we could just add it but that doesn't scale very well and is quite bug prone
	// maybe we make a function called something like Settings::Apply, where patches and hooks are applied
	// we'd have to do it at the very end of Init_PostEngine to make sure all offsets have been found
	//if (ImGui::Button("Reload settings from file"))
	//	GSettings.Load();
}
