#pragma once
#include <memory>

#include "UIWindowBase.h"
#include <vector>

#include "UI/Settings/SettingsTab.h"

class SettingsWindow : public UIWindowBase
{
public:

	SettingsWindow() : UIWindowBase("Settings", false, true,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking)
	{
	}

	std::vector<std::unique_ptr<SettingsTab>> Tabs;
	int SelectedTabIndex = 0;

	virtual void Render() override;

	virtual void OnOpen() override;
	virtual void OnClose() override;
};
