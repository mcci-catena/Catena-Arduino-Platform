/*

Module:  Catena4801.h

Function:
	class Catena4801: CatenaBase Platform to represent a Catena 4801

Copyright notice:
	See accompanying license file.

Author:
	ChaeHee Won, MCCI Corporation	November 2018

*/

#ifndef _Catena4801_H_	/* prevent multiple includes */
#define _Catena4801_H_

#pragma once

#ifndef _CATENA480X_H_
# include "Catena480x.h"
#endif

namespace McciCatena {

class Catena4801 : public Catena480x
	{
public:
        using Super = Catena480x;

        // no specific constructor.
        Catena4801() {};

	// uses default destructor

	// neither copyable nor movable
	Catena4801(const Catena4801&) = delete;
	Catena4801& operator=(const Catena4801&) = delete;
	Catena4801(const Catena4801&&) = delete;
	Catena4801& operator=(const Catena4801&&) = delete;

	virtual const char *CatenaName() const override { return "Catena 4801"; };
	// virtual float ReadVbat(void) const override provided by 480x
	// virtual float ReadVbus(void) const override provided by 480x

	///
	/// \brief return screw terminal labels for 4801
	/// \param [in]	iTerminal the screw terminal of interest
	/// \returns "JP2" if iTerminal is zero, \c nullptr otherwise.
	///
	/// The 4801 has only one screw terminal, labeled JP2.
	///
        virtual const char *get_screwTerminalLabel(uint8_t iTerminal) const override
		{
		static const char jp2[] = "JP2";
		return iTerminal == 0 ? jp2 :  nullptr;
		}

	///
	/// \brief return screw terminal Arduino pin for pin 2 of given terminal, for 4801
	/// \param [in]	iTerminal the screw terminal of interest
	/// \returns \c A1 if iTerminal is zero, zero otherwise.
	///
	/// The 4801 has only one screw terminal, labeled JP2. JP2-2
	/// is wired to \c A1.
	///
        virtual uint8_t get_screwTerminalPin2(uint8_t iTerminal) const override
		{
		return iTerminal == 0 ? A1 : 0;
		}
	///
	/// \brief return screw terminal Arduino pin for pin 3 of given terminal, for 4801
	/// \param [in]	iTerminal the screw terminal of interest
	/// \returns \c A2 if iTerminal is zero, zero otherwise.
	///
	/// The 4801 has only one screw terminal, labeled JP2. JP2-3 is
	/// wired to \c A2
	///
        virtual uint8_t get_screwTerminalPin3(uint8_t iTerminal) const override
		{
		return iTerminal == 0 ? A2 : 0;
		}

        // virtual bool has_BME280() const override
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
	// virtual bool has_Si1133() const override
        // virtual bool has_BH1750() const override;
        // virtual bool get_PMS7003Request() const override;
        virtual uint32_t enable_tcxo(bool fStatus) override
		{ return this->m_tcxo.enable(fStatus); }
        virtual bool get_tcxoRequest() const override
		{ return this->m_tcxo.getRequest(); }
        virtual bool has_tcxoControl() const override
		{ return this->m_tcxo.hasControl(); }
        // virtual bool has_usbPort() const override
        // virtual bool get_consoleIsUsb() const override
        // virtual bool has_usbVbusMeasurementStandard() const override
        // virtual uint8_t get_usbVbusMeasurementPin() const override

protected:
	/// on the Catena 4801, VOUT1 is controlled by D10
	cPowerControlGPIO  m_Vout1 = cPowerControlGPIO{ D10, 10 };
	/// on the Catena 4802, VOUT2 is controlled by D11
	cPowerControlGPIO  m_Vout2 = cPowerControlGPIO{ D11, 10 };
	/// on the Catena 4801, the screw terminal has its own VOUT,
	/// but it's VOUT2.
	cPowerControlNested m_screwTerminal[1] = { cPowerControlNested{this->m_Vout2} };
	/// on the Catena 4801, there's no 3.3v regulator
	cPowerControlDummy m_3v3;
	/// on the Catena 4801, there's a TCXO controlled by D33
	cPowerControlGPIO m_tcxo = cPowerControlGPIO{ D33, 2 };
	/// on the Catena 4801, flashVdd is controlled by VOUT1
	cPowerControlNested m_flashVdd = cPowerControlNested{this->m_Vout1};
	/// on the Catena 4801, i2c is controlled by VOUT1
	cPowerControlNested m_i2cVdd = cPowerControlNested{this->m_Vout1};
	/// on the Catena 4801, FRAM is powered from VOUT1
	cPowerControlNested m_framVdd = cPowerControlNested{this->m_Vout1};
	/// on the Catena 4801, there's no external I2C bridge
	cPowerControlDummy m_externalI2cBridgeVdd;

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

} // namespace McciCatena

/**** end of Catena4801.h ****/
#endif /* _Catena4801_H_ */
