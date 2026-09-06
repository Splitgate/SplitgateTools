#pragma once

#include "CoreTypes.h"
#include <string>

enum class EDayOfWeek
{
	Monday = 0,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday,
	Sunday
};

enum class EMonthOfYear
{
	January = 1,
	February,
	March,
	April,
	May,
	June,
	July,
	August,
	September,
	October,
	November,
	December
};

namespace ETimespan
{
	/** The maximum number of ticks that can be represented in FTimespan. */
	constexpr int64 MaxTicks = 9223372036854775807;

	/** The minimum number of ticks that can be represented in FTimespan. */
	constexpr int64 MinTicks = -9223372036854775807 - 1;

	/** The number of nanoseconds per tick. */
	constexpr int64 NanosecondsPerTick = 100;

	/** The number of timespan ticks per day. */
	constexpr int64 TicksPerDay = 864000000000;

	/** The number of timespan ticks per hour. */
	constexpr int64 TicksPerHour = 36000000000;

	/** The number of timespan ticks per microsecond. */
	constexpr int64 TicksPerMicrosecond = 10;

	/** The number of timespan ticks per millisecond. */
	constexpr int64 TicksPerMillisecond = 10000;

	/** The number of timespan ticks per minute. */
	constexpr int64 TicksPerMinute = 600000000;

	/** The number of timespan ticks per second. */
	constexpr int64 TicksPerSecond = 10000000;

	/** The number of timespan ticks per week. */
	constexpr int64 TicksPerWeek = 6048000000000;

	/** The number of timespan ticks per year (365 days, not accounting for leap years). */
	constexpr int64 TicksPerYear = 365 * TicksPerDay;
};

struct FDateTime
{
public:

	FDateTime() {}

	FDateTime(int64 InTicks)
		: Ticks(InTicks)
	{
	}

	FDateTime(int32 Year, int32 Month, int32 Day, int32 Hour = 0, int32 Minute = 0, int32 Second = 0, int32 Millisecond = 0);
	
	std::string ToString();

	int32 GetDay() const;

	EDayOfWeek GetDayOfWeek() const;

	FDateTime GetDate() const
	{
		return FDateTime(Ticks - (Ticks % ETimespan::TicksPerDay));
	}

	void GetDate(int32& OutYear, int32& OutMonth, int32& OutDay) const;

	int32 GetDayOfYear() const;

	int32 GetHour() const
	{
		return (int32)((Ticks / ETimespan::TicksPerHour) % 24);
	}

	int32 GetMillisecond() const
	{
		return (int32)((Ticks / ETimespan::TicksPerMillisecond) % 1000);
	}

	int32 GetMinute() const
	{
		return (int32)((Ticks / ETimespan::TicksPerMinute) % 60);
	}

	int32 GetHour12() const;

	int32 GetMonth() const;

	EMonthOfYear GetMonthOfYear() const
	{
		return static_cast<EMonthOfYear>(GetMonth());
	}

	int32 GetSecond() const
	{
		return (int32)((Ticks / ETimespan::TicksPerSecond) % 60);
	}

	int64 GetTicks() const
	{
		return Ticks;
	}

	double GetJulianDay() const
	{
		return (double)(1721425.5 + Ticks / ETimespan::TicksPerDay);
	}

	static bool Parse(std::string DateTimeString, FDateTime& OutDateTime);

	int32 GetYear() const;

	static bool IsLeapYear(int32 Year);

	static int32 DaysInMonth(int32 Year, int32 Month);

	static bool Validate(int32 Year, int32 Month, int32 Day, int32 Hour, int32 Minute, int32 Second, int32 Millisecond);

	static FDateTime Now();

protected:

	/** Holds the days per month in a non-leap year. */
	static const int32 DaysPerMonth[];

	/** Holds the cumulative days per month in a non-leap year. */
	static const int32 DaysToMonth[];

private:

	/** Holds the ticks in 100 nanoseconds resolution since January 1, 0001 A.D. */
	int64 Ticks;
};
