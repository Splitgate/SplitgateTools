#pragma once
#include "UI/UIElement.h"
#include <vector>
#include <map>

class ToolSettings : public UIElement
{
public:

	ToolSettings() : UIElement("Settings", false, true, true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking)
	{

	}

	std::map<const char*, bool> TabOpenMap;
	class SettingsEntry* CurrentlyRenderingTab;

	static inline std::vector<class SettingsEntry*> SettingsEntries;

	virtual void Render() override;
	virtual void OnOpen() override;
	virtual void OnClose() override;

	void DeselectAllSettings(const char* TabToExclude);
};
