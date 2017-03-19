/* Catena_PollableInterface.h	Sat Mar 18 2017 14:48:29 tmm */

/*

Module:  Catena_PollableInterface.h

Function:
	McciCatena::cPollableInterface

Version:
	V0.5.0	Sat Mar 18 2017 14:48:29 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	March 2017

Revision history:
   0.5.0  Sat Mar 18 2017 14:48:29  tmm
	Module created.

*/

#ifndef _CATENA_POLLABLEINTERFACE_H_		/* prevent multiple includes */
#define _CATENA_POLLABLEINTERFACE_H_

#pragma once

namespace McciCatena {

class cPollableInterface
	{
public:
	// empty virtual detructor
	virtual ~cPollableInterface() {};

	// override this, please:
	virtual void poll(void) = 0;
	};

}; // namespace McciCatena

/**** end of Catena_PollableInterface.h ****/
#endif /* _CATENA_POLLABLEINTERFACE_H_ */
