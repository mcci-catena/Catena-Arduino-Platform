/* CatenaSamd21.h	Wed Dec 05 2018 14:07:26 chwon */

/*

Module:  CatenaSamd21.h

Function:
	class CatenaSamd21

Version:
	V0.12.0	Wed Dec 05 2018 14:07:26 chwon	Edit level 6

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

   0.12.0  Wed Dec 05 2018 14:07:26  chwon
	Move UniqueuID and CPUID_PLATFORM_MAP structure and related methods to
	CatenaBase class.

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

	CatenaRTC::CalendarTime GetCalendarTime(void)
		{
		return this->m_Rtc.GetTime();
		}

        bool SetCalendarTime(const CatenaRTC::CalendarTime &calendarTime)
                {
                return this->m_Rtc.SetTime(&calendarTime);
                }

protected:
	virtual void registerCommands(void);

	/*
	|| Class-level information
	*/
private:
	CatenaRTC	m_Rtc;
	};

} /* namespace McciCatena */

/**** end of CatenaSamd21.h ****/
#endif /* _CATENASAMD21_H_ */
