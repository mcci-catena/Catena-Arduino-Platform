/*

Module:	Catena_usingLoRaWAN.cpp

Function:
    Catena::usingLoRaWAN()

Copyright and License:
    This file copyright (C) 2021 by

        MCCI Corporation
        3520 Krums Corners Road
        Ithaca, NY  14850

    See accompanying LICENSE file for copyright and license information.

Author:
    Terry Moore, MCCI Corporation	December 2021

*/

#include <Catena.h>
#include <Catena_AppObjects.h>
#include <Catena_Log.h>

using namespace McciCatena;

/****************************************************************************\
|
|	Manifest constants & typedefs.
|
\****************************************************************************/



/****************************************************************************\
|
|	Read-only data.
|
\****************************************************************************/



/****************************************************************************\
|
|	Variables.
|
\****************************************************************************/

Catena::LoRaWAN McciCatena::gLoRaWAN;

/*

Name:	Catena::usingLoRaWAN()

Function:
    Signal that we're using the LoRaWAN system.

Definition:
    bool Catena::usingLoRaWAN(
        void
        );

Description:
    If this routine is called, the LoRaWAN object will be referenced
    from the link, and will be setup by setup().

Returns:
    true if the object was scheduled for initialization.
    false if Catena::setup() has already run and failed, or
    if immediate initialization is performed and there's
    a fatal problem.

Notes:
    A lot of experimentation was needed to get this to work as
    expected (eliminating the gLoRaWAN object at compile time if
    this function was not called).

*/

#define FUNCTION "Catena::usingLoRaWAN"

bool
Catena::usingLoRaWAN(
    void
    )
    {
    return this->usingObject(
        *gLoRaWAN.getSetupQueue(),
        (void *)&gLoRaWAN,
        //
        // call-back function for initialization
        //
        [](void *pLoRaWAN_void, Catena *pCatena) -> bool
            {
            auto const pLoRaWAN = (Catena::LoRaWAN *)pLoRaWAN_void;
            gLog.printf(cLog::kTrace, "LoRaWAN init: ");

            if (! pLoRaWAN->begin(pCatena))
                {
                gLog.printf(cLog::kError, "LoRaWAN begin() failed\n");
                }
            else
                {
                gLog.printf(cLog::kTrace, "succeeded\n");
                }

            pCatena->registerObject(pLoRaWAN);

            /* once */
                {
                char sRegion[32];
                pCatena->SafePrintf(
                    "Configured network: %s, region %s\n",
                    pLoRaWAN->GetNetworkName(),
                    pLoRaWAN->GetRegionString(sRegion, sizeof(sRegion))
                    );
                }

            return true;
            }
        );
    }

#undef FUNCTION

/**** end of Catena_usingLoRaWAN.cpp ****/
