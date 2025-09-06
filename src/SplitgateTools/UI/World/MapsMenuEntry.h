#pragma once
#include "UI/UITitleBarEntry.h"
#include <string>
#include <vector>

// Switch case for imgui entries
namespace MapSelectability
{
	enum Type : int
	{
		None = 0,
		Playable = 1 << 0,
		Forge = 1 << 1,
		Race = 1 << 2,
		Misc = 2 << 2,
	};

	static inline const char* ToString(MapSelectability::Type MapType)
	{
		switch (MapType)
		{
		case Playable:
			return "Playable";

		case Forge:
			return "Forge";

		case Race:
			return "Race";

		case Misc:
			return "Misc";
		}
		return "Playable";
	}
}

// Save map data, slight optimisation, maps SHOULDNT be made at runtime...
class GameMapEntry
{
public:
	
	std::string DisplayName;
	std::wstring InternalName;
	int Types;

	bool HasSelectability(MapSelectability::Type Type) const
	{
		return Types & Type;
	}
	
	void OnPressed(const std::wstring& RaceType = L"");
};

class MapsMenuEntry : public UITitleBarEntry
{
public:

	MapsMenuEntry();

protected:

	virtual void Render() override;

private:

	static inline std::vector<GameMapEntry> AllMaps;
};
