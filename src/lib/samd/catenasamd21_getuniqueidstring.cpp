/* GetUniqueIDstring.cpp	Thu Oct 27 2016 23:15:50 tmm */

/*

Module:  GetUniqueIDstring.cpp

Function:
	CatenaSamd21::GetUniqueIDstring()

Version:
	V0.3.0	Thu Oct 27 2016 23:15:50 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2016 by

		MCCI Corporation
		3520 Krums Corners Road
                Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	October 2016

Revision history:
   0.1.0  Tue Oct 25 2016 03:19:12  tmm
	Module created.

*/

#ifdef ARDUINO_ARCH_SAMD

#include <CatenaSamd21.h>

#include <stdio.h>
using namespace McciCatena;


char *CatenaSamd21::GetUniqueIDstring(
        UniqueID_string_t *pStringBuf
        )
        {
        UniqueID_buffer_t CPUid;

        this->GetUniqueID(&CPUid);

        unsigned i;
        int j;
        
        for (i = 0, j = 0; i < sizeof(CPUid.b); ++i)
                {
                const int nBuf = sizeof(*pStringBuf) - j;
                const int jResult = 
                    snprintf(
                        &pStringBuf->c[j],
                        nBuf,
                        "%s%02x",
                        i == 0 ? "" : "-",
                        CPUid.b[i]
                        );

                /* deal with errors and overflow */
                if (jResult < 0 || jResult >= nBuf)
                    break;

                j += jResult;
                }

        return &pStringBuf->c[0];
        }

#endif // ARDUINO_ARCH_SAMD
