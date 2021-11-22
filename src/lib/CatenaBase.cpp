/*

Module:  CatenaBase.cpp

Function:
	Implemenation of class CatenaBase;

Copyright notice:
        See accompanying LICENSE file.

Author:
	Terry Moore, MCCI Corporation	March 2017

*/

#include "CatenaBase.h"

#include "Catena_Log.h"

using namespace McciCatena;

// the static pointer to the abstract view of the object.
CatenaBase *CatenaBase::pCatenaBase;

/* the constructor */
CatenaBase::CatenaBase(Version_t v)
	{
	this->m_appVersion = v;
	pCatenaBase = this;
	}

/* begin operating */
bool
CatenaBase::begin(void)
	{
	UniqueID_buffer_t CpuID;

	// use default debug flags
	gLog.begin();

	this->m_PollingEngine.begin();

	/* get the CPU ID */
	this->GetUniqueID(&CpuID);
	this->m_pPlatform = this->GetPlatformForID(
				&CpuID,
				&this->m_SysEUI,
				&this->m_OperatingFlags
				);

	// set up the command line collector
	this->m_Collector.begin(&Serial, &this->m_SerialReady);
	this->registerObject(&this->m_Collector);

	// set up the command line processor
	this->m_CommandStream.begin(&this->m_Collector, this);

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
