#include "ToolSettings.h"
#include "SettingsEntry.h"

#include "Games/BaseGame.h"

#include "Globals.h"

void ToolSettings::Render()
{
    ImGui::SetWindowSize(ImVec2(540, 380));

    ImGui::Text("[PUT TEXT HERE]");

    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1);
    ImGui::BeginChild("SettingList", { 150, 0, }, ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoSavedSettings);
    {
        for (auto& SettingEntry : SettingsEntries)
        {
            if (!TabOpenMap.contains(SettingEntry->EntryName))
                continue;

            if (ImGui::Selectable(SettingEntry->EntryName, &TabOpenMap[SettingEntry->EntryName]))
            {
                CurrentlyRenderingTab = SettingEntry; // Render my settings content
            }
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
    SettingsEntries.clear();
    TabOpenMap.clear();

    if (Game)
    {
        UE_LOG(LogImGui, Warning, "Major Gyatt Game");

        Game->GatherSettingsEntries();
    }

    for (auto& SettingEntry : SettingsEntries)
    {
        UE_LOG(LogImGui, Warning, "ADDING A SETTING");

        TabOpenMap.emplace(SettingEntry->EntryName, false);
    }
}
