#pragma once
#include <memory>

#include "UIWindowBase.h"
#include <vector>

#include "UI/Settings/SettingsTab.h"

class ToolSettings : public UIWindowBase
{
public:

	ToolSettings();
	virtual ~ToolSettings() override;

	std::vector<std::unique_ptr<SettingsTab>> Tabs;
	int SelectedTabIndex = 0;

	virtual void Render() override;
};
