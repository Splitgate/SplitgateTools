#include "Memory.h"

Memory::Address Memory::Base(const wchar_t* ModuleName)
{
	return uint64_t(GetModuleHandleW(ModuleName));
}

Memory::Address Memory::RVA(Address InRVA, const wchar_t* ModuleName)
{
	return Base(ModuleName).Add(InRVA);
}

Memory::Address Memory::Size(const wchar_t* ModuleName)
{
	return GetNtHeaders(ModuleName)->OptionalHeader.SizeOfImage;
}

PIMAGE_NT_HEADERS Memory::GetNtHeaders(const wchar_t* ModuleName)
{
	Address Base = Memory::Base(ModuleName);
	return Base.Add(Base.As<PIMAGE_DOS_HEADER>()->e_lfanew).As<PIMAGE_NT_HEADERS>();
}

Memory::Address Memory::GetEntryPoint(const wchar_t* ModuleName)
{
	return Base(ModuleName).Add(GetNtHeaders(ModuleName)->OptionalHeader.AddressOfEntryPoint);
}

Memory::Address Memory::GetVirtualFunction(void* Ptr, int Offset)
{
	return *reinterpret_cast<void**>(*(uint64_t*)Ptr + Offset);
}
Memory::Address Memory::DetourVFT(void* Ptr, int Offset, Address Detour)
{
	auto TargetPtr = (uint64_t*)((uint64_t) * reinterpret_cast<void***>(Ptr) + Offset);
	UNLOCK(TargetPtr, 8);
	auto Target = *TargetPtr;
	*TargetPtr = Detour.As<uint64_t>();
	return (void*)Target;
}

Memory::Address Memory::Address::Scan(Signature InSignature, int Skip, int Dist, bool Forward)
{
	if (Memory::Address::IsNull()) return 0;

	Memory::Address OutAddr = 0;

	int NumSkipped = 0;
	int NumFound = 0;
	bool bSuccess = false;

	for (int i = 0; Forward ? (i < Dist) : (i > -Dist); Forward ? i++ : i--)
	{
		for (int Byte : InSignature.Bytes)
		{
			if ((Byte != __) && Ptr[i + NumFound] != Byte)
			{
				NumFound = 0;
				break;
			}

			OutAddr = &Ptr[i];

			NumFound++;

			if (NumFound != InSignature.Bytes.size())
				continue;

			if (Skip != NumSkipped)
			{
				NumSkipped++;
				NumFound = 0;
				break;
			}

			bSuccess = true;
			goto ret;
		}
	}

ret:
	if (!bSuccess)
		return 0;

	return OutAddr;
}

typedef struct _UNWIND_INFO {
	unsigned char Version : 3;
	unsigned char Flags : 5;
	unsigned char SizeOfProlog;
	unsigned char CountOfCodes;
	unsigned char FrameRegister : 4;
	unsigned char FrameOffset : 4;
	ULONG ExceptionHandler;
} UNWIND_INFO, * PUNWIND_INFO;

Memory::Address Memory::Address::FuncStart()
{
	if (IsNull()) return 0;

	uint64_t ImageBase = 0;

	for (PRUNTIME_FUNCTION Func = RtlLookupFunctionEntry((uint64_t)Ptr, &ImageBase, 0);
		Func != nullptr;
		Func = RtlLookupFunctionEntry(ImageBase + (Func->BeginAddress - 1), &ImageBase, 0))
	{
		if (((PUNWIND_INFO)(ImageBase + Func->UnwindInfoAddress))->Flags & UNW_FLAG_CHAININFO) continue;
		return reinterpret_cast<uint8_t*>(ImageBase + Func->BeginAddress);
	}
	return 0;
}

bool Memory::Address::IsBadRead()
{
	MEMORY_BASIC_INFORMATION mbi;
	if (!IsNull() && VirtualQuery(As<void*>(), &mbi, sizeof(mbi)))
	{
		DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
		bool b = !(mbi.Protect & mask);
		if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
			b = true;
		return b;
	}
	return true;
}

Memory::Address Memory::Address::AllocPageNear()
{
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
	const uint64_t PAGE_SIZE = SysInfo.dwPageSize;

	uint64_t StartAddr = ((uint64_t)Ptr & ~(PAGE_SIZE - 1));
	uint64_t MinAddr = min(StartAddr - 0x7FFFFF00, (uint64_t)SysInfo.lpMinimumApplicationAddress);
	uint64_t MaxAddr = max(StartAddr + 0x7FFFFF00, (uint64_t)SysInfo.lpMaximumApplicationAddress);

	uint64_t StartPage = (StartAddr - (StartAddr % PAGE_SIZE));
	uint64_t PageOffset = 1;

	while (true)
	{
		uint64_t ByteOffset = PageOffset * PAGE_SIZE;
		uint64_t HighAddr = StartPage + ByteOffset;
		uint64_t LowAddr = (StartPage > ByteOffset) ? StartPage - ByteOffset : 0;

		bool bNeedsExit = HighAddr > MaxAddr && LowAddr < MinAddr;

		if (HighAddr < MaxAddr)
		{
			void* OutAddr = VirtualAlloc((void*)HighAddr, PAGE_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			if (OutAddr) return OutAddr;
		}
		if (LowAddr > MinAddr)
		{
			void* OutAddr = VirtualAlloc((void*)LowAddr, PAGE_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			if (OutAddr) return OutAddr;
		}

		PageOffset++;
		if (bNeedsExit)
			break;
	}

	return 0;
}

void Memory::Address::WriteAbsoluteJump64(Address JumpAddr)
{
	uint8_t Instructions[] = { 0x49, 0xBA, 0,0,0,0,0,0,0,0,
							   0x41, 0xFF, 0xE2 };
	uint64_t JumpAddr64 = JumpAddr.As<uint64_t>();
	memcpy(&Instructions[2], &JumpAddr64, sizeof(JumpAddr64));
	memcpy((void*)Ptr, Instructions, sizeof(Instructions));
}

void Memory::Address::DetourRel32(Address Detour)
{
	Address RelayPtr = AllocPageNear();
	RelayPtr.WriteAbsoluteJump64(Detour);

	uint64_t RelAddr = RelayPtr.Subtract(*this).Subtract(4);
	Write(&RelAddr, 4);
}

Memory::Address Memory::FindString(const char* InString, Section InSection)
{
	Address Start = InSection.Start();

	for (uint32_t i = 0; i < InSection.Size(); i++)
	{
		auto Other = Start.Add(i).As<const char*>();
		if (strcmp(InString, Other) == 0)
			return Other;
	}

	return 0;
}

Memory::Address Memory::FindString(const wchar_t* InString, Section InSection)
{
	Address Start = InSection.Start();

	for (uint32_t i = 0; i < InSection.Size(); i++)
	{
		auto Other = Start.Add(i).As<const wchar_t*>();
		if (wcscmp(InString, Other) == 0)
			return Other;
	}

	return 0;
}
