#pragma once

#include "Games/BaseGame.h"

class Splitgate : public BaseGame
{
public:

	Splitgate();

	virtual void Init_PreEngine() override;
	virtual void Init_PostEngine() override;

	virtual void PopulateTitleBarEntries(std::vector<std::unique_ptr<UITitleBarEntry>>& Entries) override;
	virtual void PopulateSettingsTabs(std::vector<std::unique_ptr<SettingsTab>>& Tabs) override;
};
