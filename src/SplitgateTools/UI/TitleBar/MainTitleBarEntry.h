#pragma once
#include "UI/TitleBar/UITitleBarEntry.h"

class MainTitleBarEntry : public UITitleBarEntry
{
public:
	MainTitleBarEntry() : UITitleBarEntry("[SplitgateTools]"){}

protected:
	virtual void Render() override;
};
