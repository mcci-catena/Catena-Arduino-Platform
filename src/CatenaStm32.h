/* CatenaStm32.h	Wed Dec 05 2018 14:09:27 chwon */

/*

Module:  CatenaStm32.h

Function:
	class CatenaStm32

Version:
	V0.12.0	Wed Dec 05 2018 14:09:27 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2017-2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

Author:
	ChaeHee Won, MCCI Corporation	October 2017

Revision history:
   0.6.0  Fri Oct 13 2017 15:19:30  chwon
	Module created.

   0.7.0  Thu Dec 14 2017 14:55:06  tmm
	Refactor common code.

   0.12.0  Wed Dec 05 2018 14:09:27  chwon
	Move UniqueuID and CPUID_PLATFORM_MAP structure and related methods to
	CatenaBase class.

*/

#ifndef _CATENASTM32_H_		/* prevent multiple includes */
#define _CATENASTM32_H_

#pragma once

#ifndef _CATENABASE_H_
# include "CatenaBase.h"
#endif

namespace McciCatena {

/* the class for Stm32-based Catenas */
class CatenaStm32 : public CatenaBase
	{
public:
        using Super = CatenaBase;

	/*
	|| Methods
	*/

        // start the Stm32 level
	virtual bool begin(void);
	bool begin(uint32_t uOverrideMask);
	bool begin(uint32_t uClearMask, uint32_t uSetMask);

	virtual void GetUniqueID(
		UniqueID_buffer_t *pIdBuffer
		) override;

protected:
	virtual void registerCommands(void);

	/*
	|| Class-level information
	*/
private:
	};

} // namespace McciCatena

/**** end of CatenaStm32.h ****/
#endif /* _CATENASTM32_H_ */
