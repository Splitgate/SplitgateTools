#pragma once
#include <memory>

#include "UI/UIElement.h"
#include <vector>

#include "UI/Settings/SettingsTab.h"

class ToolSettings : public UIElement
{
public:

	ToolSettings() : UIElement("Settings", false, true, true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking)
	{

	}

	std::vector<std::unique_ptr<SettingsTab>> Tabs;
	int SelectedTabIndex = 0;

	virtual void Render() override;
	virtual void OnOpen() override;
	virtual void OnClose() override;
};
