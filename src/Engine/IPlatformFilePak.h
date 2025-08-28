#pragma once
#include "Containers/FString.h"

class IPlatformFilePak
{
public:

	static void Init_PreEngine();

	static bool FindFileInPakFiles(void* PakFile, const wchar_t* Filename, void** OutPakFile, void* OutEntry);
	bool IsNonPakFilenameAllowed(const FString& InFilename);
};