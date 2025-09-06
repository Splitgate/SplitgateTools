#pragma once
#include <memory>
#include <vector>

#include "../UIWindowBase.h"
#include "Tabs/SettingsTabBase.h"

class SettingsWindow : public UIWindowBase
{
public:

	SettingsWindow() : UIWindowBase("Settings", false, true,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking)
	{
	}

	std::vector<std::unique_ptr<SettingsTabBase>> Tabs;
	int SelectedTabIndex = 0;

	virtual void Render() override;

	virtual void OnOpen() override;
	virtual void OnClose() override;
};
