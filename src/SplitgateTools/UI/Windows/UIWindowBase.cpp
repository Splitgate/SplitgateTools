#include "UIWindowBase.h"
#include "Renderer.h"

UIWindowBase::UIWindowBase(const char* InName, bool InbCanHaveMultiple, bool InbIsClosable, ImGuiWindowFlags InWindowFlags)
	: Name(InName),
	bCanHaveMultiple(InbCanHaveMultiple),
	bIsClosable(InbIsClosable),
	WindowFlags(InWindowFlags)
{

}

void UIWindowBase::Render()
{
	ImGui::Text("Hello! Override me please :)");
}
