#pragma once
#include <memory>

#include "UI/UIElement.h"
#include <vector>

#include "UI/Settings/SettingsTab.h"

class ToolSettings : public UIElement
{
public:

	ToolSettings();
	virtual ~ToolSettings() override;

	std::vector<std::unique_ptr<SettingsTab>> Tabs;
	int SelectedTabIndex = 0;

	virtual void Render() override;
};
