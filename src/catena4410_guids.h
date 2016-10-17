/* catena4410_guids.h	Mon Oct 17 2016 01:15:25 tmm */

/*

Module:  catena4410_guids.h

Function:
	GUIDs for the Catena4410 family.

Version:
	V0.1.0	Mon Oct 17 2016 01:15:25 tmm	Edit level 1

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
   0.1.0  Mon Oct 17 2016 01:15:25  tmm
	Module created.

*/

#ifndef _CATENA4410_GUIDS_H_		/* prevent multiple includes */
#define _CATENA4410_GUIDS_H_

// {2E6DFED4-F577-47D5-9137-B3E63976AE92}
#define GUID_HW_FEATHER_M0(f)		\
	MCCIADK_GUID_GEN_INIT(f, 0x2e6dfed4, 0xf577, 0x47d5, 0x91, 0x37, 0xb3, 0xe6, 0x39, 0x76, 0xae, 0x92)	

// {E2DECCC8-55FA-4BD3-94C3-CE66BCD0BAAC} 
#define	GUID_HW_FEATHER_M0_LORA(f)	\
	MCCIADK_GUID_GEN_INIT(f,  0xe2deccc8, 0x55fa, 0x4bd3, 0x94, 0xc3, 0xce, 0x66, 0xbc, 0xd0, 0xba, 0xac)

// {A67AD93C-551A-47D2-9ADB-E249B4CF915A}
#define	GUID_HW_FEATHER_M0_LORA_TTNNYC(f)	\
	MCCIADK_GUID_GEN_INIT(f,		\
0xa67ad93c, 0x551a, 0x47d2, 0x9a, 0xdb, 0xe2, 0x49, 0xb4, 0xcf, 0x91, 0x5a)

// {6785BBF2-B3F5-4D86-BB1F-BB132F9CB217} 
#define	GUID_HW_CATENA_4410_BASE(f)		\
	MCCIADK_GUID_GEN_INIT(f,  0x6785bbf2, 0xb3f5, 0x4d86, 0xbb, 0x1f, 0xbb, 0x13, 0x2f, 0x9c, 0xb2, 0x17)

// {0E2F07D9-3074-406B-9433-CD558CC24FB1}
#define	GUID_HW_CATENA_4410_GH(f)		\
	MCCIADK_GUID_GEN_INIT(f,  0xe2f07d9, 0x3074, 0x406b, 0x94, 0x33, 0xcd, 0x55, 0x8c, 0xc2, 0x4f, 0xb1)

// {D87BDEDB-4E52-481F-A87D-1BB7021B1C0E} 
#define	GUID_HW_CATENA_4410_MH(f)		\
	MCCIADK_GUID_GEN_INIT(f,  0xd87bdedb, 0x4e52, 0x481f, 0xa8, 0x7d, 0x1b, 0xb7, 0x2, 0x1b, 0x1c, 0xe)

// {CAC018DD-7D55-4C79-96C9-9FD879ADDF8F} 
#define	GUID_HW_CATENA_4410_SWH(f)		\
	MCCIADK_GUID_GEN_INIT(f,  0xcac018dd, 0x7d55, 0x4c79, 0x96, 0xc9, 0x9f, 0xd8, 0x79, 0xad, 0xdf, 0x8f)

// {CC0889AD-8DB8-485A-9EDC-01FC7846AFC4}
#define	GUID_HW_CATENA_4410_POND(f)		\
	MCCIADK_GUID_GEN_INIT(f,  0xcc0889ad, 0x8db8, 0x485a, 0x9e, 0xdc, 0x1, 0xfc, 0x78, 0x46, 0xaf, 0xc4)


/**** end of catena4410_guids.h ****/
#endif /* _CATENA4410_GUIDS_H_ */
