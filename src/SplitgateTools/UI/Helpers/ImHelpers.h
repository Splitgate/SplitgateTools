#pragma once

#include "imgui.h"
#include "imgui_internal.h"

namespace ImGui
{
    static void HelpMarker(const char* desc)
    {
        ImGui::TextDisabled("(?)");
        if (ImGui::BeginItemTooltip())
        {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

	bool InputIntClamped(const char* label, int* v, int min = 0, int max = 0, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0);
}