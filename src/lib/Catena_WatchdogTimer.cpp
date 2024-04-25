/*

Module:  Catena_WatchdogTimer.cpp

Function:
        Definition for Watchdog timer.

Copyright notice and License:
        See LICENSE file accompanying this project.

Author:
        Pranau R, MCCI Corporation	April 2024

*/

#include <Catena_WatchdogTimer.h>
#include <Catena_Log.h>

using namespace McciCatena;

// the global instance
cWatchdogTimer McciCatena::gIwdgTimer;

/*

Name:	cWatchdogTimer::setupWatchdog()

Function:
        Setting up a Watchdog Timer.

Definition:
        void cWatchdogTimer::setupWatchdog(
                uint32_t watchdogSeconds
                );

Description:
        This function initializes the watchdog timer.

Returns:
        returns nothing.

*/

void cWatchdogTimer::setupWatchdog()
    {
    // compute number of ticks to set in the reload register.
    // 40000 is the rough RC frequency of the watchdog, 256 is the pre-scaler.
    constexpr uint32_t knTicks = (this->kWatchdogSeconds * 40000) / 256;
    static_assert(knTicks <= 0xFFF, "knTicks must fit in a 12-bit field");

    // enable the IWDG
    IWDG->KR = IWDG_KEY_ENABLE;
    // allow write access.
    IWDG->KR = IWDG_KEY_WRITE_ACCESS_ENABLE;
    // set prescaler to 7, i.e., divide by 256. So one tick == 40000 Hz/256 == 156.25 Hz == 6.4ms
    IWDG->PR = 7;
    // set reload register.
    IWDG->RLR = knTicks;
    // wait for the register to update. Since we're initializing, we don't
    // really care very much.
    for (uint32_t tNow = millis(); millis() - tNow < 48;)
        {
        if (IWDG->SR == 0)
            break;
        }
    if (IWDG->SR != 0)
        {
        gLog.printf(gLog.kError, "watchdog setup failed!: %x\n", IWDG->SR);
        }

    // refresh the watchdog
    this->refreshWatchdog();
    }

/*

Name:	cWatchdogTimer::refreshWatchdog()

Function:
        Refreshes the Watchdog Timer.

Definition:
        void cWatchdogTimer::refreshWatchdog(
                void
                );

Description:
        This function refreshes the watchdog timer.

Returns:
        returns nothing.

*/

void cWatchdogTimer::refreshWatchdog()
    {
    IWDG->KR = IWDG_KEY_REFRESH;
    }

/*

Name:	cWatchdogTimer::safeDelay()

Function:
        Delay without hang using Watchdog Timer.

Definition:
        void cWatchdogTimer::safeDelay(
                void
                );

Description:
        This function is a wrapper function for Arduino delay() function the watchdog timer.
        It safely delay the application without the application getting hang.

Returns:
        returns nothing.

*/

void cWatchdogTimer::safeDelay(uint32_t millis)
    {
    this->refreshWatchdog();
    delay(millis);
    this->refreshWatchdog();
    }