#include "SettingsWindow.h"
#include "UI/Windows/Settings/Tabs/SettingsTabBase.h"

#include "Games/BaseGame.h"

#include "Globals.h"

void SettingsWindow::Render()
{
    ImGui::SetWindowSize(ImVec2(580, 400));

    ImGui::BeginGroup();
    {
        ImGui::BeginChild("Settings Content"); // ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); Old bottom button padding
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1);
            ImGui::BeginChild("SettingList", { 150, 0, }, ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoSavedSettings);
            {
                for (int i = 0; i < Tabs.size(); i++)
                {
                    auto& Tab = Tabs[i];
                    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
                    {
                        if (ImGui::Selectable(Tab->Name, (SelectedTabIndex == i), 0, { 0, 20 }))
                        {
                            SelectedTabIndex = i; // Render my settings content
                        }
                    }
                    ImGui::PopStyleVar();

                    ImGui::Dummy({ 0, 2 });
                }
                ImGui::EndChild();
            }
            ImGui::PopStyleVar();

            ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1);
            ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
            ImGui::BeginChild("SettingsContent", { 0, 0, }, ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoSavedSettings);
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

        //if (ImGui::Button("Reload from Save")){}
        //ImGui::SameLine();
        //if (ImGui::Button("Apply")){}
    }
    ImGui::EndGroup();
}

void SettingsWindow::OnOpen()
{
    if (Game)
    {
        Game->PopulateSettingsTabs(Tabs);
    }
}

void SettingsWindow::OnClose()
{
    Tabs.clear();
}
