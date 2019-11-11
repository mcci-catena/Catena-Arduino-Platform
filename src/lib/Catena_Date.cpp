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

#include "../Catena_Date.h"

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

