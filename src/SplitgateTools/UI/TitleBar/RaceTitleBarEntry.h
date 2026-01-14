#pragma once
#include <string>
#include <vector>

#include "UI/TitleBar/UITitleBarEntryBase.h"

class RaceTitleBarEntry : public UITitleBarEntryBase
{
public:

	RaceTitleBarEntry() : UITitleBarEntryBase("Race") {}

protected:

	virtual void Render() override;
};