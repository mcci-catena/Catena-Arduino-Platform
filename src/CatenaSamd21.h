/* CatenaSamd21.h	Wed Nov 21 2018 13:32:28 chwon */

/*

Module:  CatenaSamd21.h

Function:
	class CatenaSamd21

Version:
	V0.11.0	Wed Nov 21 2018 13:32:28 chwon	Edit level 5

Copyright notice:
	This file copyright (C) 2016-2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Terry Moore, MCCI Corporation	October 2016

Revision history:
   0.1.0  Tue Oct 25 2016 02:00:28  tmm
	Module created.

   0.3.0  Thu Oct 27 2016 22:53:12  tmm
	Eliminate array types.

   0.4.0  Wed Mar  8 2017 17:29:54  tmm
	Add flags for new well-known sensors and mod-number provisions.

   0.5.0  Sun Mar 19 2017 15:49:23  tmm
	Refactor to add support for USB command-line processing.

   0.11.0  Wed Nov 21 2018 13:32:28  chwon
	Add Sleep() method and add CatenaRTC class.

*/

#ifndef _CATENASAMD21_H_		/* prevent multiple includes */
#define _CATENASAMD21_H_

#pragma once

#ifndef _CATENABASE_H_
# include "CatenaBase.h"
#endif

#ifndef _CATENARTC_H_
# include "CatenaRTC.h"
#endif

namespace McciCatena {

/* the class for Samd21-based Catenas */
class CatenaSamd21 : public CatenaBase
	{
public:	
        using Super = CatenaBase;

	/*
	|| Methods
	*/

        // start the Samd21 level
	virtual bool begin(void);
	bool begin(uint32_t uOverrideMask);
	bool begin(uint32_t uClearMask, uint32_t uSetMask);

	virtual void GetUniqueID(
		UniqueID_buffer_t *pIdBuffer
		) override;

	virtual void Sleep(uint32_t howLongInSeconds) override;

protected:
	virtual void registerCommands(void);

	// help for the command-processing framework.
        class cSerialReady : public McciCatena::cStreamLineCollector::cStreamReady
	        {
        public:
		// return true if Serial is ready. Overridden because
		// the Arduino !!Serial() delays 10ms unconditionally!
                // so we need special business.
	        virtual bool isReady() const override;
	        };


        // the callback object to use for commands (since we're on USB)
        cSerialReady                            m_SerialReady;

	/*
	|| Class-level information
	*/
private:
	static const CPUID_PLATFORM_MAP vCpuIdToPlatform[];
	static const size_t nvCpuIdToPlatform;
	CatenaRTC		m_Rtc;
	};

} /* namespace McciCatena */

/**** end of CatenaSamd21.h ****/
#endif /* _CATENASAMD21_H_ */
