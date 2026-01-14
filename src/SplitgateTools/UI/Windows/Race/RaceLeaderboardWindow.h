#pragma once
#include <memory>
#include <vector>

#include "../UIWindowBase.h"
#include "LeaderboardTab.h"

class RaceLeaderboardWindow : public UIWindowBase
{
public:

	RaceLeaderboardWindow() : UIWindowBase("Race Leaderboards", false, true,
		ImGuiWindowFlags_NoDocking)
	{
	}

	std::vector<std::unique_ptr<LeaderboardTab>> Tabs;
	int SelectedTabIndex = 0;

	// For checking if http data is loaded
	ELoadState LoadState;

	virtual void Render() override;

	virtual void OnOpen() override;
	virtual void OnClose() override;

private:

	void SetTab(int TabIndex);
};
