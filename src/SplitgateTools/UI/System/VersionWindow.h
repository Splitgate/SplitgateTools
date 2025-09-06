#pragma once
#include "UI/UIElement.h"

class VersionWindow : public UIElement
{
public:

	VersionWindow() : UIElement("Game Information", false, true,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking)
	{
	}

	virtual void Render() override;
};
