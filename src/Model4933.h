/*

Module:  Model4933.h

Function:
        class Model4933: CatenaBase Platform to represent a Model 4933
        (such as the 4933).

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau R, MCCI Corporation	October 2023

*/

#ifndef _MODEL4933_H_	/* prevent multiple includes */
#define _MODEL4933_H_

#pragma once

#ifndef _CATENASTM32L0_H_
# include "CatenaStm32L0.h"
#endif

namespace McciCatena {

class Model4933 : public CatenaStm32L0
    {
public:
    using Super = CatenaStm32L0;

    // no specific constructor.
    Model4933() {};

    // uses default destructor

    // neither copyable nor movable
    Model4933(const Model4933&) = delete;
    Model4933& operator=(const Model4933&) = delete;
    Model4933(const Model4933&&) = delete;
    Model4933& operator=(const Model4933&&) = delete;

	virtual const char *CatenaName() const override { return "Model 4933"; };
	virtual float ReadVbat(void) const override;
	virtual float ReadVbus(void) const override;

    // LoRaWAN binding
    class LoRaWAN /* forward */;

    enum ANALOG_PINS
        {
        APIN_VBAT_SENSE = A1,
        APIN_VBUS_SENSE = A0,
        };

    enum ANALOG_CHANNELS
        {
        ANALOG_CHANNEL_A0 = 0,
        ANALOG_CHANNEL_A1 = 5,
        ANALOG_CHANNEL_A2 = 4,
        ANALOG_CHANNEL_A3 = 3,
        ANALOG_CHANNEL_A4 = 2,
        ANALOG_CHANNEL_VBAT = ANALOG_CHANNEL_A1,
	ANALOG_CHANNEL_VBUS = ANALOG_CHANNEL_A0,
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
// we are required to provide a table of platforms
virtual void getPlatformTable(
    const CATENA_PLATFORM * const * &vPlatforms,
    size_t &nvPlatforms
    ) override;

private:
    // the known platforms
    static const CATENA_PLATFORM(* const vPlatforms[]);
    static const size_t nvPlatforms;
    };

/*
|| The LoRaWAN class for the Model 4933. Assumes The Things Network
*/
class Model4933::LoRaWAN : public CatenaStm32L0::LoRaWAN
    {
public:
    using Super = CatenaStm32L0::LoRaWAN;

    /*
    || the constructor. We don't do anything at this level, the
    || Super constructor does most of the work.
    */
    LoRaWAN() {};

    bool begin(Model4933 *pParent);

protected:

private:
    };

} // namespace McciCatena

/**** end of Model4933.h ****/
#endif /* _MODEL4933_H_ */
