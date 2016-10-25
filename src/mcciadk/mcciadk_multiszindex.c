/* mcciadklib_multiszindex.c	Tue Oct 25 2016 08:42:33 tmm */

/*

Module:  mcciadklib_multiszindex.c

Function:
	McciAdkLib_MultiSzIndex()

Version:
	V0.1.0	Tue Oct 25 2016 08:42:33 tmm	Edit level 1

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
   0.1.0  Tue Oct 25 2016 08:42:33  tmm
	Module created.

*/

#include "mcciadk_baselib.h"

/*

Name:	McciAdkLib_MultiSzIndex()

Function:
	Index a code-space multisz to find the n'th entry.

Definition:
	const char *McciAdkLib_MultiSzIndex(
			const char * pmultiszStrings,
			unsigned uIndex
			);

Description:
	pmultiszStrings points to an array of characters, consisting of
	a sequence of '\0'-separated non-empty strings, and terminated by an
	empty string.  (Nul-terminated strings with an extra NUL at the end.)

	This routine finds the uIndex'th string in the array, or the trailing
	NUL if there aren't enough entries.

	Note that there can be no skipped values; a NUL encountered at the
	beginning of a value is interpreted as the end of the list.

Returns:
	Pointer to a non-zero character if uIndex'th string is present, or to 
	the trailing NUL otherwise.

*/

const char *
McciAdkLib_MultiSzIndex(
	const char * pmultiszStrings,
	unsigned uIndex
	)
	{
	const char *p;

	for (p = pmultiszStrings; 
	     *p != 0 && uIndex != 0; 
	     --uIndex)
		{
		/* skip to end of string */
		do	{
			++p;
			} while (*p != '\0');

		++p; /* point to next string */
		}

	/* 
	|| either return pointer to trailing NUL (second of double-NUL)
	|| or pointer to match.
	*/
	return p;
	}
