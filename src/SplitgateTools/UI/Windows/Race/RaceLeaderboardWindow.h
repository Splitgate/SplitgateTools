#pragma once
#include <memory>
#include <vector>

#include "../TabbedWindowBase.h"
#include "LeaderboardTab.h"

class RaceLeaderboardWindow final : public TabbedWindowBase
{
public:

	RaceLeaderboardWindow() : TabbedWindowBase("Race Leaderboards", false, true,
		ImGuiWindowFlags_NoDocking)
	{
	}

	// For checking if http data is loaded
	ELoadState LoadState;

	virtual void RenderExtraButtons() override;
	virtual void OnOpen() override;
};
