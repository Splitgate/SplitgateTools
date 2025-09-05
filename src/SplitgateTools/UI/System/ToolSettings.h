#pragma once
#include "UI/UIElement.h"
#include <vector>

class ToolSettings : public UIElement
{
public:

	ToolSettings() : UIElement("Settings", false, true, true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking)
	{

	}

	class SettingsEntry* CurrentlyRenderingTab;
	static inline std::vector<class SettingsEntry*> SettingsEntries;

	virtual void Render() override;
	virtual void OnOpen() override;
	virtual void OnClose() override;
};
