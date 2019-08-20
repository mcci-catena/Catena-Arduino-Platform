/*

Module: Catena_Timer.cpp

Function:
    Catena cTimer methods

Copyright:
    See accompanying LICENSE file for copyright and license information.

Author:
    Terry Moore, MCCI Corporation   July 2019

*/

#include "Catena_Timer.h"

#include <Catena.h>

extern McciCatena::Catena gCatena;
using namespace McciCatena;

/****************************************************************************\
|
|   A simple timer -- this uses cPollableObject because it's easier
|
\****************************************************************************/

bool cTimer::begin(std::uint32_t nMillis)
    {
    this->m_interval = nMillis;
    this->m_time = millis();
    this->m_events = 0;

    // set up for polling.
    if (! this->m_registered)
        {
        gCatena.registerObject(this);
        this->m_registered = true;
        }

    this->m_running = true;
    return true;
    }

void cTimer::end()
    {
    this->m_running = false;
    }

std::uint32_t cTimer::setInterval(
    std::uint32_t uInterval
    )
    {
    // this->m_time is the last trigger time. We change the
    // time, and then poll the timer to adjust ticks -- so
    // if you set the time shorter than current, you may get
    // immediate ticks.
    auto const uResult = this->m_interval;
    this->m_interval = uInterval;
    this->poll();
    return uResult;
    }

void cTimer::retrigger()
    {
    if (this->m_running)
        this->m_time = millis();
    }

void cTimer::poll() /* override */
    {
    if (! this->m_running)
        return;

    auto const tNow = millis();

    if (tNow - this->m_time >= this->m_interval)
        {
        this->m_time += this->m_interval;
        ++this->m_events;

        /* if this->m_time is now in the future, we're done */
        if (std::int32_t(tNow - this->m_time) < std::int32_t(this->m_interval))
            return;

        // rarely, we need to do arithmetic. time and events are in sync.
        // arrange for m_time to be greater than tNow, and adjust m_events
        // accordingly.
        std::uint32_t const tDiff = tNow - this->m_time;
        std::uint32_t const nTicks = tDiff / this->m_interval;
        this->m_events += nTicks;
        this->m_time += nTicks * this->m_interval;
        this->m_overrun += nTicks;
        }
    }

bool cTimer::isready()
    {
    return this->readTicks() != 0;
    }

std::uint32_t cTimer::readTicks()
    {
    auto const result = this->m_events;
    this->m_events = 0;
    return result;
    }

std::uint32_t cTimer::peekTicks() const
    {
    return this->m_events;
    }

std::uint32_t cTimer::getRemaining() const
    {
    auto const tNow = millis();

    auto const elapsed = tNow - this->m_time;

    if (elapsed > this->m_interval)
        return 0;
    else
        return this->m_interval - elapsed;
    }
