#pragma once
#include <memory>
#include <vector>

#include "UIWindowBase.h"

class TabEntryBase
{

};

class TabbedWindowBase : public UIWindowBase
{
public:

	TabbedWindowBase(std::string InName, bool InbCanHaveMultiple = false, bool InbIsClosable = true, ImGuiWindowFlags InWindowFlags = 0) 
		: UIWindowBase("Tabbed Window", false, true,
		ImGuiWindowFlags_NoDocking)
	{
	}

	std::vector<std::unique_ptr<TabEntryBase>> Tabs;
	int SelectedTabIndex = 0;

	virtual void Render() override;

	virtual void OnOpen() override;
	virtual void OnClose() override;

private:

	virtual void SetTab(int TabIndex);
};
