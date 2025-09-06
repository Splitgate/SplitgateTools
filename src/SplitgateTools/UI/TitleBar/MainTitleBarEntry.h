#pragma once
#include "UI/TitleBar/UITitleBarEntryBase.h"

class MainTitleBarEntry : public UITitleBarEntryBase
{
public:
	MainTitleBarEntry() : UITitleBarEntryBase("[SplitgateTools]"){}

protected:
	virtual void Render() override;
};
