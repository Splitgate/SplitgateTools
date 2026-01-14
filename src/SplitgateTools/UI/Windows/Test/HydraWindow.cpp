#include "HydraWindow.h"

#include "UI/WindowManager.h"

void HydraWindow::Render()
{
	ImVec2 Size = ImGui::GetMainViewport()->Size;

	auto randFloat = [](float a, float b) {
		return a + (b - a) * (float)rand() / (float)RAND_MAX;
	};

	ImVec2 Pos;
	Pos.x = randFloat(-150.f, Size.x - 300.f);
	Pos.y = randFloat(-25.f, Size.y - 50.f);

	ImGui::SetWindowPos(Pos, ImGuiCond_Once);

	ImGui::Text("Cut off a head, three more will take its place.");
	if (ImGui::Button("Kill"))
		Close();
}

void HydraWindow::OnClose()
{
	WindowManager::Add<HydraWindow>();
	WindowManager::Add<HydraWindow>();
	WindowManager::Add<HydraWindow>();
}
