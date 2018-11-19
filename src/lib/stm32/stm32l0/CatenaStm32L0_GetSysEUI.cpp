/* CatenaStm32L0_GetSysEUI.cpp	Fri Oct 13 2017 15:19:30 chwon */

/*

Module:  CatenaStm32L0_GetSysEUI.cpp

Function:
	CatenaStm32L0::GetSysEUI()

Version:
	V0.6.0	Fri Oct 13 2017 15:19:30 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2017 by

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

*/

#ifdef ARDUINO_ARCH_STM32

#include "CatenaStm32L0.h"

using namespace McciCatena;

/*

Name:	CatenaStm32L0::GetSysEUI()

Function:
	Returns the SysEUI for this platform

Definition:
	const CatenaBase::EUI64_buffer_t *
		CatenaStm32L0::GetSysEUI(
			void
			);

Description:
	Looks in the FRAM and returns the value there.

Returns:
	Pointer to the SysEUI.

*/

/* public: virtual override */
const CatenaStm32L0::EUI64_buffer_t *
CatenaStm32L0::GetSysEUI(void)
	{
	if (! this->m_Fram.getField(cFramStorage::StandardKeys::kSysEUI,
			this->m_SysEUI
			))
		{
		/* not able to get the EUI. fill with zero */
		memset(	(uint8_t *) &this->m_SysEUI,
			0,
			sizeof(this->m_SysEUI)
			);
		}

	return &this->m_SysEUI;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_GetSysEUI.cpp ****/
