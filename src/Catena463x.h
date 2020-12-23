/*

Module:  Catena463x.h

Function:
        class Catena463x: CatenaBase Platform to represent a Catena 463x
        (such as the 4630).

Copyright notice:
        See accompanying LICENSE file.

Author:
        Dhinesh Kumar Pitchai, MCCI Corporation	April 2019

*/

#ifndef _CATENA463X_H_	/* prevent multiple includes */
#define _CATENA463X_H_

#pragma once

#ifndef _CATENASTM32L0_H_
# include "CatenaStm32L0.h"
#endif

namespace McciCatena {

class Catena463x : public CatenaStm32L0
	{
public:
        using Super = CatenaStm32L0;

        // no specific constructor.
        Catena463x() {};

	// uses default destructor

	// neither copyable nor movable
	Catena463x(const Catena463x&) = delete;
	Catena463x& operator=(const Catena463x&) = delete;
	Catena463x(const Catena463x&&) = delete;
	Catena463x& operator=(const Catena463x&&) = delete;

	// LoRaWAN binding
	class LoRaWAN /* forward */;

	// Sigfox binding
	class Sigfox /* forward */;

	enum ANALOG_PINS
		{
		APIN_VBAT_SENSE = A1,
		APIN_VBUS_SENSE = A2,
		};

	enum ANALOG_CHANNELS
		{
		ANALOG_CHANNEL_A0 = 0,
		ANALOG_CHANNEL_A1 = 5,
		ANALOG_CHANNEL_A2 = 4,
		ANALOG_CHANNEL_A3 = 3,
		ANALOG_CHANNEL_A4 = 2,
		ANALOG_CHANNEL_VBAT = ANALOG_CHANNEL_A1,
		ANALOG_CHANNEL_VBUS = ANALOG_CHANNEL_A2,
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
class Catena463x::LoRaWAN : public CatenaStm32L0::LoRaWAN
	{
public:
	using Super = CatenaStm32L0::LoRaWAN;

	/*
	|| the constructor. We don't do anything at this level, the
	|| Super constructor does most of the work.
	*/
	LoRaWAN() {};

	bool begin(Catena463x *pParent);

protected:

private:
	};

/*
|| The Sigfox class for the Catena 463x.
*/
class Catena463x::Sigfox : public CatenaStm32L0::Sigfox
	{
public:
	using Super = CatenaStm32L0::Sigfox;

	/*
	|| the constructor. We don't do anything at this level, the
	|| Super constructor does most of the work.
	*/
	Sigfox() {};

	bool begin(Catena463x *pParent);

protected:

private:
	};

} // namespace McciCatena

/**** end of Catena463x.h ****/
#endif /* _CATENA463X_H_ */
