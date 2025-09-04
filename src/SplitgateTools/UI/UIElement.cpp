#include "UIElement.h"
#include "Renderer.h"

UIElement::UIElement(const char* InWindowName, bool InbCanHaveMultiple, bool InbIsClosable, bool InbIsOpen, ImGuiWindowFlags InWindowFlags)
	: WindowName(InWindowName),
	bCanHaveMultiple(InbCanHaveMultiple),
	bIsClosable(InbIsClosable),
	bIsOpen(InbIsOpen),
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

		// Add an element
		Renderer::UIElements.push_back(this);

		// OnOpen();
	}
}

UIElement::~UIElement()
{
	CloseInternal();
}

void UIElement::Tick()
{
	if (!HasOpened)
	{
		HasOpened = true;
		OnOpen();
	}

	if (!bIsOpen)
	{
		CloseWindow();
	}
}

void UIElement::Render()
{
	ImGui::Text("Hello! Override me please :)");
}

void UIElement::CloseWindow()
{
	if (!bIsClosable)
		return;

	CloseInternal();
}

void UIElement::CloseInternal()
{
	auto ElemIndex = std::find(Renderer::UIElements.begin(), Renderer::UIElements.end(), this);
	Renderer::UIElements.erase(ElemIndex);
	bIsOpen = false;

	// Fire closure event
	OnClose();
}
