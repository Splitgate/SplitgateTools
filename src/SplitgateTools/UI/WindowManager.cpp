#include "WindowManager.h"

#include "Globals.h"

void WindowManager::Render()
{
	for (auto It = UIWindows.begin(); It != UIWindows.end();)
	{
		auto& Window = *It;
		Window->Tick();

		ImGui::Begin(Window->Name.c_str(),
			Window->bIsClosable ? &Window->bOpen : nullptr,
			Window->WindowFlags | ImGuiWindowFlags_NoCollapse);

		Window->Render();

		ImGui::End();

		if (Window->bOpen)
		{
			++It;
		}
		else // Destroy if no longer open
		{
			Window->OnClose();
			It = UIWindows.erase(It);
		}
	}
}

void WindowManager::FlushPendingWindows()
{
	for (std::unique_ptr<UIWindowBase>& Window : PendingUIWindows)
		AddInternal(std::move(Window));
	PendingUIWindows.clear();
}

void WindowManager::AddInternal(std::unique_ptr<UIWindowBase> Window)
{
	int Found = 0;
	for (auto& ItWindow : UIWindows)
		if (Window->OriginalName == ItWindow->OriginalName)
			++Found;
	
	if (Found > 0)
	{
		if (!Window->bCanHaveMultiple)
			return;
		
		Window->Name.append("##").append(std::to_string(rand())); //lol
		//UE_LOG(LogInit, Warning, "Adding another window {}", std::string(WindowName));
	}

	UIWindows.emplace_back(std::move(Window));
	UIWindows.back()->OnOpen();
}
