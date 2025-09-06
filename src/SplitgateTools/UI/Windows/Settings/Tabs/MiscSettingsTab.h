#pragma once
#include "SettingsTabBase.h"

class MiscSettingsTab : public SettingsTabBase
{
public:
	MiscSettingsTab() : SettingsTabBase("Misc"){}
	virtual void RenderContent() override;
};
