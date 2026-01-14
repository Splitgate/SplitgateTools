#include "Globals.h"
#include "TabbedWindowBase.h"

void TabbedWindowBase::Render()
{
    ImGui::SetWindowSize(ImVec2(620, 400), ImGuiCond_FirstUseEver);

    if (WindowFlags & ImGuiWindowFlags_NoResize)
        ImGui::SetWindowSize(ImVec2(620, 400));

    ImGui::BeginGroup();
    {
        ImGui::BeginChild("TabWindow", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1);
            ImGui::BeginChild("TabList", { 220, 0, }, ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoSavedSettings);
            {
                if (ShouldDisplayTabList())
                {
                    for (int i = 0; i < Tabs.size(); i++)
                    {
                        auto& Tab = Tabs[i];
                        ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
                        {
                            if (ImGui::Selectable(Tab->Name.c_str(), (SelectedTabIndex == i), 0, {0, 20}))
                            {
                                SetTab(i); // Render this tab
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
            ImGui::BeginChild("TabData", { 0, 0, }, ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
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

        RenderExtraButtons();
    }
    ImGui::EndGroup();
}

void TabbedWindowBase::OnClose()
{
    Tabs.clear();
}

bool TabbedWindowBase::ShouldDisplayTabList() const
{
    return true;
}

void TabbedWindowBase::SetTab(int TabIndex)
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
