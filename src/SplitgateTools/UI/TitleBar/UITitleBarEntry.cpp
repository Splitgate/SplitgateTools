#include "UITitleBarEntry.h"
#include "Renderer.h"

UITitleBarEntry::UITitleBarEntry(const char* InName, ImGuiWindowFlags InFlags)
	: Name(InName), Flags(InFlags)
{
}

UITitleBarEntry::~UITitleBarEntry()
{
}

void UITitleBarEntry::Tick()
{

}

void UITitleBarEntry::Render()
{
	if (ImGui::MenuItem("Generic")) {}

	if (ImGui::BeginMenu("Generic Entry"))
	{
		if (ImGui::MenuItem("Generic Two")) {}

		if (ImGui::BeginMenu("More.."))
		{
			ImGui::MenuItem("Entry");
			if (ImGui::BeginMenu("Sub Entry"))
			{
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}
}
