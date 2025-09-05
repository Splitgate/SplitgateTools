#include "BaseSettings.h"

#include "Globals.h"

void BaseSettings::Init()
{
	if (!SettingsFile.read(INIData))
	{
		UE_LOG(LogConfig, Display, "Creating default config");

		INIData = GatherMyDefaults();
		UpdateSettings();
	}

	OnInit();
}
