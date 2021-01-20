/*

Module:  CatenaStm32L0_Sigfox_begin.cpp

Function:
	CatenaStm32L0::Sigfox::begin()

Copyright notice:
        See accompanying LICENSE file.

Author:
	Dhinesh Kumar Pitchai, MCCI Corporation	December 2020

*/

#ifdef ARDUINO_ARCH_STM32

#include <CatenaStm32L0.h>

#include <Catena_Log.h>
#include <mcciadk_baselib.h>

using namespace McciCatena;

/****************************************************************************\
|
|		Manifest constants & typedefs.
|
|	This is strictly for private types and constants which will not
|	be exported.
|
\****************************************************************************/



/****************************************************************************\
|
|	Read-only data.
|
|	If program is to be ROM-able, these must all be tagged read-only
|	using the ROM storage class; they may be global.
|
\****************************************************************************/


/****************************************************************************\
|
|	VARIABLES:
|
|	If program is to be ROM-able, these must be initialized
|	using the BSS keyword.  (This allows for compilers that require
|	every variable to have an initializer.)  Note that only those
|	variables owned by this module should be declared here, using the BSS
|	keyword; this allows for linkers that dislike multiple declarations
|	of objects.
|
\****************************************************************************/

bool CatenaStm32L0::Sigfox::begin(
	CatenaStm32L0 *pParent
	)
	{
	gLog.printf(gLog.kTrace, "+CatenaStm32L0::Sigfox::begin()\n");
	MCCI_Sigfox m_Sigfox;

	this->m_pCatena = pParent;

        /* first set up command processor -- just in case begin() failed */
        pParent->addSigfoxCommands();

	/* call the base begin */
	if (! m_Sigfox.isReady())
		{
		gLog.printf(
			gLog.kError,
			"?CatenaStm32L0::Sigfox::begin:"
			" MCCI_Sigfox:begin failed\n"
			);
		return false;
		}

	/* indicate success to the client */
	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_LoRaWAN_begin.cpp ****/
