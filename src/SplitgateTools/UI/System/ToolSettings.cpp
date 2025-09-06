#include "ToolSettings.h"
#include "UI/Settings/SettingsTabContent.h"

#include "Games/BaseGame.h"

#include "Globals.h"

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
        for (auto& SettingEntry : SettingsEntries)
        {
            if (!CurrentlyRenderingTab)
                continue;

            ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
            {
                if (ImGui::Selectable(SettingEntry->EntryName, (CurrentlyRenderingTab->EntryName == SettingEntry->EntryName), 0, {0, 20}))
                {
                    CurrentlyRenderingTab = SettingEntry; // Render my settings content
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
        if (CurrentlyRenderingTab)
        {
            CurrentlyRenderingTab->RenderContent();
        }

        ImGui::EndChild();
    }
    ImGui::PopStyleVar();
}

void ToolSettings::OnOpen()
{
    if (Game)
    {
        Game->GatherSettingsEntries();
    }

    for (auto& SettingEntry : SettingsEntries)
    {
        if (CurrentlyRenderingTab == nullptr)
        {
            CurrentlyRenderingTab = SettingEntry;
        }
    }
}

void ToolSettings::OnClose()
{
    SettingsEntries.clear();

    CurrentlyRenderingTab = nullptr;
}
