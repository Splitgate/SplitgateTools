#pragma once
#include "UI/UIElement.h"

class SystemVersionWindow : public UIElement
{
public:

	SystemVersionWindow() : UIElement("Tool Information", false, true, true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking)
	{
	}

	virtual void Render() override;
};
