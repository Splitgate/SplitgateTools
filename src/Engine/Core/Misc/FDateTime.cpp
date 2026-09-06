#include "FDateTime.h"
#include "Math/UnrealMathUtility.h"

#include "Strings/Strings.h"

#include <windows.h>

const int32 FDateTime::DaysPerMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const int32 FDateTime::DaysToMonth[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

FDateTime::FDateTime(int32 Year, int32 Month, int32 Day, int32 Hour, int32 Minute, int32 Second, int32 Millisecond)
{
	if (!Validate(Year, Month, Day, Hour, Minute, Second, Millisecond))
	{
		return;
	}

	int32 TotalDays = 0;

	if ((Month > 2) && IsLeapYear(Year))
	{
		++TotalDays;
	}

	--Year;											// the current year is not a full year yet
	--Month;										// the current month is not a full month yet

	TotalDays += Year * 365;
	TotalDays += Year / 4;							// leap year day every four years...
	TotalDays -= Year / 100;						// ...except every 100 years...
	TotalDays += Year / 400;						// ...but also every 400 years
	TotalDays += DaysToMonth[Month];				// days in this year up to last month
	TotalDays += Day - 1;							// days in this month minus today

	Ticks = TotalDays * ETimespan::TicksPerDay
		+ Hour * ETimespan::TicksPerHour
		+ Minute * ETimespan::TicksPerMinute
		+ Second * ETimespan::TicksPerSecond
		+ Millisecond * ETimespan::TicksPerMillisecond;
}

std::string FDateTime::ToString()
{
	return std::format("{:#04}.{:#02}.{:#02}-{:#02}:{:#02}:{:#02}", GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond());
}

int32 FDateTime::GetDay() const
{
	int32 Year, Month, Day;
	GetDate(Year, Month, Day);

	return Day;
}


EDayOfWeek FDateTime::GetDayOfWeek() const
{
	return static_cast<EDayOfWeek>((Ticks / ETimespan::TicksPerDay) % 7);
}

void FDateTime::GetDate(int32& OutYear, int32& OutMonth, int32& OutDay) const
{
	// Based on FORTRAN code in:
	// Fliegel, H. F. and van Flandern, T. C.,
	// Communications of the ACM, Vol. 11, No. 10 (October 1968).

	int32 i, j, k, l, n;

	l = FMath::FloorToInt((float)(GetJulianDay() + 0.5)) + 68569;
	n = 4 * l / 146097;
	l = l - (146097 * n + 3) / 4;
	i = 4000 * (l + 1) / 1461001;
	l = l - 1461 * i / 4 + 31;
	j = 80 * l / 2447;
	k = l - 2447 * j / 80;
	l = j / 11;
	j = j + 2 - 12 * l;
	i = 100 * (n - 49) + i + l;

	OutYear = i;
	OutMonth = j;
	OutDay = k;
}


int32 FDateTime::GetDayOfYear() const
{
	int32 Year, Month, Day;
	GetDate(Year, Month, Day);

	for (int32 CurrentMonth = 1; CurrentMonth < Month; ++CurrentMonth)
	{
		Day += DaysInMonth(Year, CurrentMonth);
	}

	return Day;
}


int32 FDateTime::GetHour12() const
{
	int32 Hour = GetHour();

	if (Hour < 1)
	{
		return 12;
	}

	if (Hour > 12)
	{
		return (Hour - 12);
	}

	return Hour;
}

int32 FDateTime::GetMonth() const
{
	int32 Year, Month, Day;
	GetDate(Year, Month, Day);

	return Month;
}


bool FDateTime::Parse(std::string DateTimeString, FDateTime& OutDateTime)
{
	std::string FixedString = Strings::Replace(DateTimeString, "-", " ");
	Strings::ReplaceInline(FixedString, ":", " ");
	Strings::ReplaceInline(FixedString, ".", " ");

	std::vector<std::string> Tokens = Strings::Split(FixedString, " ");

	// make sure it parsed it properly (within reason)
	if ((Tokens.size() < 6) || (Tokens.size() > 7))
	{
		return false;
	}

	const int32 Year = atoi(Tokens[0].c_str());
	const int32 Month = atoi(Tokens[1].c_str());
	const int32 Day = atoi(Tokens[2].c_str());
	const int32 Hour = atoi(Tokens[3].c_str());
	const int32 Minute = atoi(Tokens[4].c_str());
	const int32 Second = atoi(Tokens[5].c_str());
	const int32 Millisecond = Tokens.size() > 6 ? atoi(Tokens[6].c_str()) : 0;

	if (!Validate(Year, Month, Day, Hour, Minute, Second, Millisecond))
	{
		return false;
	}

	// convert the tokens to numbers
	OutDateTime.Ticks = FDateTime(Year, Month, Day, Hour, Minute, Second, Millisecond).Ticks;

	return true;
}

int32 FDateTime::GetYear() const
{
	int32 Year, Month, Day;
	GetDate(Year, Month, Day);

	return Year;
}
bool FDateTime::IsLeapYear(int32 Year)
{
	if ((Year % 4) == 0)
	{
		return (((Year % 100) != 0) || ((Year % 400) == 0));
	}

	return false;
}

int32 FDateTime::DaysInMonth(int32 Year, int32 Month)
{
	if (!((Month >= 1) && (Month <= 12)))
	{
		return 0;
	}

	if ((Month == 2) && IsLeapYear(Year))
	{
		return 29;
	}

	return DaysPerMonth[Month];
}

bool FDateTime::Validate(int32 Year, int32 Month, int32 Day, int32 Hour, int32 Minute, int32 Second, int32 Millisecond)
{
	return (Year >= 1) && (Year <= 9999) &&
		(Month >= 1) && (Month <= 12) &&
		(Day >= 1) && (Day <= DaysInMonth(Year, Month)) &&
		(Hour >= 0) && (Hour <= 23) &&
		(Minute >= 0) && (Minute <= 59) &&
		(Second >= 0) && (Second <= 59) &&
		(Millisecond >= 0) && (Millisecond <= 999);
}

FDateTime FDateTime::Now()
{
	int32 Year, Month, Day, DayOfWeek;
	int32 Hour, Minute, Second, Millisecond;

	SYSTEMTIME st;
	GetLocalTime(&st);

	Year = st.wYear;
	Month = st.wMonth;
	DayOfWeek = st.wDayOfWeek;
	Day = st.wDay;
	Hour = st.wHour;
	Minute = st.wMinute;
	Second = st.wSecond;
	Millisecond = st.wMilliseconds;

	return FDateTime(Year, Month, Day, Hour, Minute, Second, Millisecond);
}
