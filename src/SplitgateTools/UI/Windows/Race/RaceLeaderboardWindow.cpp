#include "HttpSystem/HttpSystem.h"

#include "RaceLeaderboardWindow.h"
#include "Globals.h"

#include "UI/TitleBar/MapsTitleBarEntry.h"
#include "ImGuiNotify/ImGuiNotify.hpp"
#include "TabbedWindowBase.h"

void RaceLeaderboardWindow::Render()
{
    if (WindowFlags & ImGuiWindowFlags_NoResize)
        ImGui::SetWindowSize(ImVec2(620, 400));

    ImGui::BeginGroup();
    {
        ImGui::BeginChild("Leaderboard", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1);
            ImGui::BeginChild("MapList", { 220, 0, }, ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoSavedSettings);
            {
                if (LoadState == ELoadState::Loaded)
                {
                    for (int i = 0; i < Tabs.size(); i++)
                    {
                        auto& Tab = Tabs[i];
                        ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
                        {
                            if (ImGui::Selectable(Tab->Name.c_str(), (SelectedTabIndex == i), 0, { 0, 20 }))
                            {
                                SetTab(i); // Render this leaderboard
                            }
                        }
                        ImGui::PopStyleVar();

                        ImGui::Dummy({ 0, 2 });
                    }
                }

                ImGui::EndChild();
            }
            ImGui::PopStyleVar();

            ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1);
            ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
            ImGui::BeginChild("LeaderboardData", { 0, 0, }, ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            {
                if (SelectedTabIndex >= 0 && SelectedTabIndex < Tabs.size())
                {
                    Tabs[SelectedTabIndex]->RenderContent();
                }

                ImGui::EndChild();
            }
            ImGui::PopStyleVar();
        }
        ImGui::EndChild();

        if (ImGui::Button("Refresh"))
        {
            if (SelectedTabIndex >= 0 && SelectedTabIndex < Tabs.size())
            {
                Tabs[SelectedTabIndex]->FetchLeaderboard();
            }
        }
    }
    ImGui::EndGroup();
}

void RaceLeaderboardWindow::OnOpen()
{
    httplib::Request MapListReq = httplib::Request();
    MapListReq.path = "/api/get-maps";
    MapListReq.method = "GET";

    HttpJob MapListJob = HttpJob(&HttpSystem::RaceBase, MapListReq, [&](httplib::Response Resp, httplib::Error Err)
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

void RaceLeaderboardWindow::OnClose()
{
    Tabs.clear();
}

void RaceLeaderboardWindow::SetTab(int TabIndex)
{
    SelectedTabIndex = TabIndex;

    auto& Tab = Tabs[SelectedTabIndex];
    if (Tab)
    {
        Tab->OnSelected();
    }
    else
    {
        // Retry until tab is ready
        SetTab(TabIndex);
    }
}
