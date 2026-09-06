#pragma once

#include "Globals/FForgeMap.h"

namespace ForgeUtility
{
	bool ParseMap(nlohmann::json ForgeJson, FForgeMap& OutMap);
	bool LoadFromFile(FForgeMap& OutMap);
}