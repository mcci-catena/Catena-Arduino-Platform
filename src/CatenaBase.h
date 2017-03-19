/* CatenaBase.h	Thu Oct 27 2016 22:46:30 tmm */

/*

Module:  CatenaBase.h

Function:
	class CatenaBase interfaces.

Version:
	V0.3.0	Thu Oct 27 2016 22:46:30 tmm	Edit level 3

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

   0.3.0  Thu Oct 27 2016 22:46:30  tmm
	Change buffer types away from array, types are just not intuitive.

*/

#ifndef _CATENABASE_H_		/* prevent multiple includes */
#define _CATENABASE_H_

#pragma once

#ifndef _CATENA_POLLABLEINTERFACE_H_
# include <Catena_PollableInterface.h>
#endif

#include <stdint.h>
#include <Arduino.h>

class CatenaBase : public McciCatena::cPollableInterface
	{
public:
	/* an EUI64 */
	struct EUI64_buffer_t {
		uint8_t	b[64/8];
		};
	struct EUI64_string_t
		{
		char	c[sizeof(EUI64_buffer_t) * 3 + 1];
		};

	enum OPERATING_FLAGS : uint32_t
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

        // a placeholder for the protocol.
        virtual void poll(void) {};

protected:
	EUI64_buffer_t m_SysEUI;
	};

/**** end of CatenaBase.h ****/
#endif /* _CATENABASE_H_ */
