#pragma once
#include "UI/UIElement.h"
#include <vector>

class ToolSettings : public UIElement
{
public:

	ToolSettings() : UIElement("Settings", false, true, true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking)
	{

	}

	class SettingsTabContent* CurrentlyRenderingTab;
	static inline std::vector<class SettingsTabContent*> SettingsEntries;

	virtual void Render() override;
	virtual void OnOpen() override;
	virtual void OnClose() override;
};
