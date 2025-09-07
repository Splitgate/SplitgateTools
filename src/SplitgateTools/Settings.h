#pragma once

#include "Utilities/nlohmann/json.hpp"
#include "UI/Helpers/ImHelpers.h"

class RaceSettings
{
	friend class Settings;
	void Apply();
public:
	int CountdownLength = 1;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(RaceSettings, CountdownLength);
};

class MiscSettings
{
	friend class Settings;
	void Apply();
public:
	bool bEnableLobbyFPSPatch = true;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MiscSettings, bEnableLobbyFPSPatch);
};

class Settings
{
	void Save();
public:
	void ApplyAndSave()
	{
		Apply();
		Save();
	}
	void Load();

	RaceSettings Race;
	MiscSettings Misc;

private:
	void Apply()
	{
		Race.Apply();
		Misc.Apply();
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Settings, Race, Misc)
};

inline Settings GSettings;