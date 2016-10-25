/* mcciadk_baselib.h	Tue Oct 25 2016 08:49:01 tmm */

/*

Module:  mcciadk_baselib.h

Function:
	The basic ADK library.

Version:
	V0.1.0	Tue Oct 25 2016 08:49:01 tmm	Edit level 1

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
   0.1.0  Tue Oct 25 2016 08:49:01  tmm
	Module created.

*/

#ifndef _MCCIADK_BASELIB_H_		/* prevent multiple includes */
#define _MCCIADK_BASELIB_H_

#ifndef _MCCIADK_ENV_H_
# include <mcciadk_env.h>
#endif

MCCIADK_BEGIN_DECLS

const char *
McciAdkLib_MultiSzIndex(
	const char * pmultiszStrings,
	unsigned uIndex
	);

MCCIADK_END_DECLS

/**** end of mcciadk_baselib.h ****/
#endif /* _MCCIADK_BASELIB_H_ */
