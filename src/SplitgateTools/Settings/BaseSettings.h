#pragma once

#define MINI_CASE_SENSITIVE
#include "mINI/ini.h"

#define CONFIG_FILENAME "Settings.ini"

class BaseSettings
{
	const char* SectionName = "General";

	mINI::INIFile SettingsFile;
	mINI::INIStructure INIData;

public:

	BaseSettings(const char* InSectionName = "General")
		: SectionName(InSectionName), SettingsFile(CONFIG_FILENAME)
	{
	}

	static BaseSettings& Get()
	{
		static BaseSettings Settings;
		return Settings;
	}

	// Writes setting to this settings given section with name
	template<typename DataType>
	void WriteValue(const char* Setting, DataType Value)
	{
		if constexpr (std::is_same<DataType, std::string>())
		{
			INIData[SectionName][Setting] = Value;
		}
		else
		{
			INIData[SectionName][Setting] = std::to_string(Value);
		}

		UpdateSettings();
	}

	// Get value or default if it doesnt exist
	template<typename DataType>
	DataType GetValue(const char* Setting, DataType DefaultValue)
	{
		if (INIData[SectionName].has(Setting))
		{
			std::string Value = INIData[SectionName][Setting];
			return StringToType<DataType>(Value);
		}

		return DefaultValue;
	}

	void UpdateSettings()
	{
		SettingsFile.write(INIData);
	}

	// Override on subclasses to implement default file settings if config is empty
	virtual mINI::INIStructure GatherMyDefaults()
	{
		mINI::INIStructure BaseSettingsTest;
		BaseSettingsTest[SectionName]["TestData"] = "20";

		return BaseSettingsTest;
	}

	void Init();

private:

	virtual void OnInit() {};
};