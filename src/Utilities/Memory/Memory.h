#pragma once

#include <Windows.h>
#include <winnt.h>

#include <cstdint>
#include <memoryapi.h>

#include <memory>

#include "Signature.h"

#define UNLOCK(Ptr,Size) Memory::Unlocker Unlocker_##__COUNTER__{Ptr,Size};

class Memory
{
public:
	class Unlocker
	{
		void* Ptr;
		size_t Size;
		DWORD OldProtection;

	public:
		template<typename T = uint64_t>
		inline Unlocker(T InPtr, size_t InSize) : Ptr((void*)InPtr), Size(InSize)
		{
			VirtualProtect((void*)InPtr, InSize, PAGE_EXECUTE_READWRITE, &OldProtection);
		}

		inline ~Unlocker()
		{
			VirtualProtect(Ptr, Size, OldProtection, &OldProtection);
		}
	};

	class Address;

	static Address Base(const wchar_t* ModuleName = nullptr);
	static Address RVA(Address InRVA, const wchar_t* ModuleName = nullptr);
	static Address Size(const wchar_t* ModuleName = nullptr);

	static PIMAGE_NT_HEADERS GetNtHeaders(const wchar_t* ModuleName = nullptr);
	static Address GetEntryPoint(const wchar_t* ModuleName = nullptr);

	static Address GetVirtualFunction(void* Ptr, int Offset);
	static Address DetourVFT(void* Ptr, int Offset, Address Detour);

	class Address
	{
		const uint8_t* Ptr;

	public:
		inline bool IsValid() { return !!Ptr; }
		inline bool IsNull() { return !Ptr; }

		inline Address() : Ptr(nullptr) { }

		template<typename T = uint64_t>
		inline Address(T InPtr) : Ptr((uint8_t*)uint64_t(InPtr)) {}

		template<typename T = uint64_t>
		inline operator T&() { return (T&)Ptr; }

		template<typename T = uint64_t>
		inline T& As() { return (T&)Ptr; }

		inline bool operator>(uint64_t InOffset)
		{
			return uint64_t(Ptr) > InOffset;
		}
		inline bool operator>(Address InOffset)
		{
			return uint64_t(Ptr) > uint64_t(InOffset.Ptr);
		}

		inline bool operator<(uint64_t InOffset)
		{
			return uint64_t(Ptr) < InOffset;
		}
		inline bool operator<(Address InOffset)
		{
			return uint64_t(Ptr) < uint64_t(InOffset.Ptr);
		}

		inline bool operator>=(uint64_t InOffset)
		{
			return uint64_t(Ptr) >= InOffset;
		}
		inline bool operator>=(Address InOffset)
		{
			return uint64_t(Ptr) >= uint64_t(InOffset.Ptr);
		}

		inline bool operator<=(uint64_t InOffset)
		{
			return uint64_t(Ptr) <= InOffset;
		}
		inline bool operator<=(Address InOffset)
		{
			return uint64_t(Ptr) <= uint64_t(InOffset.Ptr);
		}

		inline bool operator==(uint64_t InOffset)
		{
			return uint64_t(Ptr) == InOffset;
		}
		inline bool operator==(Address InOffset)
		{
			return uint64_t(Ptr) == uint64_t(InOffset.Ptr);
		}

		inline bool operator!=(uint64_t InOffset)
		{
			return uint64_t(Ptr) != InOffset;
		}
		inline bool operator!=(Address InOffset)
		{
			return uint64_t(Ptr) != uint64_t(InOffset.Ptr);
		}

		inline Address Add(Address InOffset)
		{
			if (IsNull()) return 0;
			return reinterpret_cast<uint8_t*>(uint64_t(Ptr) + InOffset.As<int64_t>());
		}

		template<typename T = int64_t>
		inline Address Add(T InOffset)
		{
			if (IsNull()) return 0;
			return reinterpret_cast<uint8_t*>(uint64_t(Ptr) + InOffset);
		}

		inline Address Subtract(Address InOffset)
		{
			if (IsNull()) return 0;
			return reinterpret_cast<uint8_t*>(uint64_t(Ptr) - InOffset.As<int64_t>());
		}

		template<typename T = int64_t>
		inline Address Subtract(T InOffset)
		{
			if (IsNull()) return 0;
			return reinterpret_cast<uint8_t*>(uint64_t(Ptr) - InOffset);
		}

		inline Address Rel32()
		{
			if (IsNull()) return 0;
			return reinterpret_cast<uint8_t*>((uint64_t(Ptr) + 4) + *(int32_t*)Ptr);
		}

		template<typename T = Address>
		inline T AsRVA(const wchar_t* ModuleName = nullptr)
		{
			return Subtract(Memory::Base(ModuleName).As<int64_t>()).As<T>();
		}

		Address Scan(Signature InSignature, int Skip = 0, int Dist = 2048, bool Forward = true);

		inline Address ReverseScan(Signature InSignature, int Skip = 0, int Dist = 2048)
		{
			return Scan(InSignature, Skip, Dist, false);
		}

		inline Address ScanVirtualCall(int Skip = 0, int Dist = 2048)
		{
			return Scan({ 0xFF, __, __, __, __, 0x00 }, Skip, Dist);
		}

		inline Address ReverseScanVirtualCall(int Skip = 0, int Dist = 2048)
		{
			return ReverseScan({ 0xFF, __, __, __, __, 0x00 }, Skip, Dist);
		}

		Address FuncStart();
		bool IsBadRead();
		
		// Unsafe functions (check if pointer is null before using)
		template<typename T = Address>
		inline T& Deref() { return *(T*)Ptr; }

		inline Address Write(Signature InSignature)
		{
			UNLOCK(Ptr, InSignature.Bytes.size());

			for (int i = 0; i < InSignature.Bytes.size(); i++)
			{
				if (InSignature.Bytes[i] == __)
					continue;

				*reinterpret_cast<uint8_t*>(As<int64_t>() + i) = InSignature.Bytes[i];
			}

			return *this;
		}

		inline Address Write(auto InData, size_t InSize)
		{
			UNLOCK(Ptr, InSize);
			memcpy((void*)Ptr, (void*)InData, InSize);
			return *this;
		}

		inline Address Ret0()
		{
			Write({ 0xB8, 0x00, 0x00, 0x00, 0x00, RET });
			return *this;
		}
		inline Address Ret1()
		{
			Write({ 0xB8, 0x01, 0x00, 0x00, 0x00, RET });
			return *this;
		}
		inline Address Ret2()
		{
			Write({ 0xB8, 0x02, 0x00, 0x00, 0x00, RET });
			return *this;
		}

		Address AllocPageNear();
		void WriteAbsoluteJump64(Address JumpAddr);
		void DetourRel32(Address Detour);
	};

	class Section
	{
		PIMAGE_SECTION_HEADER Ptr{};
		const wchar_t* ModuleName{};

	public:
		Section(const char* Name, const wchar_t* InModuleName = nullptr)
			: ModuleName(InModuleName)
		{
			PIMAGE_NT_HEADERS NtHeaders = Memory::GetNtHeaders(ModuleName);
			auto Sections = IMAGE_FIRST_SECTION(NtHeaders);
			for (WORD i = 0; i < NtHeaders->FileHeader.NumberOfSections; i++)
			{
				auto& Section = Sections[i];
				if (strcmp((const char*)Section.Name, Name))
					continue;
				Ptr = &Section;
				return;
			}
		}

		uint32_t Size()
		{
			if (!Ptr) return 0;
			return Ptr->Misc.VirtualSize;
		}
		Address Start()
		{
			if (!Ptr) return 0;
			return Address(Memory::Base(ModuleName).Add(Ptr->VirtualAddress));
		}
		Address End()
		{
			return Address(Start().Add(Size()));
		}
		bool Contains(Address InAddress)
		{
			return InAddress >= Start() && InAddress < End();
		}
	};

	static Address FindPattern(Signature InSignature, int Skip = 0, Section InSection = ".text")
	{
		return InSection.Start().Scan(InSignature, Skip, InSection.Size());
	}

	static Address FindString(const char* InString, Section InSection = ".rdata");
	static Address FindString(const wchar_t* InString, Section InSection = ".rdata");

	static Address FindStringRef(auto InString, Section StringSection = ".rdata", Section RefSection = ".text")
	{
		Address Start = RefSection.Start();
		Address StringAddress = FindString(InString, StringSection);

		for (uint32_t i = 0; i < RefSection.Size(); i++)
		{
			uint8_t& Byte = Start.Add(i).Deref<uint8_t>();
			if (Byte != 0x4C && Byte != 0x48)
				continue;

			uint8_t& NextByte = Start.Add(i + 1).Deref<uint8_t>();
			if (NextByte != 0x8D)
				continue;

			if (StringAddress == Start.Add(i + 3).Rel32())
				return Start.Add(i);
		}

		return 0;
	}
	static std::vector<Address> FindStringRefs(auto InString, Section StringSection = ".rdata", Section RefSection = ".text")
	{
		Address Start = RefSection.Start();
		Address StringAddress = FindString(InString, StringSection);

		std::vector<Address> OutAddr{};

		for (uint32_t i = 0; i < RefSection.Size(); i++)
		{
			uint8_t& Byte = Start.Add(i).Deref<uint8_t>();
			if (Byte != 0x4C && Byte != 0x48)
				continue;

			uint8_t& NextByte = Start.Add(i + 1).Deref<uint8_t>();
			if (NextByte != 0x8D)
				continue;

			if (StringAddress == Start.Add(i + 3).Rel32())
				OutAddr.push_back(Start.Add(i));
		}

		return OutAddr;
	}

	template<int Alignment = 4, int Max = 2048, typename ValueType>
	static int FindOffsetByValue(Address Object, ValueType Value)
	{
		for (int i = 0; i < Max; i += Alignment)
			if (Object.Add(i).Deref<ValueType>() == Value)
				return i;
		return -1;
	}
	
	template<int Alignment = 8, int Max = 2048, typename ValueType>
	static int FindOffsetByValue(Address Object, ValueType Value, int MemberOffset)
	{
		for (int i = 0; i < Max; i += Alignment)
		{
			Address Member = Object.Add(i).Deref();
			if (!Member.IsBadRead() &&
				Member.Add(MemberOffset).Deref<ValueType>() == Value)
				return i;
		}
		return -1;
	}
};