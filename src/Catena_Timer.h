/*

Module: Catena_Timer.h

Function:
    cTimer class definition

Copyright:
    See accompanying LICENSE file for copyright and license information.

Author:
    Terry Moore, MCCI Corporation   July 2019

*/

#ifndef _Catena_Timer_h_
# define _Catena_Timer_h_

#pragma once

#include <Arduino.h>
#include <Catena_PollableInterface.h>
#include <cstdint>

namespace McciCatena {

/****************************************************************************\
|
|   A simple timer -- this uses cPollableObject because it's easier
|
\****************************************************************************/

class cTimer : public McciCatena::cPollableObject
    {
public:
    // constructor
    cTimer() : m_registered(false)
             , m_running(false)
             {}

    // neither copyable nor movable
    cTimer(const cTimer&) = delete;
    cTimer& operator=(const cTimer&) = delete;
    cTimer(const cTimer&&) = delete;
    cTimer& operator=(const cTimer&&) = delete;

    // initialze to fire every nMillis
    bool begin(std::uint32_t nMillis);

    // stop operation
    void end();

    // poll function (updates data)
    virtual void poll() override;

    // return true if one or more events has occured; consumes ticks.
    bool isready();
    // return number of pending ticks, consuming them.
    std::uint32_t readTicks();
    // return number of pendign ticks, without consuming them.
    std::uint32_t peekTicks() const;
    // get the currrent tick interval.
    std::uint32_t getInterval() const
        { return this->m_interval; }
    std::uint32_t getRemaining() const;
    // change the tick interval. May cause immediate tick(s) if shortening
    // interval.
    std::uint32_t setInterval(std::uint32_t uInterval);
    // retrigger the timer, meaning that any active interval starts now.
    void retrigger();

    // display some info for debugging.
    void debugDisplay() const
        {
        Serial.print("time="); Serial.print(this->m_time);
        Serial.print(" interval="); Serial.print(this->m_interval);
        Serial.print(" events="); Serial.print(this->m_events);
        Serial.print(" overrun="); Serial.print(this->m_overrun);
        Serial.print(" registered="); Serial.print(this->m_registered);
        Serial.print(" running="); Serial.println(this->m_running);
        }

private:
    // millis() at last tick time.
    std::uint32_t   m_time;
    // millis() between ticks.
    std::uint32_t   m_interval;
    // number of unconsumed ticks (events)
    std::uint32_t   m_events;
    // count of ticks that were coalesced
    std::uint32_t   m_overrun;
    // true if clock is registered for polling.
    bool            m_registered;
    // true if clock is running.
    bool            m_running;
    };

} // namespace McciCatena

#endif  // ndef _Catena_Timer_h_
