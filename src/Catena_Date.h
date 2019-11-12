/*

Module: Catena_Date.h

Function:
    cDate class definition

Copyright:
    See accompanying LICENSE file for copyright and license information.

Author:
    Terry Moore, MCCI Corporation   July 2019, November 1989
    Charlie Mills, MCCI Corporation March 1996

*/

#ifndef _Catena_Date_h_
# define _Catena_Date_h_

#pragma once

#include <cstdint>
#include <Catena_limits.h>

namespace McciCatena {

/****************************************************************************\
|
|   The simple date object
|
\****************************************************************************/

class cDate
    {
public:
    // subtract from Common seconds to get equivalent gps seconds
    static constexpr std::int32_t kCommonToGpsSeconds = 315964800;
    using CommonTime_t = std::int64_t;
    using GpsTime_t = std::int64_t;

    // the invalid GPS time value. GPS epoch is after Common, so this defines limit of negative history
    static constexpr GpsTime_t kGpsTimeInvalidLow = std::numeric_limits<GpsTime_t>::lowest();
    static constexpr GpsTime_t kGpsTimeInvalid = kGpsTimeInvalidLow;
    static constexpr CommonTime_t kCommonTimeInvalidLow = CommonTime_t(kGpsTimeInvalidLow + kCommonToGpsSeconds);
    static constexpr CommonTime_t kCommonTimeInvalid = std::numeric_limits<CommonTime_t>::lowest();
    static constexpr CommonTime_t kCommonTimeInvalidHigh = cNumericLimits<CommonTime_t>::numeric_limits_max();
    static constexpr GpsTime_t kGpsTimeInvalidHigh = GpsTime_t(kCommonTimeInvalidHigh - kCommonToGpsSeconds);

    static constexpr bool isValidGpsTime(GpsTime_t t) { return t > kGpsTimeInvalidLow && t < kGpsTimeInvalidHigh; }
    static constexpr bool isValidCommonTime(CommonTime_t t) { return t > kCommonTimeInvalidLow && t < kCommonTimeInvalidHigh; }


    using Year_t = std::uint16_t;
    using Month_t = std::uint8_t;
    using Day_t = std::uint8_t;
    using Hour_t = std::uint8_t;
    using Minute_t = std::uint8_t;
    using Second_t = std::uint8_t;
    static constexpr Year_t kCommonYear = 1970;
    static constexpr Year_t kGpsYear = 1980;
    static constexpr Year_t kMinYear = 0;
    static constexpr Year_t kMaxYear = cNumericLimits<Year_t>::numeric_limits_max();

    // one-hot mask with 1 for zero-origin months with 31 days
    static constexpr unsigned kMonthsWith31Days =
        (1 << 0 | 1 << 2 | 1 << 4 | 1 << 6 | 1 << 7 | 1 << 9 | 1 << 11);

    // return days since day zero of the proleptic Gregorian astronomical calendar.
    // Year 0 is a leap year in this calendar.
    // Assumes positive years.
    static constexpr std::uint32_t daysSinceProlepticZero(Year_t y)
        {
        return y <= 0 ? 0
                      : 366 +
                        365 * (y-1) +
                        ((y - 1) / 4) -
                        (((y - 1) / 4) / 25) +
                        ((((y - 1) / 4) / 25) / 4);
        }

    static constexpr std::uint32_t kCommonDayZero() { return daysSinceProlepticZero(kCommonYear); }
    static constexpr std::uint32_t kGpsDayZero() { return daysSinceProlepticZero(kGpsYear) + 5; }

    // is y a leap year?
    static constexpr bool isLeapYear(Year_t y)
        {
        return (y % 4 != 0) ? false :
               (y % 100 != 0) ? true :
               (y % 400 == 0);
        }

    static constexpr GpsTime_t getGpsTime(CommonTime_t tCommon)
        {
        return isValidCommonTime(tCommon) ? tCommon - kCommonToGpsSeconds : kGpsTimeInvalid;
        }
    static constexpr GpsTime_t getCommonTime(GpsTime_t tGps)
        {
        return isValidGpsTime(tGps) ? tGps + kCommonToGpsSeconds : kCommonTimeInvalid;
        }

    static constexpr bool isValidYearMonthDay(Year_t year, Month_t month, Day_t day)
        {
        return (kMinYear <= year && year <= kMaxYear) &&
               (1 <= month && month <= 12) &&
               (1 <= day && day <= (month == 2 ? 28 + isLeapYear(year) :
                                   ((1 << (month - 1)) & kMonthsWith31Days) != 0 ? 31 :
                                   30));
        }

    static constexpr bool isValidHourMinuteSecond(Hour_t h, Minute_t m, Second_t s)
        {
        return (0 <= h && h <= 23) &&
               (0 <= m && m <= 59) &&
               (0 <= s && s <= 59);
        }

    // return day in year (0..364 or 0..365)
    static /* constexpr */ unsigned getDayInYear(Year_t year, Month_t month, Day_t day)
        {
        unsigned dayInYear = 0;

        if (month == 0 || month > 12 || day == 0)
            return 0;

        /* now, index into the year by the date */
        unsigned kStopMonth = 1 << (month - 1);
        for (unsigned maskMonth = 1 << 0; maskMonth < kStopMonth; maskMonth <<= 1)
            {
            if (maskMonth == 1 << 1)
                dayInYear += 28;
            else
                dayInYear += maskMonth & kMonthsWith31Days ? 31 : 30;
            }

        /* if March or later, and it's a leap year, add 1 */
        if (month >= 3 && isLeapYear(year))
            ++dayInYear;

        return dayInYear + day - 1;
        }

    // default constructor
    cDate() {}

    // set date portion of clock, return true if valid.
    bool setDate(Year_t y, Month_t m, Day_t d);

    // set date portion of clock by parsing ISO8601 string yyyy-mm-dd
    bool parseDateIso8601(const char *pDate, const char **ppEndPointer = nullptr);

    // set time portion of clock, return true if valid.
    bool setTime(Hour_t h, Minute_t m, Second_t s);

    // set time portion of clock by parsing ISO8601 string hh:mm:ss
    bool parseTime(const char *pTime, const char **ppEndPointer = nullptr);

    // return time according to Common epoch
    CommonTime_t getCommonTime() const;

    // return time according to GPS epoch
    GpsTime_t getGpsTime() const;

    // set time from interval from Common epoch
    bool setCommonTime(CommonTime_t t);

    // set time from interval from GPS epoch
    bool setGpsTime(GpsTime_t t);

    // get year (common era)
    Year_t year() const         { return this->m_year; }
    // get month [1..12]
    Month_t month() const       { return this->m_month; }
    // get day [1..31]
    Day_t day() const           { return this->m_day; }
    // get hour [0..23]
    Hour_t hour() const         { return this->m_hour; }
    // get minute [0..59]
    Minute_t minute() const     { return this->m_minute; }
    // get second [0..59]
    Second_t second() const     { return this->m_second; }

    // check validity
    bool isValid() const
        {
        return isValidYearMonthDay(this->m_year, this->m_month, this->m_day) &&
               isValidHourMinuteSecond(this->m_hour, this->m_minute, this->m_second);
        }

private:
    // the current year: in current era.
    Year_t      m_year;
    // month: 1..12
    Month_t     m_month;
    // day: 1..31 (limit based on month)
    Day_t       m_day;
    // hour: 0..23
    Hour_t      m_hour;
    // minute: 0..59
    Minute_t    m_minute;
    // second: 0..59
    Second_t    m_second;
    };

// check the leap-year calculations
static_assert(  cDate::isLeapYear(0),       "isLeapYear() error");
static_assert(! cDate::isLeapYear(1899),    "isLeapYear() error");
static_assert(! cDate::isLeapYear(1900),    "isLeapYear() error");
static_assert(! cDate::isLeapYear(1901),    "isLeapYear() error");
static_assert(! cDate::isLeapYear(1902),    "isLeapYear() error");
static_assert(! cDate::isLeapYear(1903),    "isLeapYear() error");
static_assert(  cDate::isLeapYear(1904),    "isLeapYear() error");
static_assert(  cDate::isLeapYear(2000),    "isLeapYear() error");

static_assert(cDate::daysSinceProlepticZero(0) == 0,
                                            "daysSinceProlepticZero() error");
static_assert(cDate::daysSinceProlepticZero(1) == 366,
                                            "daysSinceProlepticZero() error");
static_assert(cDate::daysSinceProlepticZero(4) == 4 * 365 + 1,
                                            "daysSinceProlepticZero() error");
static_assert(cDate::daysSinceProlepticZero(5) == 5 * 365 + 2,
                                            "daysSinceProlepticZero() error");
static_assert(cDate::daysSinceProlepticZero(99) == 99 * 365 + 99/4 + 1,
                                            "daysSinceProlepticZero() error");
static_assert(cDate::daysSinceProlepticZero(100) == 100 * 365 + 99/4 + 1,
                                            "daysSinceProlepticZero() error");
static_assert(cDate::daysSinceProlepticZero(101) == 101 * 365 + 100/4,
                                            "daysSinceProlepticZero() error");
static_assert(cDate::daysSinceProlepticZero(101) == 36890,
                                            "daysSinceProlepticZero() error");
static_assert(cDate::daysSinceProlepticZero(1901) == 694326,
                                            "daysSinceProlepticZero() error");
static_assert(cDate::daysSinceProlepticZero(2005) == 732312,
                                            "daysSinceProlepticZero() error");

} // namespace McciCatena



#endif /* ndef _Catena_Date_h_ */
