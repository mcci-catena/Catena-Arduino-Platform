/*

Module:	Catena_usingObject.cpp

Function:
    Catena::usingObject()

Copyright and License:
    This file copyright (C) 2021 by

        MCCI Corporation
        3520 Krums Corners Road
        Ithaca, NY  14850

    See accompanying LICENSE file for copyright and license information.

Author:
    Terry Moore, MCCI Corporation	December 2021

*/

#include "../Catena.h"

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

/*

Name:	Catena::usingObject()

Function:
    Arrange for object to be set up by Catena::setup().

Definition:
    bool Catena::usingObject(
        cSetupQueue &Object
        );

Description:
    Normally this function should be called before
    Catena::setup() is invoked; it adds the object
    to the list of objects to be initialized by
    Catena::setup().  If called after Catena::setup()
    has been run, the object is initialized immediately.

Returns:
    If Catena::setup() has not been called, this function
    returns `true`. If Catena::setup() failed, this
    function returns `false`. Otherwise, the object's
    setup() function is invoked, and its result is
    returned.

*/

#define FUNCTION "Catena::usingObject"

bool
Catena::usingObject(
    cSetupQueue &Object,
    void *pUserData,
    cSetupQueue::SetupFn_t *pSetupFn
    )
    {
    if (this->m_setupRun)
        {
        if (! this->m_setupResult)
            return false;

        this->m_setupResult = (*pSetupFn)(pUserData, this);
        return this->m_setupResult;
        }
    else
        {
        Object.init(pUserData, pSetupFn);
        if (this->m_pInitializationList == nullptr)
            this->m_pInitializationList = &Object;
        else
            this->m_pInitializationList->append(Object);

        return true;
        }
    }

#undef FUNCTION


/**** end of Catena_usingObjectPointer.cpp ****/
