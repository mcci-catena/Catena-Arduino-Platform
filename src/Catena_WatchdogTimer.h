/*

Module:  Catena_WatchdogTimer.h

Function:
        class Catena_WatchdogTimer

Copyright notice and License:
        See LICENSE file accompanying this project.

Author:
        Pranau R, MCCI Corporation	April 2024

*/

#ifndef _CATENA_WATCHDOGTIMER_H_		/* prevent multiple includes */
#define _CATENA_WATCHDOGTIMER_H_

#pragma once

#include <Arduino.h>

namespace McciCatena {

class cWatchdogTimer
    {
public:
    enum : uint32_t
        {
        IWDG_KEY_ENABLE                 = 0xCCCC,
        IWDG_KEY_WRITE_ACCESS_ENABLE    = 0x5555,
        IWDG_KEY_WRITE_ACCESS_DISABLE   = 0,
        IWDG_KEY_REFRESH                = 0xAAAA,
        };

    cWatchdogTimer() {};

    // uses default destructor

    // neither copyable nor movable
    cWatchdogTimer(const cWatchdogTimer&) = delete;
    cWatchdogTimer& operator=(const cWatchdogTimer&) = delete;
    cWatchdogTimer(const cWatchdogTimer&&) = delete;
    cWatchdogTimer& operator=(const cWatchdogTimer&&) = delete;

    static constexpr uint32_t kWatchdogSeconds = 26;

    /// @brief setup STM32Lxx IWDG to require 20 second updates.
    void setupWatchdog();

    /// @brief get another 20 seconds before the watchdog triggers
    void refreshWatchdog();

    /// @brief delay for a while, and refresh the watchdog.
    /// @param millis number of milliseconds, as for \c ::delay().
    void safeDelay(uint32_t millis);
    };

extern cWatchdogTimer gIwdgTimer;

} // namespace McciCatena

/**** end of Catena_WatchdogTimer.h ****/
#endif /* _CATENA_WATCHDOGTIMER_H_ */