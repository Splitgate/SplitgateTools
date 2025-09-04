#include "SettingsEntry.h"
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

void SettingsEntry::RenderContent()
{
	ImGui::Text("Implement :3");
}
