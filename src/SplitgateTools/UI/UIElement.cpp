#include "UIElement.h"
#include "Renderer.h"

UIElement::UIElement(const char* InWindowName, bool InbCanHaveMultiple, bool InbIsClosable, ImGuiWindowFlags InWindowFlags)
	: WindowName(InWindowName),
	bCanHaveMultiple(InbCanHaveMultiple),
	bIsClosable(InbIsClosable),
	WindowFlags(InWindowFlags)
{
	bool bFoundWindow = false;

	int FoundCounter = 0;
	for (auto& UIElement : Renderer::UIElements)
	{
		if (UIElement->WindowName == WindowName)
		{
			bFoundWindow = true;
			++FoundCounter;
		}
	}

	sprintf(WindowId, "%s", InWindowName);

	if (bCanHaveMultiple
		|| (!bCanHaveMultiple && !bFoundWindow))
	{
		if (bFoundWindow)
		{
			char Buf[256];
			sprintf(Buf, "%s###%d", WindowName, FoundCounter);
			WindowName = Buf;

			// UE_LOG(LogInit, Warning, "Adding another window {}", std::string(WindowName));
		}
	}
}

void UIElement::Render()
{
	ImGui::Text("Hello! Override me please :)");
}
