#pragma once
#include <stdint.h>
#include <corecrt_memory.h>

class FMemory
{
public:
	static void Init_PreEngine();
	
	static void* Malloc(size_t Count, unsigned int Alignment = 0);
	static void* Realloc(void* Original, size_t Count, unsigned int Alignment = 0);
	static void Free(void* Original);

	// Platform memory functions
	static inline void* Memmove(void* Dest, const void* Src, size_t Count)
	{
		return memmove(Dest, Src, Count);
	}

	static inline int32_t Memcmp(const void* Buf1, const void* Buf2, size_t Count)
	{
		return memcmp(Buf1, Buf2, Count);
	}

	static inline void* Memset(void* Dest, uint8_t Char, size_t Count)
	{
		return memset(Dest, Char, Count);
	}

	static inline void* Memzero(void* Dest, size_t Count)
	{
		return memset(Dest, 0, Count);
	}

	static inline void* Memcpy(void* Dest, const void* Src, size_t Count)
	{
		return memcpy(Dest, Src, Count);
	}
};
