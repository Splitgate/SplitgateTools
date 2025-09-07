#pragma once

// Function type macros.
#define VARARGS     __cdecl											/* Functions with variable arguments */
#define STDCALL		__stdcall										/* Standard calling convention */
#define FORCEINLINE __forceinline									/* Force code to be inline */
#define FORCENOINLINE __declspec(noinline)							/* Force code to NOT be inline */

/* Use before a function declaration to warn that callers should not ignore the return value */
#if !defined(UE_NODISCARD) && defined(__has_cpp_attribute)
	#if __has_cpp_attribute(nodiscard)
		#define UE_NODISCARD [[nodiscard]]
	#endif
#endif
#ifndef UE_NODISCARD
	#define UE_NODISCARD
#endif

// Method modifiers
#ifndef ABSTRACT
	#define ABSTRACT
#endif
#ifndef CONSTEXPR
	#define CONSTEXPR constexpr
#endif

#ifndef VARARGS
	#define VARARGS									/* Functions with variable arguments */
#endif
#ifndef CDECL
	#define CDECL									/* Standard C function */
#endif
#ifndef STDCALL
	#define STDCALL									/* Standard calling convention */
#endif
#ifndef FORCEINLINE
	#define FORCEINLINE 							/* Force code to be inline */
#endif
#ifndef FORCENOINLINE
	#define FORCENOINLINE 							/* Force code to NOT be inline */
#endif
#ifndef RESTRICT
	#define RESTRICT __restrict						/* no alias hint */
#endif

enum { INDEX_NONE = -1 };
enum { UNICODE_BOM = 0xfeff };

template<typename T32BITS, typename T64BITS, int PointerSize>
struct SelectIntPointerType
{
	// nothing here are is it an error if the partial specializations fail
};

template<typename T32BITS, typename T64BITS>
struct SelectIntPointerType<T32BITS, T64BITS, 8>
{
	// Select the 64 bit type.
	typedef T64BITS TIntPointer;
};

template<typename T32BITS, typename T64BITS>
struct SelectIntPointerType<T32BITS, T64BITS, 4>
{
	// Select the 32 bit type.
	typedef T32BITS TIntPointer;
};

struct FGenericPlatformTypes
{
	//~ Unsigned base types

	// 8-bit unsigned integer
	typedef unsigned char 		uint8;

	// 16-bit unsigned integer
	typedef unsigned short int	uint16;

	// 32-bit unsigned integer
	typedef unsigned int		uint32;

	// 64-bit unsigned integer
	typedef unsigned long long	uint64;

	//~ Signed base types.

	// 8-bit signed integer
	typedef	signed char			int8;

	// 16-bit signed integer
	typedef signed short int	int16;

	// 32-bit signed integer
	typedef signed int	 		int32;

	// 64-bit signed integer
	typedef signed long long	int64;

	//~ Character types

	// An ANSI character. 8-bit fixed-width representation of 7-bit characters.
	typedef char				ANSICHAR;

	// A wide character. In-memory only. ?-bit fixed-width representation of the platform's natural wide character set. Could be different sizes on different platforms.
	typedef wchar_t				WIDECHAR;

	// An 8-bit character type. In-memory only. 8-bit representation. Should really be char8_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
	typedef uint8				CHAR8;

	// A 16-bit character type. In-memory only.  16-bit representation. Should really be char16_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
	typedef uint16				CHAR16;

	// A 32-bit character type. In-memory only. 32-bit representation. Should really be char32_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
	typedef uint32				CHAR32;

	// A switchable character. In-memory only. Either ANSICHAR or WIDECHAR, depending on a licensee's requirements.
	typedef WIDECHAR			TCHAR;

	// Unsigned int. The same size as a pointer.
	typedef SelectIntPointerType<uint32, uint64, sizeof(void*)>::TIntPointer UPTRINT;

	// Signed int. The same size as a pointer.
	typedef SelectIntPointerType<int32, int64, sizeof(void*)>::TIntPointer PTRINT;

	// Unsigned int. The same size as a pointer.
	typedef UPTRINT SIZE_T;

	// Signed int. The same size as a pointer.
	typedef PTRINT SSIZE_T;

	typedef int32					TYPE_OF_NULL;
	typedef decltype(nullptr)		TYPE_OF_NULLPTR;
};


/**
* Windows specific types
**/
struct FWindowsPlatformTypes : public FGenericPlatformTypes
{
#ifdef _WIN64
	typedef unsigned __int64	SIZE_T;
	typedef __int64				SSIZE_T;
#else
	typedef unsigned long		SIZE_T;
	typedef long				SSIZE_T;
#endif
};

typedef FWindowsPlatformTypes FPlatformTypes;

//~ Unsigned base types.
/// An 8-bit unsigned integer.
typedef FPlatformTypes::uint8		uint8;
/// A 16-bit unsigned integer.
typedef FPlatformTypes::uint16		uint16;
/// A 32-bit unsigned integer.
typedef FPlatformTypes::uint32		uint32;
/// A 64-bit unsigned integer.
typedef FPlatformTypes::uint64		uint64;

//~ Signed base types.
/// An 8-bit signed integer.
typedef	FPlatformTypes::int8		int8;
/// A 16-bit signed integer.
typedef FPlatformTypes::int16		int16;
/// A 32-bit signed integer.
typedef FPlatformTypes::int32		int32;
/// A 64-bit signed integer.
typedef FPlatformTypes::int64		int64;

//~ Character types.
/// An ANSI character. Normally a signed type.
typedef FPlatformTypes::ANSICHAR	ANSICHAR;
/// A wide character. Normally a signed type.
typedef FPlatformTypes::WIDECHAR	WIDECHAR;
/// Either ANSICHAR or WIDECHAR, depending on whether the platform supports wide characters or the requirements of the licensee.
typedef FPlatformTypes::TCHAR		TCHAR;
/// An 8-bit character containing a UTF8 (Unicode, 8-bit, variable-width) code unit.
typedef FPlatformTypes::CHAR8		UTF8CHAR;
/// A 16-bit character containing a UCS2 (Unicode, 16-bit, fixed-width) code unit, used for compatibility with 'Windows TCHAR' across multiple platforms.
typedef FPlatformTypes::CHAR16		UCS2CHAR;
/// A 16-bit character containing a UTF16 (Unicode, 16-bit, variable-width) code unit.
typedef FPlatformTypes::CHAR16		UTF16CHAR;
/// A 32-bit character containing a UTF32 (Unicode, 32-bit, fixed-width) code unit.
typedef FPlatformTypes::CHAR32		UTF32CHAR;

/// An unsigned integer the same size as a pointer
typedef FPlatformTypes::UPTRINT UPTRINT;
/// A signed integer the same size as a pointer
typedef FPlatformTypes::PTRINT PTRINT;
/// An unsigned integer the same size as a pointer, the same as UPTRINT
typedef FPlatformTypes::SIZE_T SIZE_T;
/// An integer the same size as a pointer, the same as PTRINT
typedef FPlatformTypes::SSIZE_T SSIZE_T;

/// The type of the NULL constant.
typedef FPlatformTypes::TYPE_OF_NULL	TYPE_OF_NULL;
/// The type of the C++ nullptr keyword.
typedef FPlatformTypes::TYPE_OF_NULLPTR	TYPE_OF_NULLPTR;
