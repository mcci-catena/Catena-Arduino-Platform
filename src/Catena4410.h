/* Catena4410.h	Sat Oct 15 2016 22:15:02 tmm */

/*

Module:  Catena4410.h

Function:
	Arduino library header for Catena 4410

Version:
	V0.1.0	Sat Oct 15 2016 22:15:02 tmm	Edit level 1

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
   1.00a  Sat Oct 15 2016 22:12:56  tmm
	Module created.

*/

#ifndef _CATENA4410_H_		/* prevent multiple includes */
#define _CATENA4410_H_

#include <stdint.h>

class Catena4410
	{
public:
	typedef	uint8_t	UniqueID_buffer_t[128/8];

	Catena4410();
	void GetUniqueID(
		UniqueID_buffer_t pIdBuffer
		);
	void SafePrintf(
		const char *fmt, ...
		);

private:
	};



/**** end of Catena4410.h ****/
#endif /* _CATENA4410_H_ */
