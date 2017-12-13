/* Catena455x_GetSysEUI.cpp	Fri Oct 13 2017 15:19:30 chwon */

/*

Module:  Catena455x_GetSysEUI.cpp

Function:
	Catena455x::GetSysEUI()

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

#include "Catena455x.h"

using namespace McciCatena;

/*

Name:	Catena455x::GetSysEUI()

Function:
	Returns the SysEUI for this platform

Definition:
	const CatenaBase::EUI64_buffer_t *
		Catena455x::GetSysEUI(
			void
			);

Description:
	Looks in the FRAM and returns the value there.

Returns:
	Pointer to the SysEUI.

*/

/* public: virtual override */
const Catena455x::EUI64_buffer_t *
Catena455x::GetSysEUI(void)
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

/**** end of Catena455x_GetSysEUI.cpp ****/
