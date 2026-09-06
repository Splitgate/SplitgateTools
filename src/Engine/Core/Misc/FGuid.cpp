#include "FGuid.h"

std::string FGuid::ToString(EGuidFormats Format) const
{
	switch (Format)
	{
	case EGuidFormats::DigitsWithHyphens:
		return std::format("{:08X}-{:04X}-{:04X}-{:04X}-{:04X}{:08X}", A, B >> 16, B & 0xFFFF, C >> 16, C & 0xFFFF, D);

	//case EGuidFormats::DigitsWithHyphensInBraces:
	//	return std::format("{{:08X}-{:04X}-{:04X}-{:04X}-{:04X}{:08X}}", A, B >> 16, B & 0xFFFF, C >> 16, C & 0xFFFF, D);

	case EGuidFormats::DigitsWithHyphensInParentheses:
		return std::format("({:08X}-{:04X}-{:04X}-{:04X}-{:04X}{:08X})", A, B >> 16, B & 0xFFFF, C >> 16, C & 0xFFFF, D);

	//case EGuidFormats::HexValuesInBraces:
	//	return std::format("{0x{:08X},0x{:04X},0x{:04X},{0x{:02X},0x{:02X},0x{:02X},0x{:02X},0x{:02X},0x{:02X},0x{:02X},0x{:02X}}}", A, B >> 16, B & 0xFFFF, C >> 24, (C >> 16) & 0xFF, (C >> 8) & 0xFF, C & 0XFF, D >> 24, (D >> 16) & 0XFF, (D >> 8) & 0XFF, D & 0XFF);

	case EGuidFormats::UniqueObjectGuid:
		return std::format("{:08X}-{:08X}-{:08X}-{:08X}", A, B, C, D);

	default:
		return std::format("{:08X}{:08X}{:08X}{:08X}", A, B, C, D);
	}
}

FGuid FGuid::NewGuid()
{
    FGuid NewGuid{};
    if (CoCreateGuid((GUID*)&NewGuid) == S_FALSE)
        NewGuid = FGuid{};

    return NewGuid;
}
