#pragma once
#include "UIWindowBase.h"

class ToolInfoWindow : public UIWindowBase
{
public:

	ToolInfoWindow() : UIWindowBase("Tool Information", false, true,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking)
	{

	}

	virtual void Render() override;
};
