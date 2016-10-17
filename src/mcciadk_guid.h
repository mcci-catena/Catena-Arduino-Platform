/* mcciadk_guid.h	Mon Oct 17 2016 00:54:02 tmm */

/*

Module:  mcciadk_guid.h

Function:
	MCCIADK_GUID and so forth.

Version:
	V0.1.0	Mon Oct 17 2016 00:54:02 tmm	Edit level 1

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
   0.1.0  Mon Oct 17 2016 00:54:02  tmm
	Module created.

*/

#ifndef _MCCIADK_GUID_H_		/* prevent multiple includes */
#define _MCCIADK_GUID_H_

#ifndef _MCCIADK_ENV_H_
# include "mcciadk_env.h"
#endif

#include <stdint.h>

typedef struct MCCIADK_GUID_s
	{
	uint32_t	time_low;
	uint16_t	time_mid;
	uint16_t	time_hi_and_version;
	uint8_t		clock_seq_hi_and_reserved;
	uint8_t		clock_seq_low;
	uint8_t		node[6];
	} MCCIADK_GUID;

#define MCCIADK_GUID_NATIVE_INIT(t1, t2, t3, c1, c2, n1, n2, n3, n4, n5, n6) \
	{ (t1), (t2), (t3), (c1), (c2), { (n1), (n2), (n3), (n4), (n5), (n6) }}

/*
|| A guid on the wire.
*/
typedef struct MCCIADK_GUID_WIRE_s MCCIADK_GUID_WIRE;
struct MCCIADK_GUID_WIRE_s
	{
	uint8_t		time_low[4];
	uint8_t		time_mid[2];
	uint8_t		time_hi_and_version[2];
	uint8_t		clock_seq_hi_and_reserved;
	uint8_t		clock_seq_low;
	uint8_t		node[6];
	} __attribute__((__packed__));

#define	MCCIADK_GUID_WIRE__SIZE		16

/* validate the layout */
MCCIADK_C_ASSERT(offsetof(MCCIADK_GUID_WIRE, time_mid) == 4);
MCCIADK_C_ASSERT(offsetof(MCCIADK_GUID_WIRE, time_hi_and_version) == 6);
MCCIADK_C_ASSERT(offsetof(MCCIADK_GUID_WIRE, clock_seq_hi_and_reserved) == 8);
MCCIADK_C_ASSERT(offsetof(MCCIADK_GUID_WIRE, clock_seq_low) == 9);
MCCIADK_C_ASSERT(offsetof(MCCIADK_GUID_WIRE, node) == 10);
MCCIADK_C_ASSERT(sizeof(MCCIADK_GUID_WIRE) == MCCIADK_GUID_WIRE__SIZE);

/* 
|| initialize at compile time -- note that the arguments match those of a 
|| normal GUID initializer 
*/
#define MCCIADK_GUID_WIRE_INIT(t1, t2, t3, c1, c2, n1, n2, n3, n4, n5, n6) \
	{								\
	{ (uint8_t) (((t1) >> 24) & 0xFF),				\
	  (uint8_t) (((t1) >> 16) & 0xFF),				\
	  (uint8_t) (((t1) >> 8) & 0xFF),				\
	  (uint8_t)  ((t1) & 0xFF) },					\
	{ (uint8_t) (((t2) >> 8) & 0xFF),				\
	  (uint8_t)  ((t2) & 0xFF) },					\
	{ (uint8_t) (((t3) >> 8) & 0xFF),				\
	  (uint8_t)  ((t3) & 0xFF) },					\
	(c1),								\
	(c2),								\
	{ (n1), (n2), (n3), (n4), (n5), (n6) }				\
	}

/*
|| sometimes we need the bytes of a GUID deposited sequentiallly in a 
|| string
*/
#define MCCIADK_GUID_BYTES_INIT(t1, t2, t3, c1, c2, n1, n2, n3, n4, n5, n6) \
	/* t1 */							\
	  (uint8_t) (((t1) >> 24) & 0xFF),				\
	  (uint8_t) (((t1) >> 16) & 0xFF),				\
	  (uint8_t) (((t1) >> 8) & 0xFF),				\
	  (uint8_t)  ((t1) & 0xFF),					\
	/* t2 */							\
	  (uint8_t) (((t2) >> 8) & 0xFF),				\
	  (uint8_t)  ((t2) & 0xFF),					\
	/* t3 */							\
	  (uint8_t) (((t3) >> 8) & 0xFF),				\
	  (uint8_t)  ((t3) & 0xFF),					\
	(c1),								\
	(c2),								\
	(n1), (n2), (n3), (n4), (n5), (n6)

#define	MCCIADK_GUID_GEN_INIT(f, t1, t2, t3, c1, c2, n1, n2, n3, n4, n5, n6) \
	MCCIADK_GUID_##f##_INIT(t1, t2, t3, c1, c2, n1, n2, n3, n4, n5, n6)

/**** end of mcciadk_guid.h ****/
#endif /* _MCCIADK_GUID_H_ */
