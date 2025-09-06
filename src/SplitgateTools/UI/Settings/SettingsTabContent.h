#pragma once

class SettingsTabContent
{
public:

	SettingsTabContent(const char* InEntryName);
	virtual ~SettingsTabContent();

	// Tab Name
	const char* EntryName;

	bool DrawInt(const char* DisplayName, int* SettingToUpdate, int Min = 0, int Max = 0, int Flags = 0);

	// Tab Content
	virtual void RenderContent();
};