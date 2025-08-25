#pragma once
#include "Containers/FString.h"

enum class EVersionComponent
{
	Major,					///< Major version increments introduce breaking API changes.
	Minor,					///< Minor version increments add additional functionality without breaking existing APIs.
	Patch,					///< Patch version increments fix existing functionality without changing the API.
	Changelist,				///< The pre-release field adds additional versioning through a series of comparable dotted strings or numbers.
	Branch,					///<
};


/** Components of a version string. */
enum class EVersionComparison
{
	Neither,
	First,
	Second,
};

class FEngineVersionBase
{
public:

	/** Major version number. */
	uint16_t Major = 0;

	/** Minor version number. */
	uint16_t Minor = 0;

	/** Patch version number. */
	uint16_t Patch = 0;

	/** Changelist number. This is used to arbitrate when Major/Minor/Patch version numbers match. Use GetChangelist() instead of using this member directly. */
	uint32_t Changelist = 0;
};

class FEngineVersion : public FEngineVersionBase
{
public:
	static void Init_PreGame();

	static FEngineVersion* Current();

	/** Branch name. */
	FString Branch;
};