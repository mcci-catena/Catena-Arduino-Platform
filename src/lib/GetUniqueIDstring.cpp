/* GetUniqueIDstring.cpp	Tue Oct 25 2016 03:19:12 tmm */

/*

Module:  GetUniqueIDstring.cpp

Function:
	CatenaSamd21::GetUniqueIDstring()

Version:
	V0.1.0	Tue Oct 25 2016 03:19:12 tmm	Edit level 1

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

#include <CatenaSamd21.h>

#include <stdio.h>

char *CatenaSamd21::GetUniqueIDstring(
        UniqueID_string_t pStringBuf
        )
        {
        UniqueID_buffer_t CPUid;

        this->GetUniqueID(CPUid);

        unsigned i;
        int j;
        
        for (i = 0, j = 0; i < sizeof(CPUid); ++i)
                {
                const int nBuf = sizeof(*pStringBuf) - j;
                const int jResult = 
                    snprintf(
                        pStringBuf + j,
                        nBuf,
                        "%s%02x",
                        i == 0 ? "" : "-",
                        CPUid[i]
                        );

                /* deal with errors and overflow */
                if (jResult < 0 || jResult >= nBuf)
                    break;

                j += jResult;
                }

        return pStringBuf;
        }
