/*

Module:  Catena.h

Function:
        class Catena: virtual CatenaBase derivative representing
        the target Catena for this build.

Copyright notice:
        See accompanying LICENSE file.

Author:
        ChaeHee Won, MCCI Corporation	October 2017

*/

#ifndef _CATENA_H_		/* prevent multiple includes */
#define _CATENA_H_

#pragma once

#if defined(MCCI_CATENA_4450)
# include "Catena4450.h"
# define CATENA_H_SUPER_  McciCatena::Catena4450
#elif defined(MCCI_CATENA_4460)
# include "Catena4460.h"
# define CATENA_H_SUPER_  McciCatena::Catena4460
#elif defined(MCCI_CATENA_4470)
# include "Catena4470.h"
# define CATENA_H_SUPER_  McciCatena::Catena4470
#elif defined(MCCI_CATENA_4410)
# include "Catena4410.h"
# define CATENA_H_SUPER_  McciCatena::Catena4410
#elif defined(MCCI_CATENA_4420)
# include "Catena4420.h"
# define CATENA_H_SUPER_  McciCatena::Catena4420
#elif defined(ARDUINO_SAMD_FEATHER_M0)
# include "CatenaFeatherM0.h"
# define CATENA_H_SUPER_  McciCatena::CatenaFeatherM0
#elif defined(ARDUINO_MCCI_CATENA_4551) || defined(ARDUINO_CATENA_4551)
# include "Catena4551.h"
# define CATENA_H_SUPER_  McciCatena::Catena4551
#elif defined(ARDUINO_MCCI_CATENA_4610)
# include "Catena4610.h"
# define CATENA_H_SUPER_  McciCatena::Catena4610
#elif defined(ARDUINO_MCCI_CATENA_4611) || defined(ARDUINO_CATENA_4611)
# include "Catena4611.h"
# define CATENA_H_SUPER_  McciCatena::Catena4611
#elif defined(ARDUINO_MCCI_CATENA_4612) || defined(ARDUINO_CATENA_4612)
# include "Catena4612.h"
# define CATENA_H_SUPER_  McciCatena::Catena4612
#elif defined(ARDUINO_MCCI_CATENA_4630)
# include "Catena4630.h"
# define CATENA_H_SUPER_  McciCatena::Catena4630
#elif defined(ARDUINO_MCCI_CATENA_4617)
# include "Catena4617.h"
# define CATENA_H_SUPER_  McciCatena::Catena4617
#elif defined(ARDUINO_MCCI_CATENA_4618)
# include "Catena4618.h"
# define CATENA_H_SUPER_  McciCatena::Catena4618
#elif defined(ARDUINO_MCCI_CATENA_4801) || defined(ARDUINO_CATENA_4801)
# include "Catena4801.h"
# define CATENA_H_SUPER_  McciCatena::Catena4801
#elif defined(ARDUINO_MCCI_CATENA_4802)
# include "Catena4802.h"
# define CATENA_H_SUPER_  McciCatena::Catena4802
/* fallback in case it's SAMD but not what we expect */
#elif defined(ARDUINO_ARCH_SAMD)
# include "CatenaSamd21.h"
# define CATENA_H_SUPER_ McciCatena::CatenaSamd21
#elif defined(ARDUINO_ARCH_STM32)
# include "Catena4551.h"
# define CATENA_H_SUPER_  McciCatena::Catena4551
#else
# include "CatenaBase.h"
# define CATENA_H_SUPER_  McciCatena::CatenaBase
#endif

namespace McciCatena {

///
/// \brief the top-level abstract Catena object
///
class Catena : public CATENA_H_SUPER_
	{
public:
	/// \brief the super class to be used depends on the targeted Catena.
        using Super = CATENA_H_SUPER_;

	///
	/// \brief the constructor sets the version, if one is supplied.
	///
	/// \param[in] appVersion, if supplied is an McciCatena::Version_t
	///	object which specifies the version of the application.
	///
        Catena(Version_t appVersion = Version_t(0))
		{
		this->setAppVersion(appVersion);
		}

	// neither copyable nor movable
	Catena(const Catena&) = delete;
	Catena& operator=(const Catena&) = delete;
	Catena(const Catena&&) = delete;
	Catena& operator=(const Catena&&) = delete;

        ///
        /// \brief implementation: set up everything on this particular board.
        ///
        /// \return \c true if setup was successful.
        ///
        virtual bool setup(void) override;

	virtual uint32_t enable_3v3Boost(bool fRequest) override
		{
		return this->m_3v3.enable(fRequest);
		}

	virtual bool get_3v3BoostRequest() const override
		{
		return this->m_3v3.getRequest();
		}

	virtual bool has_3v3BoostControl() const override
		{
		return this->m_3v3.hasControl();
		}

        uint8_t has_screwTerminals() const override
		{
		return sizeof(this->m_screwTerminal) / sizeof(this->m_screwTerminal[0]);
		}

        //virtual const char *get_screwTerminalLabel(uint8_t iTerminal) const override;
        //virtual uint8_t get_screwTerminalPin2(uint8_t iTerminal) const override;
        //virtual uint8_t get_screwTerminalPin3(uint8_t iTerminal) const override;

        virtual uint32_t enable_screwTerminalVdd(uint8_t iTerminal, bool fState) override
		{
		if (iTerminal < this->has_screwTerminals())
			return this->m_screwTerminal[iTerminal].enable(fState);
		else
			return 0;
		}
        virtual bool get_screwTerminalVddRequest(uint8_t iTerminal) override
		{
		if (iTerminal < this->has_screwTerminals())
			return this->m_screwTerminal[iTerminal].getRequest();
		else
			return false;
		}
        virtual bool has_screwTerminalVddControl(uint8_t iTerminal) override
		{
		if (iTerminal < this->has_screwTerminals())
			return this->m_screwTerminal[iTerminal].hasControl();
		else
			return false;
		}

        virtual uint32_t enable_i2cVdd(bool fState) override
		{
		return this->m_i2cVdd.enable(fState);
		}
        virtual bool has_i2cVddControl() const override
		{
		return this->m_i2cVdd.hasControl();
		}
        virtual bool get_i2cVddRequest() const override
		{
		return this->m_i2cVdd.getRequest();
		}

        virtual bool has_flash() const override;

        virtual cFlash *get_flash() const override;

        virtual SPIClass *get_flashBus() const override;

        virtual uint32_t enable_flashVdd(bool fState) override
		{
		return this->m_flashVdd.enable(fState);
		}
        virtual bool has_flashVddControl() const override
		{
		return this->m_flashVdd.hasControl();
		}
        virtual bool get_flashVddRequest() const override
		{
		return this->m_flashVdd.getRequest();
		}

        /// \brief determine whether flash was successfully probed
        bool get_flashFound() const
                {
                return this->m_flashFound;
                }

        /// \brief record whether flash was successfully probed.
        void set_flashFound(bool fFound)
                {
                this->m_flashFound = fFound;
                }

        virtual bool has_fram() const override;

	// virtual cFram *getFram(void) override;
        virtual uint32_t get_framSize() const override;
        virtual ::TwoWire *get_framBus() const override;

        virtual uint32_t enable_framVdd(bool fStatus) override
		{
		return this->m_framVdd.enable(fStatus);
		}
        virtual bool has_framVddControl() const override
		{
		return this->m_framVdd.hasControl();
		}
        virtual bool get_framVddRequest() const override
		{
		return this->m_framVdd.getRequest();
		}

	// virtual bool has_externalI2cBus() const override;
        // virtual uint32_t enable_externalI2cBridgeVdd(bool fStatus) override;
        // virtual bool get_externalI2cBridgeVddRequest() const override;
        // virtual bool has_externalI2cBridgeVddControl() const override;

	// sensors -- these go in the individual files.

        // virtual bool has_BME280() const override;
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
        // virtual bool has_Si1133() const override;
        // virtual bool has_BH1750() const override;
        // virtual bool get_PMS7003Request() const override;

protected:

private:
	/// \brief local routine to isolate the flash / download setup.
	bool setup_flash(void);
	};

} // namespace McciCatena

#undef CATENA_H_SUPER_

/**** end of Catena.h ****/
#endif /* _CATENA_H_ */
