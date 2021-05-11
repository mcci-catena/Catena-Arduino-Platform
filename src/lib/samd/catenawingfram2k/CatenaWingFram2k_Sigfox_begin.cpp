/*

Module:  CatenaWingFram2k_Sigfox_begin.cpp

Function:
	CatenaWingFram2k::Sigfox::begin()

Copyright notice:
    See accompanying LICENSE file.
 
Author:
	Dhinesh Kumar Pitchai, MCCI Corporation	December 2020

*/

#ifdef ARDUINO_ARCH_SAMD

#include "CatenaWingFram2k.h"

#include "Catena_Log.h"
#include "mcciadk_baselib.h"

using namespace McciCatena;

/*

Name:	CatenaWingFram2k::Sigfox::begin()

Function:
	Record linkage to main Catena object and set up Sigfox.

Definition:
	bool CatenaWingFram2k::Sigfox::begin(
		CatenaWingFram2k *pParent
		);

Description:
	We record parent pointers, and other useful things for later.

Returns:
	true for success, false for failure.

*/

bool 
CatenaWingFram2k::Sigfox::begin(
	CatenaWingFram2k *pParent
	)
	{
	gLog.printf(gLog.kTrace, "+CatenaWingFram2k::Sigfox::begin()\n");
	MCCI_Sigfox m_Sigfox;

	this->m_pCatena = pParent;

        /* first set up command processor -- just in case begin() failed */
        pParent->addSigfoxCommands();

	/* call the base begin */
	if (! m_Sigfox.isReady())
		{
		gLog.printf(
			gLog.kError,
			"?CatenaWingFram2k::Sigfox::begin:"
			" MCCI_Catena_Sigfox:begin failed\n"
			);
		return false;
		}

	/* indicate success to the client */
	return true;
	}

#endif // ARDUINO_ARCH_SAMD
