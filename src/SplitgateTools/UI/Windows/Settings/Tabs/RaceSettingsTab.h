#pragma once
#include "SettingsTabBase.h"

class RaceSettingsTab : public SettingsTabBase
{
public:
	RaceSettingsTab() : SettingsTabBase("Race"){}
	virtual void RenderContent() override;
};
