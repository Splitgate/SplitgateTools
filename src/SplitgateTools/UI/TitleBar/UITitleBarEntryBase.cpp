#include "UITitleBarEntryBase.h"
#include "Renderer.h"

UITitleBarEntryBase::UITitleBarEntryBase(const char* InName, ImGuiWindowFlags InFlags)
	: Name(InName), Flags(InFlags)
{
}

UITitleBarEntryBase::~UITitleBarEntryBase()
{
}

void UITitleBarEntryBase::Tick()
{

}

void UITitleBarEntryBase::Render()
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
