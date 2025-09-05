#include "Settings.h"
#include <fstream>

void Settings::Save()
{
	nlohmann::json j;
	to_json(j, *this);
	std::ofstream("Settings.json") << std::setw(4) << j << '\n';
}

void Settings::Load()
{
	std::ifstream Stream("Settings.json");
	if (Stream)
	{
		nlohmann::json j = nlohmann::json::parse(Stream);
		from_json(j, *this);
	}
}
