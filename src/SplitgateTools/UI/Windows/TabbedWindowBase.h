#pragma once
#include <memory>
#include <vector>

#include "UIWindowBase.h"

class TabEntryBase
{
public:
	TabEntryBase() : Name("None") {}
	TabEntryBase(std::string InName) : Name(InName) {}

	std::string Name;

	virtual void RenderContent() {};
	virtual void OnSelected() {};
};

class TabbedWindowBase : public UIWindowBase
{
public:

	TabbedWindowBase(std::string InName, bool InbCanHaveMultiple = false, bool InbIsClosable = true, ImGuiWindowFlags InWindowFlags = 0) 
		: UIWindowBase(InName, InbCanHaveMultiple, InbIsClosable,
			InWindowFlags | ImGuiWindowFlags_NoDocking)
	{
	}

	std::vector<std::unique_ptr<TabEntryBase>> Tabs;
	int SelectedTabIndex = 0;

	template<typename TabType = TabEntryBase>
	TabType* GetTab(int Index = 0)
	{
		if (Index >= 0 && Index < Tabs.size())
		{
			return dynamic_cast<TabType*>(Tabs[Index].get());
		}

		return nullptr;
	}

	virtual void Render() override;
	virtual void RenderExtraButtons() {};

	virtual void OnClose() override;

protected:

	virtual bool ShouldDisplayTabList() const;
	virtual void SetTab(int TabIndex);
};