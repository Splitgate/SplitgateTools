#include "HttpSystem/HttpSystem.h"

#include "RaceLeaderboardWindow.h"
#include "Globals.h"

#include "UI/TitleBar/MapsTitleBarEntry.h"
#include "ImGuiNotify/ImGuiNotify.hpp"
#include "TabbedWindowBase.h"

void RaceLeaderboardWindow::RenderExtraButtons()
{
    if (ImGui::Button("Refresh"))
    {
        if (SelectedTabIndex >= 0 && SelectedTabIndex < Tabs.size())
        {
            if (LeaderboardTab* Tab = GetTab<LeaderboardTab>(SelectedTabIndex))
                Tab->FetchLeaderboard();
        }
    }
}

void RaceLeaderboardWindow::OnOpen()
{
    httplib::Request MapListReq = httplib::Request();
    MapListReq.path = "/api/get-maps";
    MapListReq.method = "GET";

    HttpSystem::SendRequest(RACEBASE_URL, MapListReq, [&](httplib::Response Resp, httplib::Error Err)
    {
        if (Err == httplib::Error::Success)
        {
            json JMapList = json::parse(Resp.body);
            std::vector<std::string> MapList = JMapList.get<std::vector<std::string>>();

            for (auto& Map : MapList)
            {
                Tabs.emplace_back(std::make_unique<LeaderboardTab>(Map));
            }

            LoadState = ELoadState::Loaded;
            SetTab(0);
        }
        else
        {
            ImGui::InsertNotification({
                ImGuiToastType::Error, 5000, "Failed to receive map list %s - %s",
                httplib::to_string(Err).c_str(), Resp.body.c_str()
                });
        }
    });
}