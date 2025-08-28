#include "IPlatformFilePak.h"
#include "Memory/Hook.h"
#include "Strings/Strings.h"

#include <filesystem>

static bool (*FindFileInPakFiles)(void*, const TCHAR*, void**, void*);
static bool (*IsNonPakFilenameAllowed)(IPlatformFilePak*, const FString&);

void IPlatformFilePak::Init_PreEngine()
{
	::FindFileInPakFiles = Memory::FindStringRef(L"Delete Record: No lower priority pak files looking for %s. (maybe not downloaded?)").FuncStart();
	LOG_ADDRESS(::FindFileInPakFiles, "IPlatformFilePak::FindFileInPakFiles");
	HOOK(FindFileInPakFiles);

	::IsNonPakFilenameAllowed = Memory::FindPattern("49 8B CF E8 ?? ?? ?? ?? 48 8B ?? ?? 0F B6 D8 48 85 C9 74 ?? E8").Scan("E8").Add(1).Rel32();
	LOG_ADDRESS(::IsNonPakFilenameAllowed, "IPlatformFilePak::IsNonPakFilenameAllowed");
	HOOK(IsNonPakFilenameAllowed);
}

// Doesnt appear to work on its own with I/O Store enabled

bool IPlatformFilePak::FindFileInPakFiles(void* PakFile, const wchar_t* Filename, void** OutPakFile, void* OutEntry)
{
	if (OutPakFile)
	{
		//OutEntry = nullptr;
		*OutPakFile = nullptr;
	}

	if (Filename && std::filesystem::exists(Filename))
	{
		return false;
	}

    return ::FindFileInPakFiles(PakFile, Filename, OutPakFile, OutEntry);
}

bool IPlatformFilePak::IsNonPakFilenameAllowed(const FString& InFilename)
{
	return true;
}
