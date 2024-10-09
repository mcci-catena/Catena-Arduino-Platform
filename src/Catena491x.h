/*

Module:  Catena491x.h

Function:
        class Catena491x: CatenaBase Platform to represent a Catena 491x
        (such as the 4917).

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau R, MCCI Corporation	November 2022

*/

#ifndef _CATENA491X_H_	/* prevent multiple includes */
#define _CATENA491X_H_

#pragma once

#ifndef _CATENASTM32L0_H_
# include "CatenaStm32L0.h"
#endif

namespace McciCatena {

class Catena491x : public CatenaStm32L0
    {
public:
    using Super = CatenaStm32L0;

    // no specific constructor.
    Catena491x() {};

    // uses default destructor

    // neither copyable nor movable
    Catena491x(const Catena491x&) = delete;
    Catena491x& operator=(const Catena491x&) = delete;
    Catena491x(const Catena491x&&) = delete;
    Catena491x& operator=(const Catena491x&&) = delete;

    // LoRaWAN binding
    class LoRaWAN /* forward */;

    enum ANALOG_PINS
        {
        APIN_VBAT_SENSE = A3,
        APIN_VBUS_SENSE = A4,
        };

    enum ANALOG_CHANNELS
        {
        ANALOG_CHANNEL_A0 = 0,
        ANALOG_CHANNEL_A1 = 5,
        ANALOG_CHANNEL_A2 = 4,
        ANALOG_CHANNEL_A3 = 3,
        ANALOG_CHANNEL_A4 = 2,
        ANALOG_CHANNEL_VBAT = ANALOG_CHANNEL_A3,
        ANALOG_CHANNEL_VBUS = ANALOG_CHANNEL_A4,
        ANALOG_CHANNEL_VREF = 17,
        };

    enum DIGITAL_PINS
        {
        PIN_STATUS_LED = D13,
        PIN_SPI2_FLASH_SS = D19,
        PIN_SPI2_MOSI = D23,
        PIN_SPI2_MISO = D22,
        PIN_SPI2_SCK = D24,
        };

    // methods
    virtual bool begin() override;

protected:

private:
    };

/*
|| The LoRaWAN class for the Catena 455x. Assumes The Things Network
*/
class Catena491x::LoRaWAN : public CatenaStm32L0::LoRaWAN
    {
public:
    using Super = CatenaStm32L0::LoRaWAN;

    /*
    || the constructor. We don't do anything at this level, the
    || Super constructor does most of the work.
    */
    LoRaWAN() {};

    bool begin(Catena491x *pParent);

protected:

private:
    };

} // namespace McciCatena

/**** end of Catena491x.h ****/
#endif /* _CATENA491X_H_ */