#include "MiscSettingsTab.h"

#include "imgui.h"
#include "Settings.h"
#include "PortalWars/UPortalWarsGameEngine.h"

void MiscSettingsTab::RenderContent()
{
	// todo: wrapper funcs for rendering these
	if (ImGui::Checkbox("Uncap FPS in lobby", &GSettings.Misc.bEnableLobbyFPSPatch))
	{
		// todo: move to GSettings.Apply() or something 
		UPortalWarsGameEngine::UpdateLobbyFPSPatch();
		GSettings.Save();
	}
}
