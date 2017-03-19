/* Catena_Completion.h	Sat Mar 18 2017 16:34:53 tmm */

/*

Module:  Catena_Completion.h

Function:
	class McciCatena::cCompletion

Version:
	V0.5.0	Sat Mar 18 2017 16:34:53 tmm	Edit level 1

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
   0.5.0  Sat Mar 18 2017 16:34:53  tmm
	Module created.

*/

#ifndef _CATENA_COMPLETION_H_		/* prevent multiple includes */
#define _CATENA_COMPLETION_H_

#pragma once

namespace McciCatena {

// a completion is an abstract class; pass a pointer to this to
// any of the event processors, and it will get dispatched. Note that
// in your derived version, 
class cCompletion
	{
public:
	virtual ~cCompletion() {};

	// neither copyable nor movable
	cCompletion(const cCompletion&) = delete;
	cCompletion& operator=(const cCompletion&) = delete;
	cCompletion(const cCompletion&&) = delete;
	cCompletion& operator=(const cCompletion&&) = delete;

	// the interface functions:
	virtual void Callback(void) = delete;
	};

}; // namespace McciCatena

/**** end of Catena_Completion.h ****/
#endif /* _CATENA_COMPLETION_H_ */
