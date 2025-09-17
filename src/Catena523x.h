/*

Module:  Catena523x.h

Function:
        class Catena5230: CatenaBase Platform to represent a Catena 5230
        (such as the 5230).

Copyright notice:
        See accompanying LICENSE file.

Author:
        Murali, MCCI Corporation	Sep 2025

*/

#ifndef _CATENA523x_H_	/* prevent multiple includes */
#define _CATENA523x_H_

#pragma once

#ifndef _CATENASTM32L0_H_
# include "CatenaStm32L0.h"
#endif

namespace McciCatena {

class Catena523x : public CatenaStm32L0
	{
public:
        using Super = CatenaStm32L0;

        // no specific constructor.
        Catena523x() {};

	// uses default destructor

	// neither copyable nor movable
	Catena523x(const Catena523x&) = delete;
	Catena523x& operator=(const Catena523x&) = delete;
	Catena523x(const Catena523x&&) = delete;
	Catena523x& operator=(const Catena523x&&) = delete;

	// LoRaWAN binding
	class LoRaWAN /* forward */;

	// enum ANALOG_PINS
	// 	{
	// 	APIN_VBAT_SENSE = A3,
	// 	APIN_VBUS_SENSE = A4,
	// 	};

	enum ANALOG_CHANNELS
		{
		ANALOG_CHANNEL_A0 = 4,
		ANALOG_CHANNEL_A1 = 3,
		ANALOG_CHANNEL_A2 = 2,
		ANALOG_CHANNEL_A3 = 0,
		ANALOG_CHANNEL_A4 = 1,
		// ANALOG_CHANNEL_VBAT = ANALOG_CHANNEL_A3,
		// ANALOG_CHANNEL_VBUS = ANALOG_CHANNEL_A4,
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
class Catena523x::LoRaWAN : public CatenaStm32L0::LoRaWAN
	{
public:
	using Super = CatenaStm32L0::LoRaWAN;

	/*
	|| the constructor. We don't do anything at this level, the
	|| Super constructor does most of the work.
	*/
	LoRaWAN() {};

	bool begin(Catena523x *pParent);

protected:

private:
	};

} // namespace McciCatena

/**** end of Catena523x.h ****/
#endif /* _CATENA523x_H_ */
