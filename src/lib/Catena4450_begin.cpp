/* Catena4450_begin.cpp	Sun Mar 12 2017 19:19:28 tmm */

/*

Module:  Catena4450_begin.cpp

Function:
	Catena4450::begin()

Version:
	V0.5.0	Sun Mar 12 2017 19:19:28 tmm	Edit level 1

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
   0.5.0  Sun Mar 12 2017 19:19:28  tmm
	Module created.

*/

#include <Catena4450.h>

/*

Name:	Catena4450::begin()

Function:
	Set up all the well-known board peripherals.

Definition:
	bool Catena4450::begin();

Description:
	Issues begin() for all the Catena4450 things.

Returns:
	true for success, false for failure.

*/

bool Catena4450::begin()
	{
	if (! this->Super::begin())
		return false;

	// do initialization
	if (! this->m_Fram.begin())
		return false;
	
	return true;
	}
