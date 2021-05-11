/*

Module:  catenafeatherm0_sigfox_begin.cpp

Function:
	CatenaFeatherM0::Sigfox::begin()

Copyright notice:
    See accompanying LICENSE file.
 
Author:
	Dhinesh Kumar Pitchai, MCCI Corporation	December 2020

*/

#ifdef ARDUINO_ARCH_SAMD

#include <CatenaFeatherM0.h>

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

bool CatenaFeatherM0::Sigfox::begin(
	CatenaFeatherM0 *pParent
	)
	{
	gLog.printf(gLog.kTrace, "+CatenaFeatherM0::Sigfox::begin()\n");

	this->m_pCatena = pParent;

	/* first call the base begin */
	if (! this->MCCI_Catena_Sigfox::begin())
                {
                gLog.printf(
                        gLog.kBug,
                        "?CatenaFeatherM0::LoRaWAN::begin:"
                        " Arduino_LoRaWAN::begin() failed\n"
                        );
		return false;
                }

        /* here's where we do any required post-processing */
        /* (none at the moment) */

        /* indicate success to the client */
        return true;
	}

#endif // ARDUINO_ARCH_SAMD
