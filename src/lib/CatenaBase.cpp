/* CatenaBase.cpp	Sun Mar 19 2017 15:53:34 tmm */

/*

Module:  CatenaBase.cpp

Function:
	Implemenation of class CatenaBase;

Version:
	V0.5.0	Sun Mar 19 2017 15:53:34 tmm	Edit level 1

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
   0.5.0  Sun Mar 19 2017 15:53:34  tmm
	Module created.

*/

#include "CatenaBase.h"
using namespace McciCatena;

bool
CatenaBase::
begin(void)
	{
	this->m_PollingEngine.begin();
	return true;
	}

void
CatenaBase::registerObject(
	McciCatena::cPollableObject *pObject
	)
	{
	this->m_PollingEngine.registerObject(pObject);
	}

void
CatenaBase::poll()
	{ 
	this->m_PollingEngine.poll();
	}

void
CatenaBase::addCommands(
	cCommandStream::cDispatch &dispatch,
	void *pContext
	)
	{
	this->m_CommandStream.registerCommands(&dispatch, pContext);
	}

/* protected virtual */
void
CatenaBase::registerCommands(void)
	{
	/* TODO(tmm@mcci.com): add the syseui command */
	return;
	}
