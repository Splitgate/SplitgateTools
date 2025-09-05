#include "Settings.h"
#include <fstream>

void Settings::Save()
{
	nlohmann::json j;
	to_json(j, *this);
	std::ofstream("settings.json") << std::setw(4) << j << '\n';
}

void Settings::Load()
{
	std::ifstream Stream("settings.json");
	if (Stream)
	{
		nlohmann::json j = nlohmann::json::parse(Stream);
		from_json(j, *this);
	}
}
