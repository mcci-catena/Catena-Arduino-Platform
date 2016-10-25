/* McciHwInfo.h	Sun Oct 16 2016 22:31:53 tmm */

/*

Module:  McciHwInfo.h

Function:
	Arduino library header for the hardware info database.

Version:
	V0.1.0	Sun Oct 16 2016 22:31:53 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2016 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	October 2016

Revision history:
   0.1.0  Sun Oct 16 2016 22:31:53  tmm
	Module created.

*/

#ifndef _MCCIHWINFO_H_		/* prevent multiple includes */
#define _MCCIHWINFO_H_

#ifndef _MCCIXDK_GUID_H_
# include "mcciadk_guid.h"
#endif

typedef struct MCCIHWINFO_KEY_s
	{
	MCCIXDK_GUID_WIRE	Key;
	} MCCIHWINFO_KEY;

/**** end of McciHwInfo.h ****/
#endif /* _MCCIHWINFO_H_ */
