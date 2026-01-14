#pragma once
#include "TabbedWindowBase.h"

class ForgeMyMapsTab : public TabEntryBase
{
public:
	ForgeMyMapsTab() : TabEntryBase() {}
	ForgeMyMapsTab(std::string InName) : TabEntryBase(InName) {}

	virtual void RenderContent() override {};
};
