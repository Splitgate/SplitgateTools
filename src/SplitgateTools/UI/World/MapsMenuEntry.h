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
		QA = 2 << 2,
	};

	static const char* ToString(MapSelectability::Type MapType)
	{
		switch (MapType)
		{
		case Playable:
			return "Playable";

		case Forge:
			return "Forge";

		case Race:
			return "Race";

		case QA:
			return "QA";
		}
		return "Playable";
	}
}

// Save map data, slight optimisation, maps SHOULDNT be made at runtime...
struct GameMapEntry
{
	std::string DisplayName;
	std::string InternalName;
	int Types;

	bool HasSelectability(MapSelectability::Type Type)
	{
		return Types & Type;
	}
};

class MapsMenuEntry : public UITitleBarEntry
{
public:

	MapsMenuEntry() : UITitleBarEntry("Maps")
	{
		OnCreate();
	}

protected:

	void OnMapPressed(GameMapEntry Map, const char* RaceType = "", bool bForceForRace = false);

	virtual void Render() override;
	virtual void OnCreate() override;

private:

	static inline std::vector<GameMapEntry> AllMaps;
};
