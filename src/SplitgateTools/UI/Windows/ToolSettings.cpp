#include "ToolSettings.h"
#include "UI/Settings/SettingsTab.h"

#include "Games/BaseGame.h"

#include "Globals.h"

ToolSettings::ToolSettings()
    : UIWindowBase("Settings", false, true,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking)
{
    if (Game)
    {
        Game->PopulateSettingsTabs(Tabs);
    }
}

ToolSettings::~ToolSettings()
{
    Tabs.clear();
}

void ToolSettings::Render()
{
    ImGui::SetWindowSize(ImVec2(580, 380));

    // if (CurrentlyRenderingTab)
    // {
    //     ImGui::Text(CurrentlyRenderingTab->EntryName);
    // }

    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1);
    ImGui::BeginChild("SettingList", { 150, 0, }, ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoSavedSettings);
    {
        for (int i = 0; i < Tabs.size(); i++)
        {
            auto& Tab = Tabs[i];
            ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
            {
                if (ImGui::Selectable(Tab->Name, (SelectedTabIndex == i), 0, {0, 20}))
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
