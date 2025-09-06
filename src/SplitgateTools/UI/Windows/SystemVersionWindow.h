#pragma once
#include "UIWindowBase.h"

class SystemVersionWindow : public UIWindowBase
{
public:

	SystemVersionWindow() : UIWindowBase("Tool Information", false, true,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking)
	{

	}

	virtual void Render() override;
};
