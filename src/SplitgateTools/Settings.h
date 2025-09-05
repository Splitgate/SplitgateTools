#pragma once

#include "Utilities/nlohmann/json.hpp"

class RaceSettings
{
public:
	float CountdownLength = 3;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(RaceSettings, CountdownLength);
};

class MiscSettings
{
public:
	bool bEnableLobbyFPSPatch = true;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MiscSettings, bEnableLobbyFPSPatch);
};

class Settings
{
public:
	void Save();
	void Load();

	RaceSettings Race;
	MiscSettings Misc;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Settings, Race, Misc)
};

inline Settings GSettings;