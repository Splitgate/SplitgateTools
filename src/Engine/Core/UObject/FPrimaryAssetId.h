#pragma once
#include "FName.h"

struct FPrimaryAssetType
{
	FPrimaryAssetType(FName Type)
		: Name(Type)
	{ }

	FName Name;
};

struct FPrimaryAssetId
{
	FPrimaryAssetType PrimaryAssetType;
	FName PrimaryAssetName;

	FPrimaryAssetId(FName Type, FName AssetName)
		: PrimaryAssetType(Type),
		PrimaryAssetName(AssetName)
	{ }
};