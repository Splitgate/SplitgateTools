#include "BaseSettings.h"

#include "Globals.h"

void BaseSettings::Init()
{
	if (!SettingsFile.read(INIData))
	{
		UE_LOG(LogConfig, Display, "Creating default config");

		if (SettingsFile.generate(GatherMyDefaults(), true))
			UE_LOG(LogConfig, Error, "Failed to generate default config");
	}

	OnInit();
}
