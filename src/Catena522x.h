/*

Module:  Catena522x.h

Function:
        class Catena5220: CatenaBase Platform to represent a Catena 5220
        (such as the 5220).

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau Ravikumar, MCCI Corporation	August 2026

*/

#ifndef _CATENA522x_H_	/* prevent multiple includes */
#define _CATENA522x_H_

#pragma once

#ifndef _CATENASTM32L0_H_
# include "CatenaStm32L0.h"
#endif

namespace McciCatena {

class Catena522x : public CatenaStm32L0
	{
public:
        using Super = CatenaStm32L0;

        // no specific constructor.
        Catena522x() {};

	// uses default destructor

	// neither copyable nor movable
	Catena522x(const Catena522x&) = delete;
	Catena522x& operator=(const Catena522x&) = delete;
	Catena522x(const Catena522x&&) = delete;
	Catena522x& operator=(const Catena522x&&) = delete;

	// LoRaWAN binding
	class LoRaWAN /* forward */;

	enum ANALOG_PINS
		{
		APIN_VBAT_SENSE = A0,
		};

	enum ANALOG_CHANNELS
		{
		ANALOG_CHANNEL_A0 = 4,
		ANALOG_CHANNEL_A1 = 3,
		ANALOG_CHANNEL_A2 = 2,
		ANALOG_CHANNEL_A3 = 0,
		ANALOG_CHANNEL_A4 = 1,
		ANALOG_CHANNEL_VBAT = ANALOG_CHANNEL_A0,
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
|| The LoRaWAN class for the Catena 522x. Assumes The Things Network
*/
class Catena522x::LoRaWAN : public CatenaStm32L0::LoRaWAN
	{
public:
	using Super = CatenaStm32L0::LoRaWAN;

	/*
	|| the constructor. We don't do anything at this level, the
	|| Super constructor does most of the work.
	*/
	LoRaWAN() {};

	bool begin(Catena522x *pParent);

protected:

private:
	};

} // namespace McciCatena

/**** end of Catena522x.h ****/
#endif /* _CATENA522x_H_ */