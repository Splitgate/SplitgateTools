#pragma once

class SettingsTab
{
public:

	SettingsTab(const char* InName);
	virtual ~SettingsTab();

	const char* Name;

	bool DrawInt(const char* DisplayName, int* SettingToUpdate, int Min = 0, int Max = 0, int Flags = 0);

	virtual void RenderContent();
};