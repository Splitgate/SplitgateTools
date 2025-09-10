#include "MiscSettingsTab.h"

#include "imgui.h"
#include "Settings.h"
#include "PortalWars/UPortalWarsGameEngine.h"

void MiscSettingsTab::RenderContent()
{
	// todo: wrapper funcs for rendering these
	RenderBoolSetting("Uncap lobby FPS", 
		"Uncaps lobby framerate from 60 to current framerate", 
		&GSettings.Misc.bEnableLobbyFPSPatch);
	
	// TODO: THIS DOESNT HANDLE UpdateLobbyFPSPatch
	// we could just add it but that doesn't scale very well and is quite bug prone
	// maybe we make a function called something like Settings::Apply, where patches and hooks are applied
	// we'd have to do it at the very end of Init_PostEngine to make sure all offsets have been found
	if (ImGui::Button("Reload settings from file"))
		GSettings.Load();
}
