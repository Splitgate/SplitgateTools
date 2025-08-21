#include "FMemory.h"
#include "Memory/Hook.h"

static void* (*Realloc)(void*, size_t, unsigned int);

void FMemory::Init_PreEngine()
{
	::Realloc = Memory::FindPattern("48 89 5C 24 ?? 48 89 74 24 ?? 57 48 83 EC ?? 48 8B F1 41 8B D8 48 8B 0D ?? ?? ?? ?? 48 8B FA 48 85 C9");
	LOG_ADDRESS(::Realloc, "FMemory::Realloc");
}

void* FMemory::Malloc(size_t Count, unsigned int Alignment)
{
	return ::Realloc(nullptr, Count, Alignment);
}

void* FMemory::Realloc(void* Original, size_t Count, unsigned int Alignment)
{
	return ::Realloc(Original, Count, Alignment);
}

void FMemory::Free(void* Original)
{
	::Realloc(Original, 0, 0);
}