#pragma once
#include "UIWindowBase.h"

class GameInfoWindow : public UIWindowBase
{
public:

	GameInfoWindow() : UIWindowBase("Game Information", false, true,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking)
	{
	}

	virtual void Render() override;
};
