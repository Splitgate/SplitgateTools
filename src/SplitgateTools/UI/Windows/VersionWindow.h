#pragma once
#include "UIWindowBase.h"

class VersionWindow : public UIWindowBase
{
public:

	VersionWindow() : UIWindowBase("Game Information", false, true,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking)
	{
	}

	virtual void Render() override;
};
