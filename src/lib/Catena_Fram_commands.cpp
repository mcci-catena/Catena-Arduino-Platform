/*

Module:  Catena_Fram_commands.cpp

Function:
        McciCatena::cFram::addCommands() and command processors.

Copyright notice:
        See accompanying LICENSE file.

Author:
        Terry Moore, MCCI Corporation	March 2017

*/

#include "Catena_Fram.h"

#include "CatenaBase.h"
#include <cstring>
#include "mcciadk_baselib.h"

using namespace McciCatena;

/****************************************************************************\
|
|	The command processors
|
\****************************************************************************/


/****************************************************************************\
|
|	The command table
|
\****************************************************************************/

static const cCommandStream::cEntry sDefaultEntries[] =
        {
        { "dump", cFram::doDump },
        { "reset", cFram::doReset },
        };

static cCommandStream::cDispatch
sDispatch(sDefaultEntries, sizeof(sDefaultEntries), "fram");

/*

Name:	McciCatena::cFram::addCommands()

Function:
        Add the FRAM commands to the Catena command table.

Definition:
        bool McciCatena::cFram::addCommands();

Description:
        All the commands are added to the system command table.

Returns:
        true for success.

*/

bool
McciCatena::cFram::addCommands()
        {
        CatenaBase::pCatenaBase->addCommands(
                sDispatch, static_cast<void *>(this)
                );
        return true;
        }

/****************************************************************************\
|
|	The command handlers
|
\****************************************************************************/

cCommandStream::CommandStatus
McciCatena::cFram::doDump(
        cCommandStream *pThis,
        void *pContext,
        int argc,
        char **argv
        )
        {
        cFram * const pFram = static_cast<cFram *>(pContext);
        uint32_t uLength;
        uint32_t uBase;
        cCommandStream::CommandStatus status;

        // get arg 2 as length; default is 32 bytes
        status = cCommandStream::getuint32(argc, argv, 2, 16, uLength, 32);

        if (status != cCommandStream::CommandStatus::kSuccess)
                return status;

        // get arg 1 as base; default is 0
        status = cCommandStream::getuint32(argc, argv, 1, 16, uBase, 0);

        if (status != cCommandStream::CommandStatus::kSuccess)
                return status;

        // dump the FRAM
        uint8_t buffer[16];
        for (uint32_t here = 0; here < uLength; here += sizeof(buffer))
                {
                char line[80];
                size_t n;

                n = uLength - here;
                if (n > sizeof(buffer))
                        n = sizeof(buffer);

                std::memset(buffer, 0, n);
                pFram->read(uBase + here, buffer, n);

                McciAdkLib_FormatDumpLine(line, sizeof(line), 0, uBase + here, buffer, n);
                pThis->printf("%s\n", line);
                }

        return status;
        }

cCommandStream::CommandStatus
McciCatena::cFram::doReset(
        cCommandStream *pThis,
        void *pContext,
        int argc,
        char **argv
        )
        {
        cFram * const pFram = static_cast<cFram *>(pContext);
        cCommandStream::CommandStatus status;
        bool fResult;

        if (argc > 2)
                return cCommandStream::CommandStatus::kInvalidParameter;
        if (argc == 2 && strcmp(argv[1], "hard") != 0)
                return cCommandStream::CommandStatus::kInvalidParameter;

        if (argc == 2)
                fResult = pFram->reset();
        else
                fResult = pFram->initialize();

        return fResult ? cCommandStream::CommandStatus::kSuccess
                       : cCommandStream::CommandStatus::kError
                       ;
        }
