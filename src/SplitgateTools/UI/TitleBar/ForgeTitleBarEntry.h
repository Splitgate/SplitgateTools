#pragma once
#include <string>
#include <vector>

#include "UI/TitleBar/UITitleBarEntryBase.h"

class ForgeTitleBarEntry : public UITitleBarEntryBase
{
public:

	ForgeTitleBarEntry() : UITitleBarEntryBase("Forge") {}

protected:

	virtual void Render() override;
};