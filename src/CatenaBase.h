/* CatenaBase.h	Tue Oct 25 2016 01:58:25 tmm */

/*

Module:  CatenaBase.h

Function:
	class CatenaBase interfaces.

Version:
	V0.1.0	Tue Oct 25 2016 01:58:25 tmm	Edit level 1

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
   0.1.0  Tue Oct 25 2016 01:58:25  tmm
	Module created.

*/

#ifndef _CATENABASE_H_		/* prevent multiple includes */
#define _CATENABASE_H_

#include <stdint.h>
#include <Arduino.h>

class CatenaBase
	{
public:
	/* an EUI64 */
	typedef	uint8_t	EUI64_buffer_t[64/8];
	typedef char EUI64_string_t[
			sizeof(EUI64_buffer_t) * 3 + 1
			];

	enum class OPERATING_FLAGS : uint32_t
		{
		fUnattended = 1 << 0,
		};

	void SafePrintf(
		const char *fmt, ...
		);

	const inline EUI64_buffer_t *GetSysEUI(void)
		{
		return &this->m_SysEUI;
		}

protected:
	EUI64_buffer_t m_SysEUI;
	};

/**** end of CatenaBase.h ****/
#endif /* _CATENABASE_H_ */
