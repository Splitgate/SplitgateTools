#pragma once
#include "FName.h"

struct FPrimaryAssetType
{
	FPrimaryAssetType(FName Type)
		: Name(Type)
	{ }

	FPrimaryAssetType()
		: Name()
	{ }

	FName Name;
};

struct FPrimaryAssetId
{
	FPrimaryAssetType PrimaryAssetType;
	FName PrimaryAssetName;

	FPrimaryAssetId()
		: PrimaryAssetType("None"),
		PrimaryAssetName("None")
	{ };

	FPrimaryAssetId(FName Type, FName AssetName)
		: PrimaryAssetType(Type),
		PrimaryAssetName(AssetName)
	{ }
};