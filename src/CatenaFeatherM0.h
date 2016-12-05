/* CatenaFeatherM0.h	Sun Dec  4 2016 19:57:13 tmm */

/*

Module:  CatenaFeatherM0.h

Function:
	Class CatenaFeatherM0

Version:
	V0.4.0	Sun Dec  4 2016 19:57:13 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2016 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Terry Moore, MCCI Corporation	December 2016

Revision history:
   0.4.0  Sun Dec  4 2016 19:57:13  tmm
	Module created.

*/

#ifndef _CATENAFEATHERM0_H_		/* prevent multiple includes */
#define _CATENAFEATHERM0_H_

#pragma once

#ifndef _CATENASAMD21_H_
# include "CatenaSamd21.h"
#endif

class CatenaFeatherM0 : public CatenaSamd21
	{
public:
	CatenaFeatherM0() {};
	~CatenaFeatherM0() {};

	// all FeatherM0s put vbat on A7
	enum ANALOG_PINS
		{
		APIN_VBAT_SENSE = A7,
		};

	enum DIGITAL_PINS
		{
		PIN_STATUS_LED = 13,
		};

	// read the current battery voltage, in engineering units
	float ReadVbat(void);

protected:

private:
	};

/**** end of CatenaFeatherM0.h ****/
#endif /* _CATENAFEATHERM0_H_ */
