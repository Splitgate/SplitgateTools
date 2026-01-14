#pragma once
#include "UI/Helpers/ImHelpers.h"
#include "UI/Windows/TabbedWindowBase.h"

#include <string>

#include "Utilities/nlohmann/json.hpp"

enum ELoadState
{
	Failed = 0,
	Loading = 1,
	Loaded = 2,
};

struct CompositeUserId
{
	std::string platformId;
	std::string platform;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(CompositeUserId, platformId, platform)
};

struct LeaderboardEntry
{
	CompositeUserId compositeUserId;
	int value;
	std::string displayName;

	bool operator==(const LeaderboardEntry& Other) 
	{
		return value == Other.value;
	}

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(LeaderboardEntry, compositeUserId, value, displayName)
};

class LeaderboardTab : public TabEntryBase
{
public:
	LeaderboardTab() : TabEntryBase() {}
	LeaderboardTab(std::string InName) : TabEntryBase(InName) {}

	std::map<std::string, std::vector<LeaderboardEntry>> CachedLeaderboard; // Difficulty, Data

	// For checking if http data is loaded
	ELoadState LoadState;

	void FetchLeaderboard();
	void DrawLeaderboardData(const char* Difficulty);

	virtual void RenderContent() override;
	virtual void OnSelected() override;
};