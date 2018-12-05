/* CatenaBase_GetSysEUI.cpp	Mon Dec 03 2018 13:04:44 chwon */

/*

Module:  CatenaBase_GetSysEUI.cpp

Function:
	CatenaBase::GetSysEUI()

Version:
	V0.12.0	Mon Dec 03 2018 13:04:44 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	December 2018

Revision history:
   0.12.0  Mon Dec 03 2018 13:04:44  chwon
	Module created.

*/

#include "CatenaBase.h"

using namespace McciCatena;

/*

Name:	CatenaBase::GetSysEUI()

Function:
	Returns the SysEUI for this platform

Definition:
	const CatenaBase::EUI64_buffer_t *
		CatenaBase::GetSysEUI(
			void
			);

Description:
	Looks in the FRAM and returns the value there.

Returns:
	Pointer to the SysEUI.

*/

/* public: virtual override */
const CatenaBase::EUI64_buffer_t *
CatenaBase::GetSysEUI(void)
	{
	auto const pFram = this->getFram();

	if (pFram == nullptr ||
	    ! pFram->getField(cFramStorage::StandardKeys::kSysEUI,
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

/**** end of CatenaBase_GetSysEUI.cpp ****/
