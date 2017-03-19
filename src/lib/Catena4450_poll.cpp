/* Catena4450_poll.cpp	Sat Mar 18 2017 18:04:58 tmm */

/*

Module:  Catena4450_poll.cpp

Function:
	Catena4450::poll()

Version:
	V0.5.0	Sat Mar 18 2017 18:04:58 tmm	Edit level 1

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
   0.5.0  Sat Mar 18 2017 18:04:58  tmm
	Module created.

*/

#include <Catena4450.h>

using namespace McciCatena;

void Catena4450::poll(void)
	{
	this->Super::poll();
	this->m_Collector.poll();
	}
