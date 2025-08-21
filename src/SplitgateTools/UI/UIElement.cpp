#include "UIElement.h"

UIElement::UIElement(const char* InWindowName, bool InbIsClosable, bool InbIsOpen, ImGuiWindowFlags InWindowFlags)
	: WindowName(InWindowName),
	bIsClosable(InbIsClosable),
	bIsOpen(InbIsOpen),
	WindowFlags(InWindowFlags)
{
}

UIElement::~UIElement()
{

}

void UIElement::Tick()
{
	if (!bIsOpen)
	{
		CloseWindow();
	}
}

void UIElement::Render()
{

}

void UIElement::CloseWindow()
{
	if (!bIsClosable)
		return;
}
