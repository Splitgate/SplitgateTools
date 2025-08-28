#include "UITitleBarEntry.h"
#include "Renderer.h"

UITitleBarEntry::UITitleBarEntry(const char* InEntryName, ImGuiWindowFlags InEntryFlags)
	: EntryName(InEntryName),
	EntryFlags(InEntryFlags)
{
	Renderer::TitleBarEntries.push_back(this);
	OnCreate();
}

UITitleBarEntry::~UITitleBarEntry()
{
	auto ElemIndex = std::find(Renderer::TitleBarEntries.begin(), Renderer::TitleBarEntries.end(), this);
	Renderer::TitleBarEntries.erase(ElemIndex);
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

void UITitleBarEntry::OnCreate()
{
}
