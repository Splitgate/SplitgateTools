#include "SystemVersionWindow.h"
#include "Globals.h"
#include <Games/BaseGame.h>

void SystemVersionWindow::Render()
{
    ImGui::SetWindowSize(ImVec2(240, 140));

    ImVec2 Center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetWindowPos(Center, ImGuiCond_Appearing);

    // Title Text
    {
        ImGui::PushFont(NULL, 26.f);

        const char* TitleText = "Splitgate Tools";

        float WindowWidth = ImGui::GetWindowSize().x;
        float TextWidth = ImGui::CalcTextSize(TitleText).x;
        ImGui::SetCursorPosX((WindowWidth - TextWidth) / 2);

        ImGui::Text(TitleText);
        ImGui::PopFont();

        ImGui::TextWrapped("by Adam & Ruby Rain");
    }

    ImGui::Separator();
    ImGui::Spacing();

    ImGui::TextWrapped("Built %s", COMPILE_DATE);

    std::string MSCVer = std::to_string(_MSC_FULL_VER);
    ImGui::TextWrapped("Visual C++ %s.%s.%s",
        MSCVer.substr(0, 2).c_str(),
        MSCVer.substr(2, 2).c_str(),
        MSCVer.substr(4).c_str());

    ImGui::TextWrapped("Avg %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
