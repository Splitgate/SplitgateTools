#pragma once

enum class SettingType
{
	String,
	Boolean,
	Integer,
	Float
};

class SettingsEntry
{
public:

	SettingsEntry(const char* InEntryName);
	virtual ~SettingsEntry();

	// Tab Name
	const char* EntryName;

	// Tab Content
	virtual void RenderContent();
};