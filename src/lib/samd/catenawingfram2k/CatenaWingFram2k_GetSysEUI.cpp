/* CatenaWingFram2k_GetSysEUI.cpp	Wed Apr 12 2017 13:05:46 tmm */

/*

Module:  CatenaWingFram2k_GetSysEUI.cpp

Function:
	CatenaWingFram2k::GetSysEUI()

Version:
	V0.5.0	Wed Apr 12 2017 13:05:46 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	April 2017

Revision history:
   0.5.0  Wed Apr 12 2017 13:05:46  tmm
	Module created.

*/

#ifdef ARDUINO_ARCH_SAMD

#include "CatenaWingFram2k.h"

using namespace McciCatena;

/*

Name:	CatenaWingFram2k::GetSysEUI()

Function:
	Returns the SysEUI for this platform

Definition:
	const CatenaBase::EUI64_buffer_t *
		CatenaWingFram2k::GetSysEUI(
			void
			);

Description:
	Looks in the FRAM and returns the value there.

Returns:
	Pointer to the SysEUI.

*/

/* public: virtual override */
const CatenaWingFram2k::EUI64_buffer_t *
CatenaWingFram2k::GetSysEUI(void)
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

#endif // ARDUINO_ARCH_SAMD
