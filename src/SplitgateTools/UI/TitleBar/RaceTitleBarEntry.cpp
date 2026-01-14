#include "RaceTitleBarEntry.h"

#include "Renderer.h"
#include "UI/WindowManager.h"
#include "UI/Windows/Race/RaceLeaderboardWindow.h"

void RaceTitleBarEntry::Render()
{
	if (ImGui::MenuItem("Leaderboards"))
		WindowManager::Add<RaceLeaderboardWindow>();
}
