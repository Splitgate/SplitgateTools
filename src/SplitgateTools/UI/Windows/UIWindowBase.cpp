#include "UIWindowBase.h"
#include "Renderer.h"

UIWindowBase::UIWindowBase(const char* InName, bool InbCanHaveMultiple, bool InbIsClosable, ImGuiWindowFlags InWindowFlags)
	: Name(InName),
	bCanHaveMultiple(InbCanHaveMultiple),
	bIsClosable(InbIsClosable),
	WindowFlags(InWindowFlags)
{
	// TODO: rewrite, move to renderer
	
	bool bFoundWindow = false;

	int FoundCounter = 0;
	for (auto& Window : Renderer::UIWindows)
	{
		if (Window->Name == Name)
		{
			bFoundWindow = true;
			++FoundCounter;
		}
	}

	sprintf(Id, "%s", InName);

	if (bCanHaveMultiple
		|| (!bCanHaveMultiple && !bFoundWindow))
	{
		if (bFoundWindow)
		{
			char Buf[256];
			sprintf(Buf, "%s###%d", Name, FoundCounter);
			Name = Buf;

			// UE_LOG(LogInit, Warning, "Adding another window {}", std::string(WindowName));
		}
	}
}

void UIWindowBase::Render()
{
	ImGui::Text("Hello! Override me please :)");
}
