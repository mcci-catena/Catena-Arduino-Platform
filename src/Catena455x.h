/* Catena455x.h	Mon Nov 26 2018 15:49:24 chwon */

/*

Module:  Catena455x.h

Function:
	class Catena455x: CatenaBase Platform to represent a Catena 455x
	(4550, 4551, etc.)

Version:
	V0.12.0	Mon Nov 26 2018 15:49:24 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2017-2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	ChaeHee Won, MCCI Corporation	October 2017

Revision history:
   0.6.0  Fri Oct 13 2017 15:19:30  chwon
	Module created.

   0.12.0  Mon Nov 26 2018 15:49:24  chwon
	Change from CatenaStm32L0LoRa to CatenaStm32L0 class.

*/

#ifndef _CATENA455X_H_		/* prevent multiple includes */
#define _CATENA455X_H_

#pragma once

#ifndef _CATENASTM32L0_H_
# include "CatenaStm32L0.h"
#endif

namespace McciCatena {

class Catena455x : public CatenaStm32L0
	{
public:
        using Super = CatenaStm32L0;

        // no specific constructor.
        Catena455x() {};

	// uses default destructor

	// neither copyable nor movable
	Catena455x(const Catena455x&) = delete;
	Catena455x& operator=(const Catena455x&) = delete;
	Catena455x(const Catena455x&&) = delete;
	Catena455x& operator=(const Catena455x&&) = delete;

	// LoRaWAN binding
	class LoRaWAN /* forward */;

	enum ANALOG_PINS
		{
		APIN_VBAT_SENSE = A3,
		APIN_VBUS_SENSE = A4,
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
	virtual float ReadVbat(void) const override;
	virtual float ReadVbus(void) const override;

        virtual const char *get_screwTerminalLabel(uint8_t iTerminal) const override
		{
		static const char jp3[] = "JP3";
		static const char jp4[] = "JP4";
		return iTerminal == 0 ? jp3 : iTerminal == 1 ? jp4 : nullptr;
		}

        virtual uint8_t get_screwTerminalPin2(uint8_t iTerminal) const override
		{
		return iTerminal == 0 ? D5 : iTerminal == 1 ? A1 : 0;
		}
        virtual uint8_t get_screwTerminalPin3(uint8_t iTerminal) const override
		{
		return iTerminal == 0 ? D12 : iTerminal == 1 ? A2 : 0;
		}

        virtual bool has_BME280() const override
		{ return true; }
        // virtual bool has_BME680() const override;
        // virtual bool has_HS3001() const override;
        // virtual bool has_SHT3x() const override;
        // virtual uint32_t enable_SHT3x(bool fStatus) override;
        // virtual bool get_SHT3xRequest() const override;
        // virtual bool has_SHT3xPowerControl() const override;
        // virtual bool has_SGPC3() const override;
        // virtual uint32_t enable_SGPC3(bool) override;
        // virtual bool get_SGPC3Request() const override;
        // virtual bool has_SGPC3PowerControl() const override;
        // virtual bool has_PMS7003() const override;
        // virtual uint32_t enable_PMS7003(bool fStatus) override;
        // virtual bool get_PMS7003Request() const override;
        // virtual bool has_PMS7003Control() const override;
        virtual bool has_Si1133() const override
		{ return true; }
        // virtual bool has_BH1750() const override;
        // virtual bool get_PMS7003Request() const override;
        // virtual uint32_t enable_tcxo(bool fStatus) override;
        // virtual bool get_tcxoRequest() const override;
        // virtual bool has_txcoControl() const override;
        virtual bool has_usbPort() const override
		{
		return true;
		}
        virtual bool get_consoleIsUsb() const override
		{
#ifdef USBCON
		return true;
#else
		return false;
#endif
		}
        virtual bool has_usbVbusMeasurementStandard() const override
		{ return true; }
        virtual uint8_t get_usbVbusMeasurementPin() const override
		{ return APIN_VBUS_SENSE; }

protected:
	cPowerControlDummy m_3v3;
	cPowerControlDummy m_flashVdd;
	cPowerControlDummy m_screwTerminal[2];
	cPowerControlDummy m_i2cVdd;
	cPowerControlDummy m_framVdd;
	cPowerControlDummy m_externalI2cBridgeVdd;
	cPowerControlDummy m_tcxo;

private:
	};

/*
|| The LoRaWAN class for the Catena 455x. Assumes The Things Network
*/
class Catena455x::LoRaWAN : public CatenaStm32L0::LoRaWAN
	{
public:
	using Super = CatenaStm32L0::LoRaWAN;

	/*
	|| the constructor. We don't do anything at this level, the
	|| Super constructor does most of the work.
	*/
	LoRaWAN() {};

	bool begin(Catena455x *pParent);

protected:

private:
	};

} // namespace McciCatena

/**** end of Catena455x.h ****/
#endif /* _CATENA455X_H_ */
