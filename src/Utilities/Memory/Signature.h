#pragma once

#include <vector>

// Use instead of ?? in sigs being initialized from vector
enum 
{
	__ = -1
};
enum Opcodes : uint8_t 
{
	NOP = 0x90,
	RET = 0xC3,
	CALL = 0xE8,
	JMP = 0xE9,
	JMP8 = 0xEB
};

class Signature
{
public:
	std::vector<int> Bytes;

	Signature(int InBytes) : Bytes({ InBytes }) { };

	Signature(std::vector<int> InBytes) : Bytes(InBytes) { };

	Signature(std::initializer_list<int> InBytes) : Bytes(InBytes) { };

	Signature(const char* InString)
	{
		const auto ItStart = const_cast<char*>(InString);
		const auto ItEnd = const_cast<char*>(InString) + strlen(InString);
		for (auto It = ItStart; It < ItEnd; ++It)
		{
			if (*It == '?')
			{
				++It;
				if (*It == '?') ++It;
				Bytes.push_back(__);
			}
			else
			{
				Bytes.push_back(strtoul(It, &It, 16));
			}
		}
	}
};