/*

Module: Catena_Date.cpp

Function:
    cDate class implementation

Copyright:
    See accompanying LICENSE file for copyright and license information.

Author:
    Terry Moore, MCCI Corporation   July 2019, November 1989
    Charlie Mills, MCCI Corporation March 1996

*/

#include <Catena_Date.h>

#include <cstring>
#include <mcciadk_baselib.h>

using namespace McciCatena;

/****************************************************************************\
|
|	Read-only data.
|
\****************************************************************************/

// set date portion of clock, return true if valid.
bool cDate::setDate(
    cDate::Year_t y,
    cDate::Month_t m,
    cDate::Day_t d
    )
    {
    if (! isValidYearMonthDay(y, m, d))
        return false;

    this->m_year = y;
    this->m_month = m;
    this->m_day = d;
    return true;
    }

// set time portion of clock, return true if valid.
bool cDate::setTime(Hour_t h, Minute_t m, Second_t s)
    {
    if (! (0 <= h && h <= 23))
        return false;
    if (! (0 <= m && m <= 59))
        return false;
    if (! (0 <= s && s <= 59))
        return false;

    this->m_hour = h;
    this->m_minute = m;
    this->m_second = s;
    return true;
    }


// return time according to Common epoch
cDate::CommonTime_t
cDate::getCommonTime() const
    {
    return getCommonTime(this->getGpsTime());
    }
#if 0
    if (! this->isValid())
        return kCommonTimeInvalid;

    /*
    || calculate the number of days since proleptic year 0
    */
    std::int_fast32_t gregorianDay = std::int_fast32_t(daysSinceProlepticZero(this->m_year)) -
                                     std::int_fast32_t(daysSinceProlepticZero(kCommonYear));

    /* now, index into the year by the date */
    gregorianDay += getDayInYear(this->m_year, this->m_month, this->m_day);

    /*
    || Convert days to seconds, and add in the current time.
    || Do this in a way that allows most of the multiplications
    || to be done at reduced precision; CommonTime_t is 64-bit.
    */
    return this->m_second +
           60 * (this->m_minute + 60 * this->m_hour) +
           CommonTime_t(24 * 60 * 60) * gregorianDay;
    }
#endif

// return time according to GPS epoch
cDate::GpsTime_t cDate::getGpsTime() const
    {
    if (! this->isValid())
        return kGpsTimeInvalid;

    /*
    || calculate the number of days since proleptic year 0
    */
    std::int_fast32_t gregorianDay = std::int_fast32_t(daysSinceProlepticZero(this->m_year))
                                   - std::int_fast32_t(daysSinceProlepticZero(kGpsYear) + 5)
                                   ;

    /* now, index into the year by the date */
    gregorianDay += getDayInYear(this->m_year, this->m_month, this->m_day);

    /*
    || Convert days to seconds, and add in the current time.
    || Do this in a way that allows most of the multiplications
    || to be done at reduced precision; CommonTime_t is 64-bit.
    */
    return this->m_second +
           60 * (this->m_minute + 60 * this->m_hour) +
           CommonTime_t(24 * 60 * 60) * gregorianDay;
    }

// set time from interval referenced to Common epoch
bool cDate::setCommonTime(CommonTime_t t)
    {
    return this->setGpsTime(getGpsTime(t));
    }

// set time from interval referenced to GPS epoch
bool cDate::setGpsTime(GpsTime_t t)
    {
    if (! isValidGpsTime(t))
        return false;

    t += CommonTime_t(86400) * kGpsDayZero();

    this->m_second = t % 60;
    t /= 60;
    this->m_minute = t % 60;
    t /= 60;
    this->m_hour = t % 24;
    t /= 24;    // t is now in days.

    /*
    || t is now days since the epoch year. Convert to proleptic gregorian days.
    */
    std::int_fast32_t gregorianDay = int(t);

    /*
    || multiply by 365.2425 to get approximate gregorian year.
    */
    /* represent 365.2425 * 400: 146,097 */
    constexpr std::int_fast32_t k = 365 * 400 + 400 / 4 - 400 / 100 + 1;

    // Compute year, rounding. This is off by
    // +/- one over the range from 0 to 65535
    auto candidateGregorianYear = gregorianDay > 365 ? (((gregorianDay - 365) * 400 + 200) / k)
                                                     : 0;
    auto firstDayOfCandidateYear = daysSinceProlepticZero(candidateGregorianYear);
    if (firstDayOfCandidateYear > gregorianDay)
        {
        --candidateGregorianYear;
        firstDayOfCandidateYear = daysSinceProlepticZero(candidateGregorianYear);
        }
    else if ((gregorianDay - firstDayOfCandidateYear) < 365)
        /* use this year */;
    else
        {
        auto const firstDayNextYear = daysSinceProlepticZero(candidateGregorianYear + 1);
        // this will be false only for day 365 of a leap year
        if (firstDayNextYear <= gregorianDay)
            {
            firstDayOfCandidateYear = firstDayNextYear;
            ++candidateGregorianYear;
            }
        }

    // final checks
    if (! (kMinYear <= candidateGregorianYear && candidateGregorianYear <= kMaxYear))
        return false;

    // zero-origin day number:
    auto dayOfYear = gregorianDay - std::int_fast32_t(firstDayOfCandidateYear);

    if (dayOfYear < 0)
        return false;

    for (unsigned iMonth = 0; iMonth < 12; ++iMonth)
        {
        Day_t kDays;

        if (iMonth == 1)
            kDays = 28 + isLeapYear(candidateGregorianYear);
        else if ((kMonthsWith31Days & (1 << iMonth)) != 0)
            kDays = 31;
        else
            kDays = 30;

        if (dayOfYear < kDays)
            {
            this->m_year = Year_t(candidateGregorianYear);
            this->m_month = Month_t(iMonth + 1);
            this->m_day = Day_t(dayOfYear + 1);
            return true;
            }

        // otherwise... reduce by number of days this month
        dayOfYear -= kDays;
        }

    // if we get here, we goofed on the calculation.
    return false;
    }

/*

Name:   cDate::parseDateIso8601()

Function:
    Set date by parsing an ISO 8601 string.

Definition:
    bool cDate::parseDateIso8601(
        const char *pDate
        );

Description:
    pDate points to an ISO8601 date, in format yyyy-mm-dd.

    If a date can be parsed, it is extracted and the date
    of this object is updated accordingly.

Returns:
    true if date was extracted, parsed, and valid (and object
    was updated); false otherwise.

*/

bool cDate::parseDateIso8601(
    const char *pDate,
    const char **ppEndPtr
    )
    {
    const char *nonceEndPtr;

    if (ppEndPtr == nullptr)
        ppEndPtr = &nonceEndPtr;

    if (pDate == nullptr)
        {
        *ppEndPtr = nullptr;
        return false;
        }

    /* set up for parsing */
    size_t nDate = strlen(pDate);
    bool fOverflow;

    /* match year */
    std::uint32_t year;
    size_t nYear = McciAdkLib_BufferToUint32(pDate, nDate, 10, &year, &fOverflow);

    *ppEndPtr = pDate + nYear;

    if (nYear == 0 || fOverflow)
        return false;
    if (pDate[nYear] != '-')
        return false;
    if (nYear == 2)
        nYear += 2000;
    if (! (kMinYear <= year && year <= kMaxYear))
        return false;

    /* consume year and '-' */
    pDate += nYear + 1;
    nDate -= nYear + 1;

    /* match month */
    std::uint32_t month;
    size_t nMonth = McciAdkLib_BufferToUint32(pDate, nDate, 10, &month, &fOverflow);

    *ppEndPtr = pDate + nMonth;

    if (nMonth == 0 || nMonth > 2 || fOverflow)
        return false;

    if (pDate[nMonth] != '-')
        return false;

    pDate += nMonth + 1;
    nDate -= nMonth + 1;

    /* match day */
    std::uint32_t day;
    size_t nDay = McciAdkLib_BufferToUint32(pDate, nDate, 10, &day, &fOverflow);

    *ppEndPtr = pDate + nDay;

    if (nDay == 0 || nDay > 2 || fOverflow)
        return false;

    const char sepDay = pDate[nDay];
    if (! (sepDay == '\0' || sepDay == 't' || sepDay == 'T' || sepDay == ' '))
        return false;

    /* set the date */
    return this->setDate(Year_t(year), Month_t(month), Day_t(day));
    }

/*

Name:   cDate::parseTime()

Function:
    Set time by parsing a string.

Definition:
    bool cDate::parseTime(
        const char *pDate,
        const char **ppEndPtr
        );

Description:
    pDate points to an ISO8601 time, in format hh:mm:ss.

    If a time can be parsed, it is extracted and the date
    of this object is updated accordingly.

    The hour field may be one or two digits.

    The minute field must be two digits.

    The seconds field can be omitted. If present, it must
    be two digits. It may be terminated by end of string,
    by a '.' (indicating that fractional seconds
    follow).

    The time may be terminated by 'z' or 'Z', as a
    time-zone indicator. So the pattern is:

    ##:##(:##)?[.zZ]?

Returns:
    true if date was extracted, parsed, and valid (and object
    was updated); false otherwise.

    If ppEndPtr is not null, *ppEndPtr is set to a pointer
    to the first unconsumed character from pDate.

    The training '.' or 'z' is not consumed.q

*/

bool cDate::parseTime(
    const char *pDate,
    const char **ppEndPtr
    )
    {
    const char *nonceEndPtr;

    if (ppEndPtr == nullptr)
        ppEndPtr = &nonceEndPtr;

    if (pDate == nullptr)
        {
        *ppEndPtr = nullptr;
        return false;
        }

    /* set up for parsing */
    size_t nDate = strlen(pDate);
    bool fOverflow;

    /* match year */
    std::uint32_t hour;
    size_t nHour = McciAdkLib_BufferToUint32(pDate, nDate, 10, &hour, &fOverflow);

    *ppEndPtr = pDate + nHour;

    if (nHour == 0 || nHour > 2 || fOverflow || ! isValidHourMinuteSecond(hour, 0, 0))
        return false;
    if (pDate[nHour] != ':')
        return false;

    /* consume year and '-' */
    pDate += nHour + 1;
    nDate -= nHour + 1;

    /* match minute */
    std::uint32_t minute;
    size_t nMinute = McciAdkLib_BufferToUint32(pDate, nDate, 10, &minute, &fOverflow);

    *ppEndPtr = pDate + nMinute;

    if (nMinute != 2 || fOverflow || ! isValidHourMinuteSecond(hour, minute, 0))
        return false;

    char sep = pDate[nMinute];
    if (! (sep == ':' || sep == '\0' || sep == 'z' || sep == 'Z'))
        return false;

    pDate += nMinute;
    nDate -= nMinute;

    /* match second */
    std::uint32_t second;

    if (sep != ':')
        second = 0;
    else
        {
        ++pDate, --nDate;

        size_t nSecond = McciAdkLib_BufferToUint32(pDate, nDate, 10, &second, &fOverflow);

        *ppEndPtr = pDate + nSecond;

        if (nSecond != 2 || fOverflow)
            return false;

        sep = pDate[nSecond];
        if (! (sep == '\0' || sep == 'z' || sep == 'Z' || sep == '.'))
            return false;

        pDate += nSecond;
        nDate -= nSecond;
        }

    /* set the date */
    return this->setTime(Hour_t(hour), Minute_t(minute), Second_t(second));
    }
