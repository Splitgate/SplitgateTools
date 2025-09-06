#pragma once
#include "UIWindowBase.h"

class HydraWindow : public UIWindowBase
{
public:
	HydraWindow() : UIWindowBase("hydra", true, true, ImGuiWindowFlags_NoResize){}
	virtual void Render() override;
	virtual void OnClose() override;
};
