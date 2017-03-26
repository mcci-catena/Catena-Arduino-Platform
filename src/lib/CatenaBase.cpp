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

#include "Catena_Log.h"

using namespace McciCatena;

// the static pointer to the abstract view of the object.
CatenaBase *CatenaBase::pCatenaBase;

/* the constructor */
CatenaBase::CatenaBase()
	{
	pCatenaBase = this;
	}

/* begin operating */
bool
CatenaBase::begin(void)
	{
	gLog.begin(cLog::DebugFlags(gLog.kError | gLog.kBug));

	this->m_PollingEngine.begin();
	return true;
	}

/* register a pollable object with the polling engine */
void
CatenaBase::registerObject(
	McciCatena::cPollableObject *pObject
	)
	{
	this->m_PollingEngine.registerObject(pObject);
	}

/* poll all the registered objects */
void
CatenaBase::poll()
	{ 
	this->m_PollingEngine.poll();
	}

/* add a number of commands to the command table */
void
CatenaBase::addCommands(
	cCommandStream::cDispatch &dispatch,
	void *pContext
	)
	{
	this->m_CommandStream.registerCommands(&dispatch, pContext);
	}

/* register all the commands for this level and above */
/* protected virtual */
void
CatenaBase::registerCommands(void)
	{
	/* TODO(tmm@mcci.com): add the syseui command */
	return;
	}
