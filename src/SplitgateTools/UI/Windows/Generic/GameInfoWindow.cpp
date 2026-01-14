#include "GameInfoWindow.h"
#include "Renderer.h"

#include <string>

#include "Engine/Core/Misc/FNetworkVersion.h"
#include "Engine/Core/Misc/FEngineVersion.h"
#include "Engine/Core/Misc/FApp.h"

void GameInfoWindow::Render()
{
    ImGui::SetWindowSize(ImVec2(240, 360));

    ImVec2 Center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetWindowPos(Center, ImGuiCond_Appearing);

    ImGui::PushFont(nullptr, 26.f);
    {
        std::string GameString = std::format("Splitgate {}", FNetworkVersion::GetProjectVersion());

        float WindowWidth = ImGui::GetWindowSize().x;
        float TextWidth = ImGui::CalcTextSize(GameString.c_str()).x;
        ImGui::SetCursorPosX((WindowWidth - TextWidth) / 2);

        ImGui::TextWrapped(GameString.c_str());
    }
    ImGui::PopFont();

    ImGui::Separator();

    ImGui::TextWrapped(FApp::GetBuildVersion().c_str());

    FEngineVersion* Current = FEngineVersion::Current();
    ImGui::TextWrapped("Unreal %d.%d.%d.%d", Current->Major, Current->Minor, Current->Patch, Current->Changelist);

    ImGui::SeparatorText("Renderer");
    ImGui::TextWrapped(FApp::RendererName.c_str());
}
