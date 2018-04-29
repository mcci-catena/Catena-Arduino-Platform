/* Catena_Guids.h	Sat Mar 31 2018 19:28:30 tmm */

/*

Module:  Catena_Guids.h

Function:
	GUIDs for the Catena4410 family.

Version:
	V0.9.0	Sat Mar 31 2018 19:28:30 tmm	Edit level 5

Copyright notice:
	This file copyright (C) 2016-2017 by

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

   0.4.0  Wed Mar  8 2017 17:25:02  tmm
	Add Catena 4450 material.

   0.5.0  Sun Mar 19 2017 18:23:19  tmm
	Fix internal naming, and add storage FRAM guids.

   0.9.0  Sat Mar 31 2018 19:28:30  tmm
	Add Catena 4460 material.

*/

#ifndef _CATENA_GUIDS_H_		/* prevent multiple includes */
#define _CATENA_GUIDS_H_

#ifndef _MCCIADK_GUID_H_
# include "mcciadk_guid.h"
#endif

/****************************************************************************\
|
|	Platform GUIDs
|
|	After adding here, you must add the corresponding entries to 
|	- [ ]  Catena_Platforms.h
|	- [ ]  CatenaStm32_gk_PlatformHierarchy.cpp or 
|	       CatenaSamd_gk_PlatformHierarchy.cpp
|	- [ ]  As appropriate to the vPlatforms[] array for
|	       Catena*_GetPlatformForID.cpp
|
\****************************************************************************/

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

// {F6A15678-C7F3-43F4-AC57-67EF5CF75541}
#define	GUID_HW_FEATHER_M0_PROTO(f)             	\
	MCCIADK_GUID_GEN_INIT(f,		        \
        0xf6a15678, 0xc7f3, 0x43f4, 0xac, 0x57, 0x67, 0xef, 0x5c, 0xf7, 0x55, 0x41)

// {3BAB150F-6E32-4459-A2B6-72ACED75059F}
#define	GUID_HW_FEATHER_M0_PROTO_WINGLORA_TTNMCCI(f)	\
	MCCIADK_GUID_GEN_INIT(f,		        \
        0x3bab150f, 0x6e32, 0x4459, 0xa2, 0xb6, 0x72, 0xac, 0xed, 0x75, 0x5, 0x9f)


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

// {E19FE1DD-6881-4EF0-AD01-C1B6E19EDA34}
#define	GUID_HW_CATENA_4410_ANATOLIAN(f)	\
	MCCIADK_GUID_GEN_INIT(f,  0xe19fe1dd, 0x6881, 0x4ef0, 0xad, 0x1, 0xc1, 0xb6, 0xe1, 0x9e, 0xda, 0x34)

// {60480ACB-DC5D-4148-B6C9-ACA13449CF1D}
#define GUID_HW_CATENA_4450_BASE(f)		\
	MCCIADK_GUID_GEN_INIT(f,  0x60480acb, 0xdc5d, 0x4148, 0xb6, 0xc9, 0xac, 0xa1, 0x34, 0x49, 0xcf, 0x1d)

// {82BF2661-70CB-45AE-B620-CAF695478BC1}
#define GUID_HW_CATENA_4450_M101(f)		\
	MCCIADK_GUID_GEN_INIT(f,  0x82bf2661, 0x70cb, 0x45ae, 0xb6, 0x20, 0xca, 0xf6, 0x95, 0x47, 0x8b, 0xc1)

// {2281255E-AC5C-48CB-A263-9DC890D16638}
#define GUID_HW_CATENA_4450_M102(f)		\
        MCCIADK_GUID_GEN_INIT(f,  0x2281255e, 0xac5c, 0x48cb, 0xa2, 0x63, 0x9d, 0xc8, 0x90, 0xd1, 0x66, 0x38)

// {1fb2506f-0f2a-4310-9e6a-9bc191e0ae12}
#define GUID_HW_CATENA_4450_M103(f)		\
	MCCIADK_GUID_GEN_INIT(f,  0x1fb2506f, 0x0f2a, 0x4310, 0x9e, 0x6a, 0x9b, 0xc1, 0x91, 0xe0, 0xae, 0x12)

// {a731f637-e3ed-4088-a9a8-f54b6671dcf6}
#define GUID_HW_CATENA_4450_M104(f)		\
        MCCIADK_GUID_GEN_INIT(f,  0xa731f637, 0xe3ed, 0x4088, 0xa9, 0xa8, 0xf5, 0x4b, 0x66, 0x71, 0xdc, 0xf6)

// {3037D9BE-8EBE-4AE7-970E-91915A2484F8}
#define GUID_HW_CATENA_4460_BASE(f)		\
	MCCIADK_GUID_GEN_INIT(f,  0x3037d9be, 0x8ebe, 0x4ae7, 0x97, 0xe, 0x91, 0x91, 0x5a, 0x24, 0x84, 0xf8)

// {31E563D1-0267-43FC-BCA0-9A4CB5BFC55A}
#define GUID_HW_CATENA_4460_M101(f)		\
	MCCIADK_GUID_GEN_INIT(f,  0x31e563d1, 0x267, 0x43fc, 0xbc, 0xa0, 0x9a, 0x4c, 0xb5, 0xbf, 0xc5, 0x5a)

// {494F3C17-8AC1-4F80-8ECC-CA4DD3DCCBDC}
#define GUID_HW_CATENA_4460_M102(f)		\
        MCCIADK_GUID_GEN_INIT(f,  0x494f3c17, 0x8ac1, 0x4f80, 0x8e, 0xcc, 0xca, 0x4d, 0xd3, 0xdc, 0xcb, 0xdc)

// {A882186F-F4AB-4EE4-9402-7B628A76D886}
#define GUID_HW_CATENA_4460_M103(f)		\
	MCCIADK_GUID_GEN_INIT(f,  0xa882186f, 0xf4ab, 0x4ee4, 0x94, 0x2, 0x7b, 0x62, 0x8a, 0x76, 0xd8, 0x86)

// {398A9E5A-E22F-4265-9D35-BF45433DDBE3}
#define GUID_HW_CATENA_4460_M104(f)		\
        MCCIADK_GUID_GEN_INIT(f,  0x398a9e5a, 0xe22f, 0x4265, 0x9d, 0x35, 0xbf, 0x45, 0x43, 0x3d, 0xdb, 0xe3)

// {EA8568EC-5DAE-46EE-929A-A3F6B00A565E}
#define GUID_HW_CATENA_4470_BASE(f)		\
	MCCIADK_GUID_GEN_INIT(f, 0xea8568ec, 0x5dae, 0x46ee, 0x92, 0x9a, 0xa3, 0xf6, 0xb0, 0x0a, 0x56, 0x5e)

// {DD0A37A6-E469-43EC-B173-FED795129455}
#define GUID_HW_CATENA_4470_M101(f)		\
	MCCIADK_GUID_GEN_INIT(f,  0xdd0a37a6, 0xe469, 0x43ec, 0xb1, 0x73, 0xfe, 0xd7, 0x95, 0x12, 0x94, 0x55)

// {59F4EE9E-5DBE-4EEE-95B1-4F1C05731A2D}
#define GUID_HW_CATENA_4550_BASE(f)		\
	MCCIADK_GUID_GEN_INIT(f,  0x59f4ee9e, 0x5dbe, 0x4eee, 0x95, 0xb1, 0x4f, 0x1c, 0x05, 0x73, 0x1a, 0x2d)

// {8FEEDBAC-C5FC-445D-BF8A-C64C82348EE6}
#define GUID_HW_CATENA_4551_BASE(f)		\
	MCCIADK_GUID_GEN_INIT(f,  0x8feedbac, 0xc5fc, 0x445d, 0xbf, 0x8a, 0xc6, 0x4c, 0x82, 0x34, 0x8e, 0xe6)

// {e8405eb4-9618-405d-90b3-cc137017c498}
#define GUID_HW_CATENA_4551_M101(f)		\
	MCCIADK_GUID_GEN_INIT(f,  0xe8405eb4, 0x9618, 0x405d, 0x90, 0xb3, 0xcc, 0x13, 0x70, 0x17, 0xc4, 0x98)

#define GUID_HW_CATENA_4551_M102(f)		\
	MCCIADK_GUID_GEN_INIT(f,  0x1de63656, 0x2be7, 0x45bd, 0xa1, 0x4c, 0xc3, 0x70, 0x86, 0xb0, 0x9a, 0xb1)

// {84cd9ea0-05d8-4ae5-a87b-cecd3c99583a}
#define GUID_HW_CATENA_4551_M103(f)		\
	MCCIADK_GUID_GEN_INIT(f,  0x84cd9ea0, 0x05d8, 0x4ae5, 0xa8, 0x7b, 0xce, 0xcd, 0x3c, 0x99, 0x58, 0x3a)

// {93f951c8-b15a-4907-b7d1-6a467a0dcd1d}
#define GUID_HW_CATENA_4551_M104(f)		\
	MCCIADK_GUID_GEN_INIT(f,  0x93f951c8, 0xb15a, 0x4907, 0xb7, 0xd1, 0x6a, 0x46, 0x7a, 0x0d, 0xcd, 0x1d)

/**** end of catena_guids.h ****/
#endif /* _CATENA_GUIDS_H_ */
