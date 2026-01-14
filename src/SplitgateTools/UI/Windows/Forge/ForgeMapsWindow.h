#pragma once

#include "TabbedWindowBase.h"
#include "ForgeMyMapsTab.h"
#include "ForgeAllMapsTab.h"

class ForgeMapsWindow final : public TabbedWindowBase
{
public:

	ForgeMapsWindow() : TabbedWindowBase("The Forge", false, true,
		ImGuiWindowFlags_NoDocking)
	{
		Tabs.emplace_back(std::make_unique<ForgeAllMapsTab>("All Maps"));
		Tabs.emplace_back(std::make_unique<ForgeMyMapsTab>("Your Maps"));
	}
};
