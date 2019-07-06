# The MCCI Catena Arduino Platform Library

This library provides a simple-to-use framework for taking advantage of many of the features of the MCCI Catena&reg; Arduino products.

_Apologies_: This document is a work in progress, and is published in this intermediate form in hopes that it will still be better than nothing.

[![GitHub release](https://img.shields.io/github/release/mcci-catena/Catena-Arduino-Platform.svg)](https://github.com/mcci-catena/Catena-Arduino-Platform/releases/latest) [![GitHub commits](https://img.shields.io/github/commits-since/mcci-catena/Catena-Arduino-Platform/latest.svg)](https://github.com/mcci-catena/Catena-Arduino-Platform/compare/v0.15.0...master) [![Build Status](https://travis-ci.com/mcci-catena/Catena-Arduino-Platform.svg?branch=master)](https://travis-ci.com/mcci-catena/Catena-Arduino-Platform)

<!-- TOC depthFrom:2 updateOnSave:true -->

- [Overview](#overview)
- [Coding Practices](#coding-practices)
- [Components](#components)
	- [Namespace `McciCatena`](#namespace-mccicatena)
	- [Class `Catena` and header file `Catena.h`](#class-catena-and-header-file-catenah)
	- [Board-specific Classes](#board-specific-classes)
	- [Class derivation](#class-derivation)
		- [STM32 Classes](#stm32-classes)
		- [SAMD Classes](#samd-classes)
	- [Platform Management](#platform-management)
		- [Platform GUIDs](#platform-guids)
		- [GUIDs for the Catena 461x family](#guids-for-the-catena-461x-family)
			- [Catena 4610](#catena-4610)
			- [Catena 4611](#catena-4611)
			- [Catena 4612](#catena-4612)
			- [Catena 4617](#catena-4617)
			- [Catena 4618](#catena-4618)
			- [Catena 4630](#catena-4630)
		- [GUIDs for the Catena 4450/4460/4470 family](#guids-for-the-catena-445044604470-family)
			- [Catena 4450](#catena-4450)
			- [Catena 4460](#catena-4460)
			- [Catena 4470](#catena-4470)
		- [GUIDs for the Catena 4801 family](#guids-for-the-catena-4801-family)
		- [GUIDs for Adafruit Feather M0s](#guids-for-adafruit-feather-m0s)
	- [Polling Framework](#polling-framework)
		- [Making a class pollable](#making-a-class-pollable)
		- [Using pollable objects in sketches](#using-pollable-objects-in-sketches)
	- [Finite State Machine (FSM) Framework](#finite-state-machine-fsm-framework)
		- [Getting ready](#getting-ready)
		- [Defining the state enum](#defining-the-state-enum)
		- [Identify the parent class](#identify-the-parent-class)
		- [Add the state enum to the parent class](#add-the-state-enum-to-the-parent-class)
		- [Define the FSM instance in the parent class](#define-the-fsm-instance-in-the-parent-class)
		- [Declare a method function in the parent class](#declare-a-method-function-in-the-parent-class)
		- [Implement the FSM dispatch function](#implement-the-fsm-dispatch-function)
		- [Implement the FSM intialization](#implement-the-fsm-intialization)
	- [LoRaWAN Support](#lorawan-support)
	- [FRAM Storage Management](#fram-storage-management)
		- [FRAM Storage Formats](#fram-storage-formats)
			- [Object Storage Structure](#object-storage-structure)
			- [Bit layout of `uSizeKey`](#bit-layout-of-usizekey)
			- [The FRAM header object](#the-fram-header-object)
		- [Adding FRAM objects](#adding-fram-objects)
		- [Class hierarchy within the FRAM library](#class-hierarchy-within-the-fram-library)
	- [Asynchronous Serial Port Command Processing](#asynchronous-serial-port-command-processing)
		- [Collecting lines asynchronously from streams](#collecting-lines-asynchronously-from-streams)
		- [The command parser](#the-command-parser)
		- [Command stream methods for use by functions](#command-stream-methods-for-use-by-functions)
		- [Synchronous Command Functions](#synchronous-command-functions)
		- [Asynchronous Command Functions](#asynchronous-command-functions)
	- [`Catena_functional.h`](#catena_functionalh)
- [Command Summary](#command-summary)
	- [Standard commands](#standard-commands)
	- [FRAM commands](#fram-commands)
	- [LoRaWAN commands](#lorawan-commands)
		- [LoRaWAN Parameters](#lorawan-parameters)
- [Adding your own commands](#adding-your-own-commands)
- [Example sketches](#example-sketches)
	- [`catena_hello`](#catena_hello)
	- [`catena_hello_lora`](#catena_hello_lora)
	- [`catena_usercommand`](#catena_usercommand)
- [Board Support Dependencies](#board-support-dependencies)
- [Other Libraries and Versions Required](#other-libraries-and-versions-required)
- [Library Release History](#library-release-history)

<!-- /TOC -->
<!-- Due to a bug in Markdown TOC, the table is formatted incorrectly if tab indentation is set other than 4. Due to another bug, this comment must be *after* the TOC entry. -->

## Overview

## Coding Practices

In order to assist people who are not everyday readers and writer of C++, this library adopts some rules.

1. All names are in the `McciCatena` namespace.

2. In classes with elaborate hierarchy, we normally define a private synonym of `Super` which refers to the parent class. This is done so that we can change parent/child relationships without breaking code.

3. We tend to use the `m_...` prefix on the names of class member fields.

4. We tend to use `this->m_...` to refer to class members (rather than omitting `this->`). We do this for emphasis, and to avoid visual ambituity.

5. We tend to name classes starting with a lower-case letter `c`, i.e., <code><strong>c</strong><em><u>ClassName</u></em></code>. For the `Catena...` classes, we don't follow this rule, however.

6. We don't use most of the standard C++ library (because of the frequent use of exceptions), nor do we use exceptions in our own code. The exception framework tends to be inefficient, and it's a source of coding problems because the error paths are not directly visible.

7. However, we do take advantage of some of the C++-11 header files, such as `<functional>`, `<type_traits>`, and `<cstdint>`.  (Sometimes we have to do extra work for this.)

## Components

### Namespace `McciCatena`

Unless otherwise specified, all symbols are defined inside namespace `McciCatena`. Usually sketches begin with something like this:

```c++
#include <Catena.h>

//... other includes

using namespace McciCatena;
```

### Class `Catena` and header file `Catena.h`

`Catena.h` is the main header file for the library. It uses the `#defines` injected by `board.txt` and `platform.txt` from the Arduino environment to create a class named `Catena` derived from the `Catena...` class that is specific to the board for which the software is being built. This allows examples to be source-compatible, no matter which Catena is our target.

### Board-specific Classes

`Catena.h` defines the class `Catena` in terms on one of the following classes based on the setting of the BSP:

The known classes and header files are:

Class                 | Header File             | Description
----------------------|-------------------------|------------------
`Catena4410`          | `Catena4410.h`          | First generation MCCI systems with BME180
`Catena4420`          | `Catena4420.h`          | Feather M0 Bluetooth + LoRa Radio Wing
`Catena4450`          | `Catena4450.h`          | MCCI Catena 4450
`Catena4460`          | `Catena4460.h`          | MCCI Catena 4460
`Catena4470`          | `Catena4470.h`          | MCCI Catena 4470
`Catena4551`          | `Catena4551.h`          | MCCI Catena 4551 first-generation Murata-based board.
`Catena4610`          | `Catena4610.h`          | MCCI Catena 4610 second-generation Murata-based board with LiPo charging
`Catena4611`          | `Catena4611.h`          | MCCI Catena 4611 second-generation Murata-based board with fixed Vdd, no charging
`Catena4612`          | `Catena4612.h`          | MCCI Catena 4612 second-generation Murata-based board with variable Vdd, no charging.
`Catena4617`          | `Catena4617.h`          | MCCI Catena 4617 second-generation Murata-based board with variable Vdd, no charging
`Catena4618`          | `Catena4618.h`          | MCCI Catena 4618 second-generation Murata-based board with variable Vdd, no charging.
`Catena4630`          | `Catena4630.h`          | MCCI Catena 4630 Murata-based board with Air Quality Sensor.
`Catena4801`          | `Catena4801.h`          | MCCI Catena 4801 Murata-based board with Modbus.

### Class derivation

The following figures gives the class derivation hierarchy for the board classes.

The tree is too big to show in one diagram here. So we split according to the two families: STM32-based CPUs, and SAMD-based CPUS.

<!-- 

For all these, see assets/CatenaBase.plantuml. for some bizarre reason github won't render these unless image links are http (not https).

Each image is structured as a reference to the image, plus a link. Both must be updated; only the link may be https; the image must be http.

Also note that the link is SVG, but the image is PNG.

-->

#### STM32 Classes

The first figure just gives relationships; the second has details about the members of each class.

**Catena STM32 Class Relationshiops:**
<!-- $enableStm32 = 1, $enableSamd = 0,  $enableMembers = 0 -->

[![**Catena STM32 Class Relationships**:](http://www.plantuml.com/plantuml/png/xLdhSzf64F_C_HMNIQUn7Jy0nuz9cvaG6JnC0JE0cm-P37EI5b0jwTIx4uIc_j-xEX1w65bsEqdicNunQFUtZ_snktVWzr8neKBF9KUl2EboEtJXRSLW2dpsWKau35pcQprfXRwb7Ew_9PL-9BpkTgY02GZmBQ2IXm9V9bpGGRDnqMqUUZQD89RBfAIsmwQ2UOUaGWmUB8KpdIbqD0IWpBAu5p1_wVXJscaRpTwmIIUE2uXjX6h61G978CIITZaNi4-xXj6c1XS15opAgDBZS_1C4BHUhPqJqkCAP3fDnsDJaDJnezow5pet9e1EmGTq0JOrbpHWKaKfg9aZTNp4AqugQeRLWPYZUwReJAdWxT7HOh7OK7E8-H_53_kK-JR5MarpvGpB4213L_k-7dJa8QMZMUI9IQeuDO7Qt8z2O0JcKkLu8FVfmb4pP6sY-LMaCcTkY2M0xM2wEhF8DvqpuJ3JHOq9BbySeckWc8-7Hb71oOT1AWl7HVU0VvOyf0lCuPMaFe0DzY4Xhuu8UV4J-96thcPJqdUqkf4DbNTSHqajaJ3FrX3YCymvO1RGhcKvg-wXNmbXfbI2MMhT14bRePBIrwWCKTcyRf--6PjXLEjO_N97Wm_8NLPpxJj_Xj2-s9Xz-hnD7Tl6Ql13ZrxrcuF6gDsx7BSwZSjXekXt6gFMrQ2Rad-LYYd7kWsHyUFwM57iAOLDxhLSDfNZIr0uCxqmecjdDsDXSkvIY_jImPT0PErUZziIxP3ujT6SEpOTiWdqCLqrMKiZwvY43-iwwHx672ubIkE0CSktAxpH63LxZN5SZ3ROHEUYVN77JaT7LCnQ77x615edyXMPiROqs1gueGvLLq4qR-X4qv0fla2PjivmkBqA4wQENq3Cgfvr5_QO1pkxL6ksEHgw0C5THot1FBet1HMzIUSVSDTDFmjCH2eGL-OVOAcrXjcsWRF9V3jElo_ut94Ezx7ieLgweElC2zTWtLAeRvX16jBs9po7k5AC5K1qTZrGGpQ7be7xCIXGNF25lqKr11avpGV5qZfym3uoFrTXF0-fuTyBaeS8ypB7VSnTrbCUDKqQOXIjeqAK4OHjUnlK6e9mc3i0PYyhtZZrfDNPlKGHeCTkPThdhel4DdtiDq1T-dcwzgu4CA_jenmCL8Ebk43evlqAjgvoXKPrvZc27CGpWChiZ6XjmI1wuo2kMSCq6vBD8HdFH3xDh0XDMAI-9I-S2Tt9x7iy0NQtB_P8Nw_HhzcHeRezqdEP4cmgutCGmh6Xo-JD1dMJHuM62qn4c7sQ28UW4hE2NPQVwOUrtC6giC78GEe7ojACW8_7CacOISx6Ocequ4aoKqv4GT6bxT02fcOWkbMiEpvX6xr6v-fot6_tKiVkHVkoFMfq4kaTNYOkPuf6a_cxoLHwRR_CB_dyrjCdqjrLM6ghzuGsrckjqRk1vPgCxUL-HAFeKAZVVCjwN-TFeCG1YZvmhWmU-chdPpD-kohrlYQtrcbZZxV4tPMpSVhVkh-3VP0lzbzNb9cZ0hQAGQ7CWFAxePoiytAoj29JFkwFP8iKDD6RavCltxHzYcYuBKhejFRK4Zf-OWcTLtz8GkLREPFbcnhENt52wHlozWlooqViI4INNRuEudjXaTlfpNZjTVmRkjrYO_IlsnUR6y-us-ZJkGutud70Jv-ts4ddBnWhwkV14QKvqkgF9-r_qawGj0VIKtYHVpmPkGlmpXvToUUngVpqUVU-mcBS3sBjzF6iVLTIxcPyzjGpBh-kvCxUsaCA-iuqdzQUMMDWnhLdbt7zsMLyzkmoFdzk6Hy_jyaxhpxnoIk_jAP-9lprxuE3p4zB9Ukyo5WZokCcSRHn_bUCGaJonP_aJ69KV06_Nrz7VSvX5d1I1ZWj0vnL7qZXIfBdi5CbcQSO47zpBTRYzD-hFIxMwYzOX2HHSb5h90cH2rajLjrZLIzMdHMhpabIIwwsAaaAoHMsQUdR-1F9_GFk7QtkafbZ0_LMejZWFRw4dliF)](https://www.plantuml.com/plantuml/svg/xLdhSzf64F_C_HMNIQUn7Jy0nuz9cvaG6JnC0JE0cm-P37EI5b0jwTIx4uIc_j-xEX1w65bsEqdicNunQFUtZ_snktVWzr8neKBF9KUl2EboEtJXRSLW2dpsWKau35pcQprfXRwb7Ew_9PL-9BpkTgY02GZmBQ2IXm9V9bpGGRDnqMqUUZQD89RBfAIsmwQ2UOUaGWmUB8KpdIbqD0IWpBAu5p1_wVXJscaRpTwmIIUE2uXjX6h61G978CIITZaNi4-xXj6c1XS15opAgDBZS_1C4BHUhPqJqkCAP3fDnsDJaDJnezow5pet9e1EmGTq0JOrbpHWKaKfg9aZTNp4AqugQeRLWPYZUwReJAdWxT7HOh7OK7E8-H_53_kK-JR5MarpvGpB4213L_k-7dJa8QMZMUI9IQeuDO7Qt8z2O0JcKkLu8FVfmb4pP6sY-LMaCcTkY2M0xM2wEhF8DvqpuJ3JHOq9BbySeckWc8-7Hb71oOT1AWl7HVU0VvOyf0lCuPMaFe0DzY4Xhuu8UV4J-96thcPJqdUqkf4DbNTSHqajaJ3FrX3YCymvO1RGhcKvg-wXNmbXfbI2MMhT14bRePBIrwWCKTcyRf--6PjXLEjO_N97Wm_8NLPpxJj_Xj2-s9Xz-hnD7Tl6Ql13ZrxrcuF6gDsx7BSwZSjXekXt6gFMrQ2Rad-LYYd7kWsHyUFwM57iAOLDxhLSDfNZIr0uCxqmecjdDsDXSkvIY_jImPT0PErUZziIxP3ujT6SEpOTiWdqCLqrMKiZwvY43-iwwHx672ubIkE0CSktAxpH63LxZN5SZ3ROHEUYVN77JaT7LCnQ77x615edyXMPiROqs1gueGvLLq4qR-X4qv0fla2PjivmkBqA4wQENq3Cgfvr5_QO1pkxL6ksEHgw0C5THot1FBet1HMzIUSVSDTDFmjCH2eGL-OVOAcrXjcsWRF9V3jElo_ut94Ezx7ieLgweElC2zTWtLAeRvX16jBs9po7k5AC5K1qTZrGGpQ7be7xCIXGNF25lqKr11avpGV5qZfym3uoFrTXF0-fuTyBaeS8ypB7VSnTrbCUDKqQOXIjeqAK4OHjUnlK6e9mc3i0PYyhtZZrfDNPlKGHeCTkPThdhel4DdtiDq1T-dcwzgu4CA_jenmCL8Ebk43evlqAjgvoXKPrvZc27CGpWChiZ6XjmI1wuo2kMSCq6vBD8HdFH3xDh0XDMAI-9I-S2Tt9x7iy0NQtB_P8Nw_HhzcHeRezqdEP4cmgutCGmh6Xo-JD1dMJHuM62qn4c7sQ28UW4hE2NPQVwOUrtC6giC78GEe7ojACW8_7CacOISx6Ocequ4aoKqv4GT6bxT02fcOWkbMiEpvX6xr6v-fot6_tKiVkHVkoFMfq4kaTNYOkPuf6a_cxoLHwRR_CB_dyrjCdqjrLM6ghzuGsrckjqRk1vPgCxUL-HAFeKAZVVCjwN-TFeCG1YZvmhWmU-chdPpD-kohrlYQtrcbZZxV4tPMpSVhVkh-3VP0lzbzNb9cZ0hQAGQ7CWFAxePoiytAoj29JFkwFP8iKDD6RavCltxHzYcYuBKhejFRK4Zf-OWcTLtz8GkLREPFbcnhENt52wHlozWlooqViI4INNRuEudjXaTlfpNZjTVmRkjrYO_IlsnUR6y-us-ZJkGutud70Jv-ts4ddBnWhwkV14QKvqkgF9-r_qawGj0VIKtYHVpmPkGlmpXvToUUngVpqUVU-mcBS3sBjzF6iVLTIxcPyzjGpBh-kvCxUsaCA-iuqdzQUMMDWnhLdbt7zsMLyzkmoFdzk6Hy_jyaxhpxnoIk_jAP-9lprxuE3p4zB9Ukyo5WZokCcSRHn_bUCGaJonP_aJ69KV06_Nrz7VSvX5d1I1ZWj0vnL7qZXIfBdi5CbcQSO47zpBTRYzD-hFIxMwYzOX2HHSb5h90cH2rajLjrZLIzMdHMhpabIIwwsAaaAoHMsQUdR-1F9_GFk7QtkafbZ0_LMejZWFRw4dliF "Click for SVG image of Class Relationships")

**Catena STM32 Class Hierarchy (full detail):**
<!-- $enableStm32 = 1, $enableSamd = 0,  $enableMembers = 1 -->
[![**Catena STM32 Class Hierarchy (full detail):**](http://www.plantuml.com/plantuml/png/xLbjRzh84t_DVegj-jU_IPe78Cs3UbUfr8487H04vFgYgj3Q7i0Ns-lRNKEvNhxxZHUC7y1ngghNHBet0S_yvc5-pCmkvBrKJAZGSyd9AqAwt0vTU5inc0AVVM0IZWEN-LfFMg5lAOVxRqcb7mblknqgO082V0kevA70bmaNTD1iN7MRnvvD8uZbCYcfxR2fODungH231qlXJ6SA7Gq1ABCixWNCNph-b7RQHhCtRDA9um9Y6w6QSO704GYnf5tE1HpIhc6qgS55m0MBCghq-1my4mIjLskNXFIu0fbEq_7O52Hr_2YtxfNEZGcWK_01NO1DpIKDc5HH2chcI1qVyOgJYffXTI3cw9ufEbCgU7joibWiDjGSO_udySCXPRvDiLRJN3d3CWJ8qDM-RmSTUKpfQ1PvOf8gJasWDlUZ41Y1kLGv7iX3kd3K35cRQ7uLgSoP6s89O3kOhiuiyatdJ3ZCT55ZWiiNn-WQAERZeL5KSF9XaCh2ST4zu9yb3-a2StWbgGzWWtrCoAiJGbxy3_p8MrUpAUaxMjl8XiexhMSbpBDHK2N4PvXpm2oWNSjoLjr3lnB2JAa4izIw2PAsGYMbhr4Peh9vspv_CpR3gDQn-cLBWm_8NLPpwpj_Xj2-sfXz-hnB7Tl6Ql13ZzxqcuF6gDsx7hSwZUjXekXt6gFMpQ2Rad-LYYd7kW-HyTFwM57iAOLDxhLSDfNZQr0uCxqmecjlFsDXSkvIY_jImPT0PErUZziIxP3ujT6SEpOTiWdq1GQQhAMHTKp2XtMTz01Z3fSIfN70cENx5TveZ9gznZWkHXjiedFHljgoqz4H5RCMXvynWTQ9V4Lch2qDjWPkg4FLJH3D6phHD6IABr2chJCSxgx2XAdZ5n2pgcVTXJtcmTu-rPfTZeOkGB3jg2MOHm-ceA8twVm3xhhfPu69I0NYnlm3BBNMCDisS3QPRyVfzmMVEzBXFfOzL4iNT9rvuHgiMmhr3JD8Gzh-XESGDugn0eZEhWTgoERGCd0_1WMAAxxmTwY682Ed-Q1OMeSFs4VcvogCvo4r_0T1yX1XNkQuZxdBUigZfab3ZA9rL8WoWh1jxu9QGn0ESmV0x6N56wUUj3gxboW2zDYjJFlSTP7Ofe_z1gXBFq_NtfK0vhLzb8E1Qh0K5mZTl5_1rbMkqAZEF4UGetW6K9cT4Qqj64Hl7CGrQvXcGx8v9EEPoAUP5Q49YzJtv8KpeNkPVOydmFxknHxfwpNwDJioD6gFp5oc19lA-1o4S6peCdctGTtbKQ7X0XCHvf0cmY6enAnWb-Ld-d4jTxGgR30oaFg1iZGZuEEnJ19cahEnc1eDU9BCb1DHM7H9ExI0gHc8RXNhZa_OHg_HkRaUzzkzrB5xrRvkZngTHBh5oyJbJD5eCdytcKglxPVv9P_VUlf4sbw5fRPwJsXZlTOQlJjOhidONUv7D8eEXVhTZwptTVu4IXoWw0Fdok2Xh_R-RyPlLwK-rEJMEctiyPRONZaRfz_N_HtiWtontr5HQewAsIe0XJ83ok-6Sh8koydIYapvk3-IBLBGH6_EphxyqFOfek6-AA7psbDBwFI99NHP_La9bMpbJ9PlQZX_nGcbRyYxByWl7xqZDRhiydKKtmkBt4xlnckluz_GxGuRet_RljhSUCRTHf_ETRWHZmD--hp3JZf_mLXHFm-EACsHLlzsqluZxGn9UoGzXHVvRoSZTm7UEw0h-JmsbPy-xpzKM8pxIQoTVpzhFvIKxOmlddh6vTULtDbRUqn1_pBDvxLdrXYOSUtPPLn_TXbVFBkCBvzRngVFRV8kgqzyyielhQcV2N_z--WeyzDIsJelCDQ8yhX976sS_nMZ495yyISvanWLNy0VrjTHdtEO1PoL0SxB01VLHrAuakGvx5H9vYa6nDzSYxKu_GzgJukr-WiM8KcKNDGQIKBaGbQBLGzOrOjL5yMgIvBKagkjIf92SeLjMle-_aHo_u1xHwlxPEROGBsLA3PuZo-Xv_u3)](https://www.plantuml.com/plantuml/svg/xLbjRzh84t_DVegj-jU_IPe78Cs3UbUfr8487H04vFgYgj3Q7i0Ns-lRNKEvNhxxZHUC7y1ngghNHBet0S_yvc5-pCmkvBrKJAZGSyd9AqAwt0vTU5inc0AVVM0IZWEN-LfFMg5lAOVxRqcb7mblknqgO082V0kevA70bmaNTD1iN7MRnvvD8uZbCYcfxR2fODungH231qlXJ6SA7Gq1ABCixWNCNph-b7RQHhCtRDA9um9Y6w6QSO704GYnf5tE1HpIhc6qgS55m0MBCghq-1my4mIjLskNXFIu0fbEq_7O52Hr_2YtxfNEZGcWK_01NO1DpIKDc5HH2chcI1qVyOgJYffXTI3cw9ufEbCgU7joibWiDjGSO_udySCXPRvDiLRJN3d3CWJ8qDM-RmSTUKpfQ1PvOf8gJasWDlUZ41Y1kLGv7iX3kd3K35cRQ7uLgSoP6s89O3kOhiuiyatdJ3ZCT55ZWiiNn-WQAERZeL5KSF9XaCh2ST4zu9yb3-a2StWbgGzWWtrCoAiJGbxy3_p8MrUpAUaxMjl8XiexhMSbpBDHK2N4PvXpm2oWNSjoLjr3lnB2JAa4izIw2PAsGYMbhr4Peh9vspv_CpR3gDQn-cLBWm_8NLPpwpj_Xj2-sfXz-hnB7Tl6Ql13ZzxqcuF6gDsx7hSwZUjXekXt6gFMpQ2Rad-LYYd7kW-HyTFwM57iAOLDxhLSDfNZQr0uCxqmecjlFsDXSkvIY_jImPT0PErUZziIxP3ujT6SEpOTiWdq1GQQhAMHTKp2XtMTz01Z3fSIfN70cENx5TveZ9gznZWkHXjiedFHljgoqz4H5RCMXvynWTQ9V4Lch2qDjWPkg4FLJH3D6phHD6IABr2chJCSxgx2XAdZ5n2pgcVTXJtcmTu-rPfTZeOkGB3jg2MOHm-ceA8twVm3xhhfPu69I0NYnlm3BBNMCDisS3QPRyVfzmMVEzBXFfOzL4iNT9rvuHgiMmhr3JD8Gzh-XESGDugn0eZEhWTgoERGCd0_1WMAAxxmTwY682Ed-Q1OMeSFs4VcvogCvo4r_0T1yX1XNkQuZxdBUigZfab3ZA9rL8WoWh1jxu9QGn0ESmV0x6N56wUUj3gxboW2zDYjJFlSTP7Ofe_z1gXBFq_NtfK0vhLzb8E1Qh0K5mZTl5_1rbMkqAZEF4UGetW6K9cT4Qqj64Hl7CGrQvXcGx8v9EEPoAUP5Q49YzJtv8KpeNkPVOydmFxknHxfwpNwDJioD6gFp5oc19lA-1o4S6peCdctGTtbKQ7X0XCHvf0cmY6enAnWb-Ld-d4jTxGgR30oaFg1iZGZuEEnJ19cahEnc1eDU9BCb1DHM7H9ExI0gHc8RXNhZa_OHg_HkRaUzzkzrB5xrRvkZngTHBh5oyJbJD5eCdytcKglxPVv9P_VUlf4sbw5fRPwJsXZlTOQlJjOhidONUv7D8eEXVhTZwptTVu4IXoWw0Fdok2Xh_R-RyPlLwK-rEJMEctiyPRONZaRfz_N_HtiWtontr5HQewAsIe0XJ83ok-6Sh8koydIYapvk3-IBLBGH6_EphxyqFOfek6-AA7psbDBwFI99NHP_La9bMpbJ9PlQZX_nGcbRyYxByWl7xqZDRhiydKKtmkBt4xlnckluz_GxGuRet_RljhSUCRTHf_ETRWHZmD--hp3JZf_mLXHFm-EACsHLlzsqluZxGn9UoGzXHVvRoSZTm7UEw0h-JmsbPy-xpzKM8pxIQoTVpzhFvIKxOmlddh6vTULtDbRUqn1_pBDvxLdrXYOSUtPPLn_TXbVFBkCBvzRngVFRV8kgqzyyielhQcV2N_z--WeyzDIsJelCDQ8yhX976sS_nMZ495yyISvanWLNy0VrjTHdtEO1PoL0SxB01VLHrAuakGvx5H9vYa6nDzSYxKu_GzgJukr-WiM8KcKNDGQIKBaGbQBLGzOrOjL5yMgIvBKagkjIf92SeLjMle-_aHo_u1xHwlxPEROGBsLA3PuZo-Xv_u3 "Click for SVG image of Class Hierarchy")

#### SAMD Classes

The first figure just gives relationships; the second has details about the members of each class.

**Catena SAMD Class Relationshiops:**
<!-- $enableStm32 = 0, $enableSamd = 1,  $enableMembers = 0 -->
[![**Catena SAMD Class Relationships**:](http://www.plantuml.com/plantuml/png/xLbhRzfC5lxC_8gfkjecQIv0cekwMwdKWGWj40Ha-w6gqDW-W3UsnpipXhBT_FVtU63m9JYaglesaTulJJpFSoxdoJd70_qe5HCg3dnoyeQGBdTZ7zvNBAOWP9-OXEF8Pw76IIiE7UNny3sfz9F3ssw72fY0WD01AdaiyCU42pfeDgwwpUF0fGd5yPcKrFNOLB3Wc5I8nQEbyAOpXOw609Gv3WyY5YwzS4exRQlP6pRfnFC1kOrOpRX0uWY4MD8kvm8EQTUosjJY8kA29HbLUdmEWGs2rgkrIq9wN87Cfka5R0gIUc6IM_TAvyO4q2c4W2x0fVQIHaog90KrywIEZtp5IKNDi3eGStJF59qf5Rq_ELai5XjfZZ7_4_DmI5deKgpLjbVEi0m1ClQrxzj1HnvJEfebdfYaYbCRgCl390H6O3vLNW3oa2uyDKFL9bfVHIfpviTO0hWUfgipItpJEHCUitr4RF3vuXXT0yLy0ZJI1M_-6AIoy7nq3_ZFaiTqWJcyaJG4SC4z9kJD2I6l_WPXugshrPJq0wrkpeOgEAtdJrZWua6Da91XpX5pW7OTnrjr3_rE2BEb4inHwoP8smf1IjyY62FOl6sVlnlRSLBhMFtZ0O8Fg5qUkGsz_yRGljgOVVcw3JQs1eOm3kXDlpbeZDgzwt6hqxWUfa2_qnYrRWRTpFbtgPZodFiO5J-jZnN5dbBOv47BPrCvlWQ5CzEB4xdszdCMDkS-TNWe5NoBHDxkxRWjqGw5NnlDkUVI8Pj07zDLa_LfOcr4-BIkanvWpE5Iuga9Q5I-N_6jngZPQunDCTfW4vsBzjK3EnqTMKOr4tx615gdvoin3Reps9guaGwXcoYPDtIYPSWLNm9chNCQRg_2XgaNbWYpgcVTXJqMmDu-rSWsHqCV87heg2LOG0-ce99Vilb7tFUppm8JaGh4ZVqVSDGQOQvhuMooq3NfzmMVUzBZ8POzL4iVT9t5mpLPjnJY3JlAKjhXX1SODugn4ehEhWTgoERGid0_HYN05L-4Mw0XoCHfCIYMrk43zfc5fiB2F6I6_o1A7nBEwuBsHhkyfo9hcZR4A5b7fImhYjlkDgep1E4nVm3CNLQ2SUP9m_czH96WnsvbskU-Zy8sG-mtG2pxF5rxLm9Oq0xn72o4mL5S87Np-ugshdB5HZZtd52Ep0mWc9yHZPOCOZ2EJCsQfjMGR0xPyJJdqzoAq88by3rvvKteNcxVuyBVtxxO4xnUez_p8qDrUsJdCdEmgOpFGGZFXIwJTnlMNP4LMpumaN0EQNeu19MQbUoouam_hUMELeKDHXPAFr2ELWH2rvkGh3BctLWkZIOyIsLs2r5Qz8vsQ05JCn3TAjPjth2DNgDpSptkjtkPr-vL-xezQdJIqmUwJ7pE54qcyzysKzcr_Rguv8jRJx-H7gx2dRPwJsXZlTOQlJjOhiNONkvdD4fU2lMxNrdls-8RA7M0HvyuLnQFGxNtTzlykYhriIPtrcbZZxV4mvMpSVfptT_1FYWM-nCLPUQeHAroKYnpfE9ks2tMvMwnD90h7_T7kaLAckZTsTctNze-PJBSboLqNdjk2Pq-iuGkgxybeTrREPVbknhENtb2sHlozWlowoVlICtUTVawClV2ChVJk_7Qw_XVw7QBZTM_RLzjRZpZRgDFvphSY9k0NxvkiPFU_s2iQ5Wa9v8MHAl_9TgFYtQ6eZrHdlAB_0-BKRW0xntGrVdScCelN_SVAypmVfDgvp-lsYyLvM765yyzuztNbSAxj_RKWlv4cSzhBwmnCEFQYyku_k8oldXn6L--j8nFNzhaNLQV-UJjlhHclYRyv_-FZd9VBUrOvsN6ch4xRXf76nU_nIXbf1_yIS74iCm5_76yZdZ1OPvmjejmlejmKNsYXAkJeeQTAiaz6OBvv5gEulG_YfwMe_e35Y5fb4BK6ab3549Mow57hEhbq4KvT4dIMWgrLKbQIA6mJKlVcxz8uJ_WFj3gFfbvBj1WTMGCFkAFEF3_0000)](https://www.plantuml.com/plantuml/svg/xLbhRzfC5lxC_8gfkjecQIv0cekwMwdKWGWj40Ha-w6gqDW-W3UsnpipXhBT_FVtU63m9JYaglesaTulJJpFSoxdoJd70_qe5HCg3dnoyeQGBdTZ7zvNBAOWP9-OXEF8Pw76IIiE7UNny3sfz9F3ssw72fY0WD01AdaiyCU42pfeDgwwpUF0fGd5yPcKrFNOLB3Wc5I8nQEbyAOpXOw609Gv3WyY5YwzS4exRQlP6pRfnFC1kOrOpRX0uWY4MD8kvm8EQTUosjJY8kA29HbLUdmEWGs2rgkrIq9wN87Cfka5R0gIUc6IM_TAvyO4q2c4W2x0fVQIHaog90KrywIEZtp5IKNDi3eGStJF59qf5Rq_ELai5XjfZZ7_4_DmI5deKgpLjbVEi0m1ClQrxzj1HnvJEfebdfYaYbCRgCl390H6O3vLNW3oa2uyDKFL9bfVHIfpviTO0hWUfgipItpJEHCUitr4RF3vuXXT0yLy0ZJI1M_-6AIoy7nq3_ZFaiTqWJcyaJG4SC4z9kJD2I6l_WPXugshrPJq0wrkpeOgEAtdJrZWua6Da91XpX5pW7OTnrjr3_rE2BEb4inHwoP8smf1IjyY62FOl6sVlnlRSLBhMFtZ0O8Fg5qUkGsz_yRGljgOVVcw3JQs1eOm3kXDlpbeZDgzwt6hqxWUfa2_qnYrRWRTpFbtgPZodFiO5J-jZnN5dbBOv47BPrCvlWQ5CzEB4xdszdCMDkS-TNWe5NoBHDxkxRWjqGw5NnlDkUVI8Pj07zDLa_LfOcr4-BIkanvWpE5Iuga9Q5I-N_6jngZPQunDCTfW4vsBzjK3EnqTMKOr4tx615gdvoin3Reps9guaGwXcoYPDtIYPSWLNm9chNCQRg_2XgaNbWYpgcVTXJqMmDu-rSWsHqCV87heg2LOG0-ce99Vilb7tFUppm8JaGh4ZVqVSDGQOQvhuMooq3NfzmMVUzBZ8POzL4iVT9t5mpLPjnJY3JlAKjhXX1SODugn4ehEhWTgoERGid0_HYN05L-4Mw0XoCHfCIYMrk43zfc5fiB2F6I6_o1A7nBEwuBsHhkyfo9hcZR4A5b7fImhYjlkDgep1E4nVm3CNLQ2SUP9m_czH96WnsvbskU-Zy8sG-mtG2pxF5rxLm9Oq0xn72o4mL5S87Np-ugshdB5HZZtd52Ep0mWc9yHZPOCOZ2EJCsQfjMGR0xPyJJdqzoAq88by3rvvKteNcxVuyBVtxxO4xnUez_p8qDrUsJdCdEmgOpFGGZFXIwJTnlMNP4LMpumaN0EQNeu19MQbUoouam_hUMELeKDHXPAFr2ELWH2rvkGh3BctLWkZIOyIsLs2r5Qz8vsQ05JCn3TAjPjth2DNgDpSptkjtkPr-vL-xezQdJIqmUwJ7pE54qcyzysKzcr_Rguv8jRJx-H7gx2dRPwJsXZlTOQlJjOhiNONkvdD4fU2lMxNrdls-8RA7M0HvyuLnQFGxNtTzlykYhriIPtrcbZZxV4mvMpSVfptT_1FYWM-nCLPUQeHAroKYnpfE9ks2tMvMwnD90h7_T7kaLAckZTsTctNze-PJBSboLqNdjk2Pq-iuGkgxybeTrREPVbknhENtb2sHlozWlowoVlICtUTVawClV2ChVJk_7Qw_XVw7QBZTM_RLzjRZpZRgDFvphSY9k0NxvkiPFU_s2iQ5Wa9v8MHAl_9TgFYtQ6eZrHdlAB_0-BKRW0xntGrVdScCelN_SVAypmVfDgvp-lsYyLvM765yyzuztNbSAxj_RKWlv4cSzhBwmnCEFQYyku_k8oldXn6L--j8nFNzhaNLQV-UJjlhHclYRyv_-FZd9VBUrOvsN6ch4xRXf76nU_nIXbf1_yIS74iCm5_76yZdZ1OPvmjejmlejmKNsYXAkJeeQTAiaz6OBvv5gEulG_YfwMe_e35Y5fb4BK6ab3549Mow57hEhbq4KvT4dIMWgrLKbQIA6mJKlVcxz8uJ_WFj3gFfbvBj1WTMGCFkAFEF3_0000 "Click for SVG image of SAMD Class Relationships")

**Catena SAMD Class Hierarchy (full detail):**
<!-- $enableStm32 = 0, $enableSamd = 1,  $enableMembers = 1 -->
[![**Catena SAMD Class Hierarchy (full detail):**](http://www.plantuml.com/plantuml/png/xLbjSzf64FxC_OhBfDFOZby0no-JDZCXCdYO0cO0Dnyo6UOaBQ1Qqgbt9mXD_T-xEX1wCRBiTfBOC_qIM_Sy-xAFTrS7UIyL4ohqN7BqYf0kjqCNtbOCfi1d7vY4my1bliP9A_GjvN3_BQdqeyFhReSAc800tm8gUIZmnuGBEcWsBhhDGy-c4SLocPJKTjXKCE-GL8Z1WwLmfZE5ZeO0b5aMzmBcBnr_IZjjezaRDkd4SG6vZL3Dk43Y28HOqYxd0lPfrp3QrE0Yu8956LLwV0wU2OBMgxLpGdfSWKodwNXi2f8wVfHRzqBdnWJGAVY0Bi2cvf86JAee1JLpf8wFVCL9HSsmEX1pTCyKdIaLl3qwMYmM6saECV-Z-66VCj-cMAjfhfnX6G9aw6hVruEEFAHqD8iyCKaLfoPGc_jH28p0NAeS3-G-NJXgXgfDjBwALERC3R44i1rCLsSM-QPp9XnckeYOuFB58Re6YlbuQ1GLdFmnI6NXkEWUy9yb3-a2StWbgGzWWtr8oAiZGbxy17xahQlLbFGThMtEXiexhkD9DJbXdgqfn6UOSy0ie5tBSbRTGxyImaof1BFKkWcIjaAGqjS8XWWshzkdRyPc6DKwLh_SGV01jSiYrxxpPmZjYutPfy_RuDWsXi4FFNhLRmuQetRlSjpgD2w72T3lD4Qjgq4tTVvLAgOSwpP4nO_hOqMnfnGskTTosLIEBq7XpFJ2IAwTtOo5oRbBBUvB1Ly2aRLxFMvBj4FXrqPpxjXqo2RGntJLP7qQMSSYV5ZNIVSmvd0fyJGE6Ajyk-8RZL6prnZ7nMY3JNGksXTtx7HqPCMgnU5dJA1rShuICqPBWws96-aGkWgYUKCdMePCyGLWsZgZuVOgJ9WwVe4mgthMNTXZ7kpiKeriSpHq0OAxZbg2UNHl2Yhw9Pr_m5qt_Imm4Qb0N9b_WANM2BDj0sUJ-NQSVb_mkICTxcFPGxLqGTUPFroJTKiXtZ23DANjJtYESQKOAw3ex7gWXcmEBGFtOr00NF25lmKQWeoSveDYQHq-O1-PdwimdeVKyEy5oKF4UPdJFjOkwodFcYODCOhMKI7B2CAslOrg3K4up1q0ivSLRvnwqd1sBr4awB5RcVQvwwAmJH_x3H1BFq_NtfK0vhLzF0S3OR0K5qZT_BwYhQjSiH7EF4UKWtW64CpEY4OB1j4R1t7DcgRLa6mEoNWcvzFCYj22HV0jUU5Cw4vct-F2tzs-s2EyNgDVioD3TNkavp9riAcCpq48nuOkapSRrasU5HekC15nzcboE0ILc1NiijnCFx3b3bQ53KO6IZzGbbO4VDkPa4IPvDrOB8qcF4bboeKeBBga7Lh0r0n4jufrns_OHg_HkReSzzkzr6ltedtP7ZKwoUaTNIOkPuf6a_cxoLHwRR_CB_dyrjDlfBkhiDHMxmbjhDTQetS3oxKOsylzY4RHIw5-yothVPr_0oKEyEW3vyhWeQzsVZRZNrUbtjVarZfjxF6MkBjoDax_M_Tti0_onVxhYZ9pLAnM0IcK6L9-DvIBTLukbWOovUF-IBP8GHEzEJdvyatRfqY6swA4JcjFBQ7Z9vRGUVM795I-bJDPlgdX_1KdbBuXRxyWltp4ZaHssUNh8BuN5hcTtepNNiU_eTijDaR_kdsnkV6Ckuq-dUjm8nu7_FHvYvrq_eAnedwU74cQ4wt-lsYF5-q4HNkWFCKN-KUBaRi0x-pHrVayDfMVFk_ULrZC-q6gdJvUjUygojsCpvvwnkNNbToxj_RGWhwZpAUrPzOOc77jsMLSVtOPdpsxZC-VMyR7pstopgjFVFBABwsfhmb__VlW8FFLKiawBpBMZFAuIHnjdFyMev2HVF0dEPEO5L_0xyVhYESSPWadPOJJCi9PzO4Ihaxo6dQg9FCK4-9FhiKeJl-zw74ngZzW4P94oKMjaIH4BcIr6Bh7gbuCdHL3voIf9LTRbII5v0hRjFHj_1V9_GVSErhT9JF71kgjZcA3z_WZzDn_0000)](https://www.plantuml.com/plantuml/svg/xLbjSzf64FxC_OhBfDFOZby0no-JDZCXCdYO0cO0Dnyo6UOaBQ1Qqgbt9mXD_T-xEX1wCRBiTfBOC_qIM_Sy-xAFTrS7UIyL4ohqN7BqYf0kjqCNtbOCfi1d7vY4my1bliP9A_GjvN3_BQdqeyFhReSAc800tm8gUIZmnuGBEcWsBhhDGy-c4SLocPJKTjXKCE-GL8Z1WwLmfZE5ZeO0b5aMzmBcBnr_IZjjezaRDkd4SG6vZL3Dk43Y28HOqYxd0lPfrp3QrE0Yu8956LLwV0wU2OBMgxLpGdfSWKodwNXi2f8wVfHRzqBdnWJGAVY0Bi2cvf86JAee1JLpf8wFVCL9HSsmEX1pTCyKdIaLl3qwMYmM6saECV-Z-66VCj-cMAjfhfnX6G9aw6hVruEEFAHqD8iyCKaLfoPGc_jH28p0NAeS3-G-NJXgXgfDjBwALERC3R44i1rCLsSM-QPp9XnckeYOuFB58Re6YlbuQ1GLdFmnI6NXkEWUy9yb3-a2StWbgGzWWtr8oAiZGbxy17xahQlLbFGThMtEXiexhkD9DJbXdgqfn6UOSy0ie5tBSbRTGxyImaof1BFKkWcIjaAGqjS8XWWshzkdRyPc6DKwLh_SGV01jSiYrxxpPmZjYutPfy_RuDWsXi4FFNhLRmuQetRlSjpgD2w72T3lD4Qjgq4tTVvLAgOSwpP4nO_hOqMnfnGskTTosLIEBq7XpFJ2IAwTtOo5oRbBBUvB1Ly2aRLxFMvBj4FXrqPpxjXqo2RGntJLP7qQMSSYV5ZNIVSmvd0fyJGE6Ajyk-8RZL6prnZ7nMY3JNGksXTtx7HqPCMgnU5dJA1rShuICqPBWws96-aGkWgYUKCdMePCyGLWsZgZuVOgJ9WwVe4mgthMNTXZ7kpiKeriSpHq0OAxZbg2UNHl2Yhw9Pr_m5qt_Imm4Qb0N9b_WANM2BDj0sUJ-NQSVb_mkICTxcFPGxLqGTUPFroJTKiXtZ23DANjJtYESQKOAw3ex7gWXcmEBGFtOr00NF25lmKQWeoSveDYQHq-O1-PdwimdeVKyEy5oKF4UPdJFjOkwodFcYODCOhMKI7B2CAslOrg3K4up1q0ivSLRvnwqd1sBr4awB5RcVQvwwAmJH_x3H1BFq_NtfK0vhLzF0S3OR0K5qZT_BwYhQjSiH7EF4UKWtW64CpEY4OB1j4R1t7DcgRLa6mEoNWcvzFCYj22HV0jUU5Cw4vct-F2tzs-s2EyNgDVioD3TNkavp9riAcCpq48nuOkapSRrasU5HekC15nzcboE0ILc1NiijnCFx3b3bQ53KO6IZzGbbO4VDkPa4IPvDrOB8qcF4bboeKeBBga7Lh0r0n4jufrns_OHg_HkReSzzkzr6ltedtP7ZKwoUaTNIOkPuf6a_cxoLHwRR_CB_dyrjDlfBkhiDHMxmbjhDTQetS3oxKOsylzY4RHIw5-yothVPr_0oKEyEW3vyhWeQzsVZRZNrUbtjVarZfjxF6MkBjoDax_M_Tti0_onVxhYZ9pLAnM0IcK6L9-DvIBTLukbWOovUF-IBP8GHEzEJdvyatRfqY6swA4JcjFBQ7Z9vRGUVM795I-bJDPlgdX_1KdbBuXRxyWltp4ZaHssUNh8BuN5hcTtepNNiU_eTijDaR_kdsnkV6Ckuq-dUjm8nu7_FHvYvrq_eAnedwU74cQ4wt-lsYF5-q4HNkWFCKN-KUBaRi0x-pHrVayDfMVFk_ULrZC-q6gdJvUjUygojsCpvvwnkNNbToxj_RGWhwZpAUrPzOOc77jsMLSVtOPdpsxZC-VMyR7pstopgjFVFBABwsfhmb__VlW8FFLKiawBpBMZFAuIHnjdFyMev2HVF0dEPEO5L_0xyVhYESSPWadPOJJCi9PzO4Ihaxo6dQg9FCK4-9FhiKeJl-zw74ngZzW4P94oKMjaIH4BcIr6Bh7gbuCdHL3voIf9LTRbII5v0hRjFHj_1V9_GVSErhT9JF71kgjZcA3z_WZzDn_0000 "Click for SVG image of SAMD Class Hierarchy")

### Platform Management

The hardware supported by this platform is generally similar. The architecture allows for the following kinds of variation (as outlined in the class hierarchy):

1. CPU differences (Cortex M0, RISC-V, etc.)
2. SOC differences (SAMD21, STM32L0, etc.)
3. PC-board differences (different sensors, power supply, capabilities)
4. Optional component population differences (pull-up resistor values, etc.)
5. Externally-connected sensors (one-wire temperature sensors, etc.)

Items 1-3 are to some degree known at compile time, based on the Catena model chosen. However, it's inconvenient to update the BSP for every possible modification, so we allow some variation at run time, guided by the contents of FRAM.

The system is identified to the software by a platform object, of type `CATENA_PLATFORM`. Several platform objects are built into the firmware image, based on the known variations for component population and external sensors. The appropriate platform object is located at boot time by the Catena Arduino Platform framework. Some values representing possibly variation are stord as `PlatformFlags` in the `CATENA_PLATFORM`. This variable is of type`PLATFORM_FLAGS`.

#### Platform GUIDs

Each `CATENA_PLATFORM` has a unique identification. This is a 128-bit binary number called a GUID (or [UUID](https://en.wikipedia.org/wiki/Universally_unique_identifier)), generated by MCCI during the system design process. The platform GUIDs are defined in the header file [`Catena_Guids.h`](./src/Catena_Guids.h). For convenience, here's a table of the known GUIDs.

All of these names begin with the string `GUID_HW_`, so we omit that from the tables below.

The M101 and M102 designations are used by the [Catena-Sketches](https://github.com/mcci-catena/Catena-Sketches) family of applications to determine what external sensors are available. This lets them avoid trying to poll external hardware unless the appropriate platform configuration is set. The well known configurations are:

- **M101**:  external sensors on screw terminals are contact closures, and should be totalized.
- **M102**: external sensors on screw terminals are a mixture of one-wire temperature sensors and two-wire soil temperature/humidity probes.

The designations "M103" and "M104" are reserved for use by MCCI.

For boards with FRAM, the appropriate platform GUID should be selected and programmed into FRAM using the command `system configure platformguid`, followed by the GUID value. For boards without FRAM, the library has provisions for tying the GUID to the CPU serial number. Contact MCCI for details.

The tables below were generated from `Catena_Guids.h` using a script, and then hand annotated. The script is a one-line shell command using awk:

```bash
awk '/^[/][/]/ {
    s = $2; gsub(/[{}]/, "", s);
    getline;
    g = $2; gsub(/\(f\)$/, "", g);
    gsub(/^GUID_HW_/, "", g);
    printf("`%s` | `%s` |\n", g, tolower(s));
    }' src/Catena_Guids.h | LC_ALL=C sort
```

#### GUIDs for the Catena 461x family

##### Catena 4610

The Catena 4610 uses a LiPo battery like traditional Feathers, and includes a BME280 temperature/pressure/humidity sensor, and a Si1133 light sensor.

Name | GUID | Description
-----|------|------------
`CATENA_4610_BASE` | `53ca094b-b888-465e-aa0e-e3064ec56d21` | Base Catena 4610, assuming no modifications or customizations.
`CATENA_4610_M101` | `6a5d8d0c-d5ae-4143-adc7-8f84ec56a867` | Catena 4610 M101, configured for power monitoring and other pulse-input applications.
`CATENA_4610_M102` | `18252b1c-3c0d-403e-8012-224d96c5af06` | Catena 4610 M102, configured for environental monitoring
`CATENA_4610_M103` | `c2cf6cf4-a4c3-4611-941f-6955ffa5bfdc` | Catena 4610 M103 -- contact MCCI
`CATENA_4610_M104` | `bfed4740-a58a-4ef6-933a-09cb22e93d00` | Catena 4610 M104 -- contact MCCI

##### Catena 4611

The 4611 uses a boost regulator that is either on or fully off, controlled by the enable pin. It's therefore a hybrid between the 4610 (which uses a battery charger switch controlled by the enable pin), and the 4612 (which instead uses the switch to jump from raw Vbat to regulated 3.3V). The 4611 is available by special order from MCCI. Generally, MCCI uses the 4612 instead.

Name | GUID | Description
-----|------|------------
`CATENA_4611_BASE` | `9bb29dca-0685-4837-8182-3dfa309d279f` | Base Catena 4611, assuming no modifications or customizations.
`CATENA_4611_M101` | `4e995471-1570-4767-adae-6657ef871bcd` | Catena 4611 M101, configured for power monitoring and other pulse-input applications.
`CATENA_4611_M102` | `964bcf91-9c45-4386-a6e7-5f2d7c3641ef` | Catena 4611 M102, configured for environmental monitoring.
`CATENA_4611_M103` | `c85b27cb-7cf9-4025-92bb-2009c08449e5` | Catena 4611 M103 -- contact MCCI
`CATENA_4611_M104` | `c22be8af-e693-4319-b243-1c2d10197973` | Catena 4611 M103 -- contact MCCI

##### Catena 4612

The 4612 runs off an unregulated battery supply, with the option of a boost regulator that can bring the system voltage up to 3.3V.

Name | GUID | Description
-----|------|------------
`CATENA_4612_BASE` | `915decfa-d156-4d4f-bac5-70e7724726d8` | Base Catena 4612, assuming no modifications or customizations.
`CATENA_4612_M101` | `d210a354-c49a-4c4f-856a-4b545dcfaa20` | Catena 4612 M101, configured for power monitoring and other pulse-input applications.
`CATENA_4612_M102` | `7fa9709d-17af-463e-ae7f-8210e49acd7a` | Catena 4612 M102, configured for environmental monitoring.
`CATENA_4612_M103` | `ff8b2ac6-75cd-4ed3-980b-50b209e64551` | Catena 4612 M103 -- contact MCCI
`CATENA_4612_M104` | `dea48489-cdac-43f4-b8ad-edb08ce21546` | Catena 4612 M103 -- contact MCCI

##### Catena 4617

The Catena 4617 is a variant of the Catena 4612 with a IDT HS3001 temperature/humidity sensor in place of the Bosch BME280.

Name | GUID | Description
-----|------|------------
`CATENA_4617_BASE` | `6767c2f6-d5d5-43f4-81af-db0d4d08815a` | Base Catena 4617, assuming no modifications or customizations.

##### Catena 4618

The Catena 4618 is a variant of the Catena 4612 with a Sensirion SHT31-DIS-F temperature/humidity sensor in place of the Bosch BME280.

Name | GUID | Description
-----|------|------------
`CATENA_4618_BASE` | `b75ed77b-b06e-4b26-a968-9c15f222dfb2` | Base Catena 4618, assuming no modifications or customizations.

##### Catena 4630

The Catena 4630 is a variant of the Catena 4610. It deletes the Si1131 light sensor, and adds an IDT MOX4410 atmospheric gas sensor, plus a connection for an external Plantower PMS7003 PM2.5/dust sensor.

Name | GUID | Description
-----|------|------------
`CATENA_4630_BASE` | `17281c12-d78a-4e4f-9c42-c8bbc5499c91` | Base Catena 4618, assuming no modifications or customizations.

#### GUIDs for the Catena 4450/4460/4470 family

##### Catena 4450

The 4450 Feather Wing includes a BME280 temperature/humidity/pressure sensor, and a BH1750 lux sensor.

Name | GUID | Description
-----|------|------------
`CATENA_4450_BASE` | `60480acb-dc5d-4148-b6c9-aca13449cf1d` | Base Catena 4450, assuming no modifications or customizations.
`CATENA_4450_M101` | `82bf2661-70cb-45ae-b620-caf695478bc1` | Catena 4450 M101, configured for power monitoring and other pulse-input applications.
`CATENA_4450_M102` | `2281255e-ac5c-48cb-a263-9dc890d16638` | Catena 4450 M102, configured for environmental monitoring.
`CATENA_4450_M103` | `1fb2506f-0f2a-4310-9e6a-9bc191e0ae12` | Catena 4450 M103 -- contact MCCI
`CATENA_4450_M104` | `a731f637-e3ed-4088-a9a8-f54b6671dcf6` | Catena 4450 M103 -- contact MCCI

##### Catena 4460

The 4460 Feather Wing includes a BME680 air quality sensor, and a BH1750 lux sensor.

Name | GUID | Description
-----|------|------------
`CATENA_4460_BASE` | `3037d9be-8ebe-4ae7-970e-91915a2484f8` | Base Catena 4460, assuming no modifications or customizations.
`CATENA_4460_M101` | `31e563d1-0267-43fc-bca0-9a4cb5bfc55a` | Catena 4460 M101, configured for power monitoring and other pulse-input applications.
`CATENA_4460_M102` | `494f3c17-8ac1-4f80-8ecc-ca4dd3dccbdc` | Catena 4460 M102, configured for environmental monitoring.
`CATENA_4460_M103` | `a882186f-f4ab-4ee4-9402-7b628a76d886` | Catena 4460 M103 -- contact MCCI
`CATENA_4460_M104` | `398a9e5a-e22f-4265-9d35-bf45433ddbe3` | Catena 4460 M103 -- contact MCCI

##### Catena 4470

The 4470 Feather Wing includes a BME280 temperature/humidity/pressure sensor, a BH1750 lux sensor, and a Modbus/RS-485 interface connected to `Serial1`.

Name | GUID | Description
-----|------|------------
`CATENA_4470_BASE` | `ea8568ec-5dae-46ee-929a-a3f6b00a565e` | Base Catena 4470, assuming no modifications or customizations.
`CATENA_4470_M101` | `dd0a37a6-e469-43ec-b173-fed795129455` | Catena 4470 M101, configured for power monitoring and other pulse-input applications.

#### GUIDs for the Catena 4801 family

Name | GUID | Description
-----|------|------------
`CATENA_4801_BASE` | `10ea7e25-a4a4-45fd-8959-c04a6a5d7f95` | Base Catena 4801, assuming no modifications or customizations.

#### GUIDs for Adafruit Feather M0s

MCCI also uses this libray with Feather M0s without MCCI hardware. These GUIDs are useful in that situation.

Name | GUID | Description
-----|------|------------
`FEATHER_M0_LORA_TTNNYC` | `a67ad93c-551a-47d2-9adb-e249b4cf915a` | Feather M0 LoRa, modified per The Things Network NYC standards -- DIO1 connected to D6.
`FEATHER_M0_LORA` | `e2deccc8-55fa-4bd3-94c3-ce66bcd0baac` | Feather M0 LoRa, but DIO1 connection is not known.
`FEATHER_M0_PROTO_WINGLORA_TTNMCCI` | `3bab150f-6e32-4459-a2b6-72aced75059f` | Feather M0 Proto with a separate LoRa Feather Wing. This is sometimes known as an MCCI Catena 4420.
`FEATHER_M0_PROTO` | `f6a15678-c7f3-43f4-ac57-67ef5cf75541` | A Feather M0 Proto.
`FEATHER_M0` | `2e6dfed4-f577-47d5-9137-b3e63976ae92` | Some unspecified member of the Feather M0 family.

### Polling Framework

When composing software from components, it's inconvenient and bug-prone to have to manually edit the Arduino `loop()` function to poll each component.

To compensate, the Catena platform defines a framework for polling, and allows components to register to be polled.

The foundation of the framework is `cPollableInterface`, an abstract class. Any object inheriting from `cPollableInterface` will provide a `poll()` method; this provides a standard way to poll an object.

Pollable objects are managed via a central instance of `cPollingEngine`, which works on objects that are derived from `cPollableObject`. You create your pollable class by arranging for it to inherit from `cPollableObject`; then at run time you arrange to register  and normally created by arranging for them to inherit from `cPollableObject`. This adds a few tracking fields to your class, and makes them available to the `cPollingEngine` when you register the object with the polling engine.

The abstract relationships are shown below.

[![**Pollable framework UML Class Diagram **](http://www.plantuml.com/plantuml/png/bL9DRziw43nhVyN2btoyM4voUquG94_nGrDOQP2FSx4IrnOBYXH8oggGfh-zb0p7RjGswCNMahCpiyEzT9wihqi5aps0r8XQyJHAoBEc_yXnN2dI_JtyN-lSIwEd4DrrXq_f72qlsBBE6PsfRVXR68lvdL6ACiKtshDTK3ZE5Jc7GjBIKXb318cfMYkNXGzi3yn8UMxAYdGhzHBdDydizc662wauOAzgNnWRW8ziROkfCPQFC4sI6qoChOobpbRqiLOjdXwV_0jmQpoxNP-of2Kxb1hlPrVfzImk1P9bNBBcqCu2inOhShwJzuLqlNR0UmEHBnWTsnLR98-5zzLqCccQvZMRE7YVR8eZ92qmp9bbQpA6oBAhLS_zT3ztCU9ZqJ6HGsymOnH6Tn91WHHwMR8xmaRw0eLSz05xY5QaQdR8GWQxlREE6rTaPPUr2ppAD76WPWn-oj2q6VWwxGQ6g94gq7FuQKsDJV2mvr0p2nOnmDu4e922aSlAjrXocR5r09g2dufonZJFvNery1b0RbztyE3XFlhdcJBdBUN-1RyF8Vi7FqDMHWgi0ch_u0nU0DOhq_U5u6PwAJNZ-Vc6v-AYrzZjsfGP1-nERKFjWkgdx0ldVl_Lohh6mmuv7foMdwiFJABtid69MrCxs86hNvBTybV_Ew4vEN_sJXp3ZC-_ZSTl0lepRAiARt8rumiS7KyFHmFW2wndVd9ia-xhv9r763IS8ZaSnN4ymdTRjniM3q8EOuntRFm5zXJWahMeImK_0000)](https://www.plantuml.com/plantuml/svg/bL9DRziw43nhVyN2btoyM4voUquG94_nGrDOQP2FSx4IrnOBYXH8oggGfh-zb0p7RjGswCNMahCpiyEzT9wihqi5aps0r8XQyJHAoBEc_yXnN2dI_JtyN-lSIwEd4DrrXq_f72qlsBBE6PsfRVXR68lvdL6ACiKtshDTK3ZE5Jc7GjBIKXb318cfMYkNXGzi3yn8UMxAYdGhzHBdDydizc662wauOAzgNnWRW8ziROkfCPQFC4sI6qoChOobpbRqiLOjdXwV_0jmQpoxNP-of2Kxb1hlPrVfzImk1P9bNBBcqCu2inOhShwJzuLqlNR0UmEHBnWTsnLR98-5zzLqCccQvZMRE7YVR8eZ92qmp9bbQpA6oBAhLS_zT3ztCU9ZqJ6HGsymOnH6Tn91WHHwMR8xmaRw0eLSz05xY5QaQdR8GWQxlREE6rTaPPUr2ppAD76WPWn-oj2q6VWwxGQ6g94gq7FuQKsDJV2mvr0p2nOnmDu4e922aSlAjrXocR5r09g2dufonZJFvNery1b0RbztyE3XFlhdcJBdBUN-1RyF8Vi7FqDMHWgi0ch_u0nU0DOhq_U5u6PwAJNZ-Vc6v-AYrzZjsfGP1-nERKFjWkgdx0ldVl_Lohh6mmuv7foMdwiFJABtid69MrCxs86hNvBTybV_Ew4vEN_sJXp3ZC-_ZSTl0lepRAiARt8rumiS7KyFHmFW2wndVd9ia-xhv9r763IS8ZaSnN4ymdTRjniM3q8EOuntRFm5zXJWahMeImK_0000 "Click for SVG version")

#### Making a class pollable

Let's say that `UserClass1` exists and has the following definition.

```c++
class UserClass1 : public ParentClass {
public:
  // constructor
  UserClass1() {}
  void begin();
};
```

To make `UserClass1` pollable, you change the class declaration as follows:

1. Change `UserClass1` to inherit from `McciCatena::cPollableObject`, using multiple inheritance if needed. List `McciCatena::cPollableObject` last. No need to make it `public`.

2. Declare a new public virtual method `void poll()`. We recommend that you use the `override` keyword to make it clear that this is an override for a method declared in the parent class.

You also, of cousre, must supply an implementaion of `UserClass::poll()`.

Here's an example of the transformed `UserClass1`:

```c++
class UserClass1 : public ParentClass, McciCatena::cPollableObject {
public:
  // constructor
  UserClass1() {}
  void begin();

  // <<Pollable>> requirements:
  virtual void poll() override;
};
```

#### Using pollable objects in sketches

Each instance of your pollable class (in our example, each instance of type `UserClass1`) must be *registered* with a polling engine. The most convenient polling engine to use is the once provided by the `CatenaBase` class, which is normally instantiated as `gCatena`.  Simply call `gCatena.registerObject()` to register your object with the Catena polling engine. So for example:

```c++
#include <Catena.h>

// create the gCatena instance.
McciCatena::Catena gCatena;

// create an instance of my object.
UserClass1 myUserObject;

void setup() {
  // conventionally, we call gCatena.begin() first:
  gCatena.begin();

  // now register the object.
  gCatena.registerObject(&myUserObject);
}

void loop() {
  // poll all the objects registered with gCatena.
  gCatena.poll();
  // do any other work...
}
```

If you're not using the full `Catena` class framework, you still can use polling; just declare your own `cPollingEngine` instance.  For example:

```c++
#include <Arduino.h>
#include <Catena_PollableInterface.h>

// create the polling engine instance.
McciCatena::cPollingEngine gPollingEngine;

// create an instance of my object.
UserClass1 myUserObject;

void setup() {
  // conventionally, we call gPollingEngine.begin() first:
  gPollingEngine.begin();

  // now register the object.
  gPollingEngine.registerObject(&myUserObject);
}

void loop() {
  // poll all the objects registered with gCatena.
  gPollingEngine.poll();
  // do any other work...
}
```

### Finite State Machine (FSM) Framework

Finite state machines are very useful when implementing non-blocking asynchronous programs, or modeling external hardware that changes state independently of the system.

We've ported an implementation of MCCI's standard FSM approach. It's good for Mealy or Moore designs, and is intended to be easy to implement and maintain.  This version is C++ oriented; it assumes that each FSM instance is associated with a C++ class object (the "parent" object). The work of implementing the FSM is divided between the parent object and the FSM object. The FSM object does all the bookkeeping and handling of corner conditions; the parent object provides a method function, which the FSM calls whenever it seems appropriate to do so.

Here's what you need:

- an `enum class` type with all of your states, and a couple of distinguished states. This type is referred to as `TState`; and
- an associated C++ `class` type (referred to as `TParent`), containing...
- a method that will be used as the dispatcher by the evaluator, of signature `TParent::someName(TState currentState, bool fEntry) -> TState`.

In addition to the states relevant to your problem, `TState` must have three distinct values with well-known names.

- `TState::stNoChange` does not correspond to any state. Instead, it's the value retured by the dispatch routine when the state is not to be changed. (Returning the current state will cause the FSM to transition from the current state to the current state.)

- `TState::stInitial` is the initial state of your FSM.

- `TState::stFinal` is the final state of your FSM. Once the FSM reaches this state, it will remain there until another call to `fsm.init()` is made.

We'll use the [coin-operated turnsile](https://en.wikipedia.org/wiki/Finite-state_machine#Example:_coin-operated_turnstile) example from Wikipedia to make things more concrete.

#### Getting ready

The author usually starts by drawing a diagram, labled with the states and transitions.

For example, here's the turnstile state diagram:

[![**Coin Operated Turnstile State Diagram](http://www.plantuml.com/plantuml/png/XP9FRvmm4CNFpAUOMwGg2srltKELY6oaL4iKvS-fxS7WMR1gR6IFboBrmtTW5RHQLUe5u_6ytxndk8ci0gVUGd45K7cTB6sp-vUAVgj-i9GFLhdb7EwJQzXujuNiQIw-LNiCTA10hY6CFWLP3ZvW8t8KBXDgezgW-XmoAFqm1TDsBFeN8bHDu_j1kScu5lSFvUxnPOS7O-M48UkOXxZT5aLhk4jrBhr5tpHcqmZMgQ9SbirjqCauln53BADxcNERkFD1XhnI21DMtWUwngei7x3qOV11pI6oRybE-FdZfoy0ZvufdgSolMefed7ulBkjxdPvhr45mfOSYPmqrXCEAl9idJiJJxwDOmyPTuIHmf621C4vXwGOnt6zoINB--OQbTCeTrJN9nX15YWckx3VdlSnHtpjPfAAo1vhGktTF48cA8jiUehNE1hkK9l3yZaOigEoIIAGDc9tSJQpyQY2KRMbA1phnrpGXEAd5z5xuBjg3WpPQApWIHwJJpYAXwk8ZaYJpW6k2e3l7txYPlCL8-zzyqlcRENrmHasoW8iVy1wjcxVd3qLO9LTCEZub687PvMTup0LE0kN69o2YsmiNJ9c-4eflN5d3POEB4spQV7P9TP-T3-4Ss_SoQ-e_mUJseMfvFvbwu9r6Gx_h0xWXfnElOM_)](https://www.plantuml.com/plantuml/svg/XP9FRvmm4CNFpAUOMwGg2srltKELY6oaL4iKvS-fxS7WMR1gR6IFboBrmtTW5RHQLUe5u_6ytxndk8ci0gVUGd45K7cTB6sp-vUAVgj-i9GFLhdb7EwJQzXujuNiQIw-LNiCTA10hY6CFWLP3ZvW8t8KBXDgezgW-XmoAFqm1TDsBFeN8bHDu_j1kScu5lSFvUxnPOS7O-M48UkOXxZT5aLhk4jrBhr5tpHcqmZMgQ9SbirjqCauln53BADxcNERkFD1XhnI21DMtWUwngei7x3qOV11pI6oRybE-FdZfoy0ZvufdgSolMefed7ulBkjxdPvhr45mfOSYPmqrXCEAl9idJiJJxwDOmyPTuIHmf621C4vXwGOnt6zoINB--OQbTCeTrJN9nX15YWckx3VdlSnHtpjPfAAo1vhGktTF48cA8jiUehNE1hkK9l3yZaOigEoIIAGDc9tSJQpyQY2KRMbA1phnrpGXEAd5z5xuBjg3WpPQApWIHwJJpYAXwk8ZaYJpW6k2e3l7txYPlCL8-zzyqlcRENrmHasoW8iVy1wjcxVd3qLO9LTCEZub687PvMTup0LE0kN69o2YsmiNJ9c-4eflN5d3POEB4spQV7P9TP-T3-4Ss_SoQ-e_mUJseMfvFvbwu9r6Gx_h0xWXfnElOM_ "Click for SVG view")

#### Defining the state enum

Define an `enum class` as follows:

```c++
enum class MyStateEnum {
  stNoChange = 0,    // this name must be present: indicates "no change of state"
  stInitial,         // this name must be presnt: it's the starting state.
  // use-case-specific states
  // ...
  stFinal,           // this name must be present, it's the terminal state.
};
```

For example, for the turnstile diagram:

```c++
enum class State {
  stNoChange = 0,    // this name must be present: indicates "no change of state"
  stInitial,         // this name must be presnt: it's the starting state.
  stLocked,
  stUnlocked,
  stFinal,           // this name must be present, it's the terminal state.
};
```

#### Identify the parent class

This means finding the class name for the class that is going to contain this FSM. One class can contain many FSMs, but each FSM class has only one parent class.

For our example, we'll say that the class modeling turnstiles is `Turnstile`.

#### Add the state enum to the parent class

Add the enum you defined above, and add it to the parent class. For example:

```c++
class Turnstile {

  // states for FSM
  enum class State {
    stNoChange = 0,    // this name must be present: indicates "no change of state"
    stInitial,         // this name must be presnt: it's the starting state.
    stLocked,
    stUnlocked,
    stFinal,           // this name must be present, it's the terminal state.
  };

};
```

#### Define the FSM instance in the parent class

Add an FSM instance as a member of the parent class. It's up to you whether to make it `public`, `private`, or `protected`.

```c++
class Turnstile {

  // states for FSM
  enum class State {
    stNoChange = 0,    // this name must be present: indicates "no change of state"
    stInitial,         // this name must be presnt: it's the starting state.
    stLocked,
    stUnlocked,
    stFinal,           // this name must be present, it's the terminal state.
  };

  // the FSM instance
  McciCatena::cFSM<Turnstile, State>  m_fsm;
};
```

#### Declare a method function in the parent class

Finally, we have to declare a method function that the FSM can call.  Extending the turnstile example again:

```c++
class Turnstile {

  // states for FSM
  enum class State {
    stNoChange = 0,    // this name must be present: indicates "no change of state"
    stInitial,         // this name must be presnt: it's the starting state.
    stLocked,
    stUnlocked,
    stFinal,           // this name must be present, it's the terminal state.
  };

  // the FSM instance
  McciCatena::cFSM<Turnstile, State>  m_fsm;

  // the FSM dispatch function called by this->m_fsm.
  State fsmDispatch(State currentState, bool fEntry);
};
```

#### Implement the FSM dispatch function

Your FSM dispatch function will look like this.

```c++
void Turnstile::fsmDispatch(Turnstile::State currentState, bool fEntry) {
  State newState = State::stNoChange;

  switch (currentState) {

  case State::stInitial:
    if (fEntry) {
      // entry is not considered in this state, always move on.
    }
    digitalWrite(LOCK, 1);
    pinMode(LOCK, OUTPUT);
    newState = State::stLocked;
    break;

  case State::stLocked:
    if (fEntry) {
      digitalWrite(LOCK, 1);
    }
    if (this->m_evShutdown) {
      this->m_evShutdown = false;
      newState = State::stFinal;
    } else if (this->m_evCoin) {
      this->m_evCoin = false;
      newState = State::stUnlocked;
    } else if (this->m_evPush) {
      this->m_evPush = false;
      // stay in this state.
    } else {
      // stay in this state.
    }
    break;

  case State::stUnlocked:
    if (fEntry) {
      digitalWrite(LOCK, 0);
    }
    if (this->m_evShutdown) {
      this->m_evShutdown = false;
      newState = State::stFinal;
    } else if (this->m_evCoin) {
      this->m_evCoin = false;
      // stay in this state.
    } else if (this->m_evPush) {
      this->m_evPush = false;
      newState = State::stLocked;
    } else {
      // stay in this state.
    }
    break;

  case State::stFinal:
    // by policy, we idle with the turnstile locked.
    digitalWrite(LOCK, 1);
    // stay in this state.
    break;

  default:
    // the default means unknown state.
    // transition to locket.
    newState = State::stLocked;
    break;
  }
  return newState;
}
```

#### Implement the FSM intialization

Somewhere in your initialization for `Turnstile`, add the following code. For example, if `Turnstile` has a `Turnstile::begin()` method, you could write:

```c++
void Turnstile::begin() {
  // other init code...

  // set up FSM
  this->m_fsm.init(*this, fsmDispatch);

  // remaining init code...
}
```

### LoRaWAN Support

*To be supplied*

### FRAM Storage Management

Many MCCI Catena models include FRAM storage for keeping data across power cycles without worrying about the limited write-tolerance of EEPROM or flash. (FRAM, or ferro-electric RAM, is essentially non-volatile memory that can be freely written. Flash EPROM and EEPROM can be written, but tend to have non-local error properties and limited write durability. They are good for storing code, but troublesome for storing counters, because a location must be updated each time a counter is written.)

The abstract class `cFram` is used to represent a FRAM-based storage element. It is abstract in that is uses several virtual methods that must be supplied by the concrete class that represents the specific FRAM chip. (For example, `cFram2K` represents a 2k by 8 FRAM.)

#### FRAM Storage Formats

All FRAMs managed by `cFram` use a common object format on the FRAM, defined by the header file `Catena_FramStorage.h`.

- Storage is viewed as a linear sequence of objects.

- Each object uses a common format.

- Each object consists of a common 24-byte header followed by a variable-length storage field.

- Objects are always a multiple of 4 bytes long.

- Objects are identified by "globally unique ID" (or GUID) and "key" (an 8-bit value). GUIDs are 16-byte values, generated by a standard algorithm with low likelihood of collision. We considered using one GUID for each object, but that would consume a lot of room in system flash memory. So instead, we use an extra one-byte key, which allows most objects to share a common GUID. This approach allows for more space-efficient code on systems with limited system memory.

Each standard object contains a data payload. For any given object, the payload size is fixed when the object is created.

Objects normally contain two payload slots. The slots are written alternately (so that the old version is always available). A voting scheme is used to determine which slot is currently live. Three bytes are used for storing the "current" slot indicator, and are updated only after the new data have been written. A system interruption before the second byte of the trio is written will cause the system to use the old value after recovering from the problem; a system interruption after the second byte of the trio is written will cause the system to use the new value.

The first `uint32_t` of an object records the overall size of the object, and the size of each data payload slot.  Objects are always required to be a multiple of 4 bytes long, so the size is recorded as a count of `uint32_t` values. Objects are allowed to be up to 2^18 bytes long. Data payload fields are specified in bytes, and are limited to [0..32767] bytes.

There is an escape clause. If bit 31 of the first `uint32_t`is set, the object is not "standard". In such a case, the contents of the object after the standard header cannot be used for a standard data payload (as defined above). This may be desirable payloads that are written only once, when the FRAM is intialized; but it leaves redundancy management to the client.

This format is summarized in the following tables.

##### Object Storage Structure

| Bytes | Name | Type  | Description |
|:-----:|:----:|:-----:|:------------|
| 0..3  | `uSizeKey` | `uint32_t` | The size of the overall object, and the size of a datum within the object. This item is stored in little-endian format.  The bit layout is shown below. |
| 4..19 | `Guid` | `MCCIADK_GUID_WIRE` | the 16-byte globally-unique ID of the object. This GUID is stored in _wire_ order (big endian). |
| 20 | `Key` | `uint8_t` | An additional byte of name, allowing up to 256 objects to be defined by a single common GUID. |
| 21..23 | `uVer[3]` | `uint8_t[3]` | Array of current slot indicators. Normally these are all identical and either 0x00 or 0x01. However, after a system upset, it is possible that these will not be the same. If `uVer[0]` is equal to `uVer[1]`, then the slot is selected by the value of these bytes. Otherwise, the slot is selected by the value of `uVer[3]`. |
| 24.._size_-1 | - | - | Reserved space for the data payload. Slot zero starts at byte 24 and runs for the number of data bytes defined by bits 30..16 of `uSizeKey`. Slot one starts immediately after slot zero. |

##### Bit layout of `uSizeKey`

| Bits | Name | Mask | Description |
|:----:|:----:|:----:|:------------|
| 15..0 | `Size` | `cFramStorage::SIZE_MASK` | The size of the object in "clicks". Each click is four bytes. |
| 30..16 | `DataSize` | `cFramStorage::DATASIZE_MASK` | The size of the object's data payload in bytes. This may be zero. |
| 31 | `fNonStandard` | `cFramStorage::NONSTD_MASK` | If zero, the object's payload uses the redundant scheme described above; the payload size is necessarily limited to 32767 byes. If non-zero, the object's payload uses a client-supplied encoding and representation; but can use up to 256 kbytes (since the object size can represent up to 256 kbytes) |

##### The FRAM header object

An FRAM store managed by this library is expected to begin with a header object. A header object is identified by the well-known GUID `{1DE7CDCD-0647-4B3C-A18D-8138A3D9613F}` and the key `kHeader` (zero).

The header object carries a single 4-byte (`uint32_t`) payload, which is interpreted as the end-of-storage address -- the offset of the first byte on the FRAM that is not used for object storage. If an object is added to the store, this pointer is updated after the new object object has been fully committed. The new object is not permanently committed until the end-of-storage pointer is atomically updated.

#### Adding FRAM objects

1. Determine the GUID and key you want to use. If you are adding the item as part of the Catena library, you can use the GUID `GUID_FRAM_CATENA_V1(WIRE)`, `{1DE7CDCD-0647-4B3C-A18D-8138A3D9613F}`; add the key to `McciCatena::cFramStorage::StandardKeys`, defined in `Catena_FramStorage.h`.

   There is no presentable way to use a non-standard GUID; several changes must be made in `Catena_Fram.cpp` to enable this.

2. Ultimately, the metadata for your new object is represented by a 32-bit value of type `cFramStorage::StandardItem`. The constructor takes three (optionally four) arguments:

   - `uKey`, the 8-bit key value
   - `uSize`, the 16-bit object size. (If your object is variable size, you must specify a maximum size, and the actual size of the object must be represented as part of the object data somehow.)
   - `fNumber`, a boolean value. If true, then the value represents a little-endian value; if false, big-endian. This is used for displays and the command interpreter.
   - Optionally `fReplicated` (assumed `true`), which controls whether the replicated data-storage scheme should be used.

3. Find the table `McciCatena::cFramStorage::vItemDefs[]` in `Catena_FramStorage.cpp`, and add your `StandardItem` value at the appropriate offset.

4. To query the value of your object, you can use `gCatena.getFram()->getField(uKey, Value)`; this is a templated function which will set Value according toe the curent value stored for `uKey`.

   - You may also use `gCatena.getFram()->getField(uKey, (uint8_t *)&buffer, sizeof(buffer))`.

5. To set the value of your object, you can use `gCatena.getFram()->saveField(uKey, Value)`; this is a templated function which will write Value to the object identified by `uKey`.

   - You may also use `gCatena.getFram()->saveField(uKey, (const uint8_t *)&buffer, sizeof(buffer))`.

#### Class hierarchy within the FRAM library

<!-- The following image renders well on github, but doesn't render in the previewer in vscode unless you turn on "insecure content". Of course, github rendering is more important, but this is irksome -->
[![Image of cFram -- see assets/cfram.plantuml](http://www.plantuml.com/plantuml/png/ZLLXRzem4FsUNs5fqihAK3KcJHDKKoiw9CJGAjZwPPgYmpRKKcmZsw5YZF--ivCGGaEoBpHzztn-xt4Nxgm3urAPH3TNv341vNxElX4XOSt9IXiodj_FegkRI87aTWEKancbOWIEAo3x29RM6Q2Eq0Ii9VIH6oii5jt2QUERx2D2klk2To1RWTT1GfPQumTV2zLvDspY22MSW4JyuIr4mortRSti4xPYhQmv1xQxpDfgmL3OrlV2A3pn44Krxc__zdg4ZWt8YBmAoyDPFXT3QKxYelq6pmr66znLOx0e8NPu8zGaQITPzSM8khF1THxFOF1zjKBLPCsTBMq5QS3OI4i9YjcY2Owg4_H0StpXK80S38x_WaijOVeXbPks8zKJyzFel7aBMpuMF2diEIut2KP1Rjrkm_qka1zVI2qrY4S_xSThc6y29kDL-5eyK1kNZ7Lu605zvhz-UrugbFiS-rBwn4rU63Pqu8fbpu4Kh2GQM3YPYmbW6AjNQ85zLSxaurD_4II9fiSXolo5oPXWA0_WeDYTSYNwKxyKtRDTw9uFNG6KSzPjdQtUFOELIOMB_NijdlH7cCJpIn0Gjh7Sr_wq8Wfd9ZKMNuX7QHIL51VdoxJKgM8L93D_YkLnLANY92e6XlbG_fCwvJjRKj4afY0hmUJOqVoHsIc6vVfzligLigz-x7UoFyXwMrOtgUjGhQmZ3IsGrDG2ZENuQabvHIp-raXm9Twx793NE2dCPZnoBUEDV_28l_N3fsmN5jYzmFBrmE3E45oO0WbgY_KQdin5hpe6MBd33SBVpNy0)](https://www.plantuml.com/plantuml/svg/ZLLXRzem4FsUNs5fqihAK3KcJHDKKoiw9CJGAjZwPPgYmpRKKcmZsw5YZF--ivCGGaEoBpHzztn-xt4Nxgm3urAPH3TNv341vNxElX4XOSt9IXiodj_FegkRI87aTWEKancbOWIEAo3x29RM6Q2Eq0Ii9VIH6oii5jt2QUERx2D2klk2To1RWTT1GfPQumTV2zLvDspY22MSW4JyuIr4mortRSti4xPYhQmv1xQxpDfgmL3OrlV2A3pn44Krxc__zdg4ZWt8YBmAoyDPFXT3QKxYelq6pmr66znLOx0e8NPu8zGaQITPzSM8khF1THxFOF1zjKBLPCsTBMq5QS3OI4i9YjcY2Owg4_H0StpXK80S38x_WaijOVeXbPks8zKJyzFel7aBMpuMF2diEIut2KP1Rjrkm_qka1zVI2qrY4S_xSThc6y29kDL-5eyK1kNZ7Lu605zvhz-UrugbFiS-rBwn4rU63Pqu8fbpu4Kh2GQM3YPYmbW6AjNQ85zLSxaurD_4II9fiSXolo5oPXWA0_WeDYTSYNwKxyKtRDTw9uFNG6KSzPjdQtUFOELIOMB_NijdlH7cCJpIn0Gjh7Sr_wq8Wfd9ZKMNuX7QHIL51VdoxJKgM8L93D_YkLnLANY92e6XlbG_fCwvJjRKj4afY0hmUJOqVoHsIc6vVfzligLigz-x7UoFyXwMrOtgUjGhQmZ3IsGrDG2ZENuQabvHIp-raXm9Twx793NE2dCPZnoBUEDV_28l_N3fsmN5jYzmFBrmE3E45oO0WbgY_KQdin5hpe6MBd33SBVpNy0 "SVG image of cFram hierarchy")

<!-- The following image renders well on github, but doesn't render in the previewer in vscode unless you turn on "insecure content". Of course, github rendering is more important, but this is irksome -->
[![Image of FRAM Storage objects -- see assets/cframstorage.plantuml](http://www.plantuml.com/plantuml/png/ZLJRJXin47tVhx3sXPjMY29LA5GLRI8GQjDKYU05eMhYdGrkrjx85mga_7jjxoKsMQA-9CgSCpDdpZXpeJH8RNYP7E-HdoK8VJF-JXY71PAYakHo-cMSx1qd0ZYg5L0aOqhP43GA8EiaeIKeHUYb13xJbNHfwuIG8nI6ava6aOFCl-6pSa7zlC8S2sAOqASVVcaoAGg5-jnXQoj4C_fAAw5qbtcjaPD7zpd2GjtTEshosjTsC8Xod8YiwyDXYRTG6jm_yCqwO85wnbxQSAZNGVQxGUPLLHAcVXW-Hnd7fhWg6RM6v4sydk3gvdf43csHAyCQPAF7U37dxqqocVz6ggVmf_Poa-QqMb4kwEkxsYtZp14QiFjzY_Bkz3vGsuwuV-2bzhJrebjQ7RBadKetCay-rgRMyThQ21Fur83rxeE5nBb3Q-fIXLQhk6oSzm3AUihoFipM78660FBrE6ZwW6gpdoER4A-zVPI1VFk0ysrjZwvtNvEUzs64OWk1EH4D9BHw-ouhRywaw1W1uQmUQMYan2r0lGUuhOwOef9n9i3kxwifYkujAokxSMgR4ypKy5fv4Nchr5Psg2LgNribc6RMnxS6-hswa7k3uQ59Jr9oc6MVIReSmrEwwnmCmYEo-tpuOnKtyat-bnt-VJ_zpJAIzg5fajXvBb3azlR-0m00)](https://www.plantuml.com/plantuml/svg/ZLJRJXin47tVhx3sXPjMY29LA5GLRI8GQjDKYU05eMhYdGrkrjx85mga_7jjxoKsMQA-9CgSCpDdpZXpeJH8RNYP7E-HdoK8VJF-JXY71PAYakHo-cMSx1qd0ZYg5L0aOqhP43GA8EiaeIKeHUYb13xJbNHfwuIG8nI6ava6aOFCl-6pSa7zlC8S2sAOqASVVcaoAGg5-jnXQoj4C_fAAw5qbtcjaPD7zpd2GjtTEshosjTsC8Xod8YiwyDXYRTG6jm_yCqwO85wnbxQSAZNGVQxGUPLLHAcVXW-Hnd7fhWg6RM6v4sydk3gvdf43csHAyCQPAF7U37dxqqocVz6ggVmf_Poa-QqMb4kwEkxsYtZp14QiFjzY_Bkz3vGsuwuV-2bzhJrebjQ7RBadKetCay-rgRMyThQ21Fur83rxeE5nBb3Q-fIXLQhk6oSzm3AUihoFipM78660FBrE6ZwW6gpdoER4A-zVPI1VFk0ysrjZwvtNvEUzs64OWk1EH4D9BHw-ouhRywaw1W1uQmUQMYan2r0lGUuhOwOef9n9i3kxwifYkujAokxSMgR4ypKy5fv4Nchr5Psg2LgNribc6RMnxS6-hswa7k3uQ59Jr9oc6MVIReSmrEwwnmCmYEo-tpuOnKtyat-bnt-VJ_zpJAIzg5fajXvBb3azlR-0m00 "SVG image of cFramStorage class hierarchy")

### Asynchronous Serial Port Command Processing

The Catena Aruduino platform provides both an asynchronous command-line collection object and a full command parser.

The `Catena::begin()` method normally crates a commmand parser instance that's linked to a command parser instance. For

#### Collecting lines asynchronously from streams

The header file `Catena_StreamLineCollector.h` defines the class `cStreamLineCollector`. This class is a `cPollableObject`, and as such is polled automaticaly by the governing `cPollingEngine`. A read is launched by calling `cStreamLineCollector::readAsync()`, passing a callback function, a buffer (base and size), and a context handle. When a command has been accumulated, the specified callback function is called according to the following prototype:

```c++
typedef void (cStreamLineCollector::ReadCompleteCbFn)(
    void* pCtx,
    cStreamLineCollector::ErrorCode uStatus,
    uint8_t *pBuffer,
    size_t nBuffer
    );
```

- `pCtx` is the user-supplied context parameter passed to `cStreamLineCollector::readAsync`.
- `uStatus` indicates whether the read was successful, and gives a rough idea of the failure reason if not.
- `pBuffer` points to the first byte of data. This might be `nullptr` in case of error, and it might be different than the user's original buffer pointer.
- `nBuffer` is passed as the actual number of data bytes in the buffer. In case of error, `nBuffer` will be zero.

#### The command parser

A command parser is initialized with a reference to a  `cStreamLineCollector` instance and a convenience reference to the governing `cCatena` instance. It is initialized with

```c++
bool cCommandParser::begin(cStreamLineCollector *pStream, cCatena *pCatena)`
```

The command parser works by parsing the input line into words, and then finding the command in _command tables_, which the client registers at run time using the following function:

```c++
void cCommandParser::registerCommands(cDispatch *pDispatch, void *pContext);
```

Multiple command tables can be registered dynamically; this allows modules to add commands as they are initailized. There's no need to edit a central command table.

The command tables consist of a top-level `cCommandParser::cDispatch` instance. This is **not** a `const` -- it has bookkeeping entries to help with building the tables at runtime without requiring `malloc()`. The dispatch instance points in turn to a

```c++
static cCommandStream::cDispatch myTable(/* cCommandStream::cEntry * */&table, sizeof(table));
```

or

```c++
static cCommandStream::cDispatch myTable(/* cCommandStream::cEntry * */&table, sizeof(table), "groupname");
```

In the first case, the commands are each entered into the top-level name space.  In the second case, a top-level command named `groupname` is entered, and each of the commands in the table is entered as a secondary command.

The command tables themselves are simple arrays of name/function pointer pairs.

```c++
static cCommandStream::CommandFn function1, function2 /*, etc. */;

static const cCommandStream::cEntry table[] = {
	"cmd1", function1,
	"cmd2", function2,
	// ...
};
```

The signature of each function is:

```c++
cCommandStream::CommandStatus function1(
    cCommandStream *pThis,
    void *pContext,
    int argc,
    char **argv
    );
```

`pThis` points to the parent `cCommandStream` instance. `pContext` is the user data from the relevant `cCommandStream::cDispatch` object. `argc` and `argv` are very much like the command arguments to a C `main()` function. `argv[0]` is the matching command, and `argv[1..argc-1]` are the parsed arguments from the command line.

A commmand function may operate synchronously or asynchronously.

#### Command stream methods for use by functions

Command stream functions may call any of these functions:

- `pThis->printf()` formats results to pass back to the command source.
- `pThis->getuint32()` scans an argument and converts to `uint32_t`.
- `pThis->completeCommand(CommandStatus)` signals the completion of an [asynchronous command](asynchronous-command-functions).

#### Synchronous Command Functions

A synchronous command function does all of its work in the initial function call, and returns a status code.  The status code can be any value _except_ `CommandStatus::kPending`. Synchronous commands must not call `pThis->completeCommand(CommandStatus)`.

#### Asynchronous Command Functions

An asynchronous command function allows for work to continue after the initial function call. The main command function typically has two parts.

1. The first part of the command is normally coded synchronously; it checks parameters, etc., and retuns non-`kPending` status. In this part of the command, there's no chance of `pThis->completeCommand` being called.

2. The second part of the command is coded asynchronously. The asynchronous paths each call `pThis->completeCommand()` when all work has been done. Once the function has established at least one asynchronous completion path, the main function must return `kPending` (and must ensure that all the completion paths call `completeCommand()`).

### `Catena_functional.h`

This wrapper allows the C++ `<functional>` header file to be used with Arduino code.

The technical problem is that the `arduino.h` header file defines `min()` and `max()` macros. This causes problems with parsing the `<functional>` header file, at least with GCC.

The solution is a hack: undefine `min()` prior to including `<functional>`, and then redefine them using the well-known definitions.

## Command Summary

### Standard commands

The following commands are supported by the Catena command parser.

| Command | Description |
|-------------|----------------------------------|
| `echo` _args_ | write arguments to the log stream
| `help` | list the known commands |
| `system configure operatingflags` _[&nbsp;uint32 ]_ | display or set the operating flags for this system. |
| `system configure platformguid`  _[&nbsp;hexGuid ]_ | display or set the platform GUID for this system |
| `system configure syseui` _[ eui64 ]_ | display or set the system serial number, a 64-bit number.
| `system reset` | dynamically restart the system, as if the reset button were pressed |

### FRAM commands

| Command | Description |
|-------------|----------------------------------|
| `fram reset` _[_ `hard` _]_ | reset the contents of the FRAM. A soft reset assumes that the datastructures are correct, and resets values to defaults. A hard reset invalidates the FRAM, so that the next boot will fully reconstruct it. |
| `fram dump` _[ base [ len ] ]_ | dump the contents of FRAM, starting at _base_ for _len_ bytes. If _len_ is absent, a length of 32 bytes is assumed. If _base_ is also absent, then 32 bytes are dumpped starting at byte zero.

### LoRaWAN commands

The following commmands are added by the Catena LoRawAN module.

| Command | Description |
|-------------|----------------------------------|
| `lorawan configure` _param [ value ]_  | Display or set a LoRaWAN parameter.
| `lorawan join` | unjoin if joined, then start a new join session.

#### LoRaWAN Parameters

Notes that these parameters are generall not loaded into the LMIC immediately. They are primarily used at boot time and at join time.

| Command     | Target device type | Description |
|-------------|---------------------|----------------------------------|
`lorawan configure deveui` _[ value ]_	| OTAA | Set the devEUI for this device to _value_, a 64-bit EUI given in big-endian (natural) form.
`lorawan configure appeui` _[ value ]_	| OTAA |Set the AppEUI for this device to _value_, a 64-bit EUI given in big-endian (natural) form.
`lorawan configure appkey` _[ value ]_	| OTAA |Set the AppKey for this device to _value_, a 128-bit value given in big-endian (natrual) form.
`lorawan configure nwkskey` _[ value ]_	| ABP | Set the NwkSKey for this device (the network session key) to _value_.  For OTAA devices, this reflects the value saved after them most recent join.
`lorawan configure appskey` _[ value ]_	| ABP |Set the AppSKey for this device (the application session key) to  _value_. For OTAA devices, this reflects the value saved after them most recent join.
`lorawan configure devaddr` _[ value ]_	| either | Set the device address, a 32-bit number, in big-endian form. **_Setting devaddr to zero on an OTAA device will cause the LMIC to try to rejoin after the next restart._** For OTAA devices, this reflects the value saved after them most recent join.
`lorawan configure netid` _[ value ]_	| either | Set the network ID, in big-endian form. For OTAA devices, this reflects the value saved after them most recent join.
`lorawan configure fcntup` _[ value ]_	| either | the curent uplink framecount, `FCntUp` in the LoRaWAN spec.
`lorawan configure fcntdown` _[ value ]_ | either | the current downlink framecount, `FCntDown` in the LoRaWAN spec.
`lorawan configure join` _[ value ]_ | either | if zero, the provisioning data will _not_ be loaded into the LMIC at startup. Older versions of the [arduino-lorawan](https://github.com/mcci-catena/arduino-lorawan) might still allow transmits to cause the device to start trying to join, but it will use invalid credentials.

## Adding your own commands

Here's a step-by-step procedure. There's a fully worked example, [catena_usercommand](#catenausercommand).

- Include the header file.

```c++
#include <Catena_CommandStream.h>

// for simplicity, we always assume this:
using namespace McciCatena;
```

- Declare your command functions.

```c++
// forward references to the command functions
cCommandStream::CommandFn cmdOne, cmdTwo /*, .. etc. */;
```

- Create a table of commands.  In this example, we use `static`, but that's not required.

```c++
// the individual commmands are put in this table
static const cCommandStream::cEntry sMyCommmandTable[] =
        {
        { "one", cmdOne },
        { "two", cmdTwo },
        // other commands go here....
        };
```

- Create the top-level structure that represents the command table to the parser. This cannot be `const`, becuase internal fields are used for linkage (to avoid run-time memory allocation in the library).

```c++
// a top-level structure wraps the above and connects to the system table
// it optionally includes a "first word" so you can for sure avoid name clashes
// with commands defined by the framework.
static cCommandStream::cDispatch sMyCommands(
    sMyCommmandTable,          	// this is the pointer to the table
    sizeof(sMyCommmandTable),  	// this is the size of the table
    "application"               // this is the "first word" for all the commands
                                // in this table. If nullptr, then the commands
                                // are added to the main table.
    );
```

- Register the table with the framework. As usual, we assume `gCatena` names the global top-level object.

```c++
gCatena.addCommands(
    // app dispatch table, passed by reference
    sMyCommands,
    // optionally a context pointer using static_cast<void *>().
    // normally only libraries (needing to be reentrant) need
    // to use the context pointer.
    nullptr
    );
```

- Write your command functions. Here's an example.

```c+++
// process the command "application one"
// argv[0] is "one" (the matching word)
// argv[1..argc-1] are the arguments, if any
cCommandStream::CommandStatus cmdOne(
    cCommandStream *pThis,
    void *pContext,
    int argc,
    char **argv
    )
    {
    // output your response using pThis->printf(), so that if there
    // are multiple command sources, the answers will go to the right
    // place.
    pThis->printf("Hello, world!\n");

    return cCommandStream::CommandStatus::kSuccess;
    }
```

## Example sketches

### `catena_hello`

This is a very simple sketch without LoRaWAN support. It shows the minimal boilerplate needed to use this library. Although it's not obvious, while looping, the program automatically flashes the LED and accepts commands from the console.

### `catena_hello_lora`

This sketch adds LoRaWAN uplink to the basic hello-world application. If the LoRaWAN system is provisioned, the app transmits a single message to port 16, containing the bytes `0xCA`, `0xFE`, `0xBA`, and `0xBE`, in sequence.

If the LoRaWAN system is not provisioned, the application enteres an idle loop; you can use the LoRaWAN commands to set things up.

### `catena_usercommand`

This sketch is very similar to `cathea_hello`. It shows how to add a user-defined comamand, `application hello`, that prints "`Hello, world!`".

## Board Support Dependencies

## Other Libraries and Versions Required

| Library | Recommended Version | Minimum Version | Comments |
|---------|:-------:|:----:|----------|
| [arduino-lmic](https://github.com/mcci-catena/arduino-lmic) | HEAD | 2.1.0 | Earlier versions will fail to compile due to missing `lmic_pinmap::rxtx_rx_polarity` and `lmic_pinmap::spi_freq` fields. |
| [arduino-lorawan](https://github.com/mcci-catena/arduino-lorawan) | HEAD | 0.5.3 | Needed in order to support the Murata module used in the Catena 4551, and for bug fixes in LoRaWAN::begin handling. |
| [catena-mcciadk](https://github.com/mcci-catena/Catena-mcciadk) | 0.1.2 | 0.1.2 | Needed for miscellaneous definitions |

## Library Release History

- v0.15.0 includes the following changes.

  - [#135](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/135), [#178](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/178) add platform support for Catena 4617, 4618 and 4630.
  - [#176](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/176) add documentation on platform GUIDs.
  - [#170](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/170) increased size of BME680 key to allow support of recent BSEC library versions.
  - [#168](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/168) updates `TxBuffer_t` for usability.
  - [#158](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/158) add clock info to `CatenaWingFram2k`, and add API to get configured clock rate.
  - [#164](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/164), [#165](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/165), [#163](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/163) enhance logging APIs.
  - [#161](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/161) adds asynchronous command processing. v0.14.0.60.
  - [#160](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/160) add section in README.md: [Adding Your Own Commands](#adding-your-own-commands).
  - [#157](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/157), [#153](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/153) Map SleepMode::Standby to STOP, and remove ineffective calls to `__HAL_PWR_CLEAR_FLAG()` in STM32 SleepForAlarm(). [#150](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/150) change STM32 Sleep() to request STOP mode instead of STANDBY mode.
  - [#28](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/28) add `lorawan join` command.
  - [#145](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/145) Fix errors in `catena_hello_lora` example.
  - [#143](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/143) clean up a compile warning for `cFramStorage::setCurrent()`.
  - [#142](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/141) add `CATENA_ARDUINO_PLATFORM_VERSION` to allow careful clients to `#error` rather than die with obscure compile problems.
  - [#141](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/141) add `cCommandStream::getuint32()`.
  - [#140](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/140) Add example sketch for user commands.
  - [#138](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/138) Check `ARDUINO_LORAWAN_VERSION`.
  - [#137](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/137) Document command parser and commands.
  - [#136](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/136) Add `system reset` command.
  - [#125](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/125) Turn on and off HSI clock as needed if system clock is using MSI clock.
  - [#127](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/127) Improve，stabilize AnalogRead() API.

- v0.14.0 (2019-02-10) includes changes for the following issues.

  - [#65](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/65) refactors the stm32/samd code so that code common for the two ports only appears once.
  - [#117](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/117) adds `Catena::ReadAnalog()` method for STM32 that reads the analog reference and adjust the raw ADC counts.
  - [#119](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/119) corrects an error that prevented provisioning on MCCI Catena 4612 and 4801 boards.

- v0.13.0 incorporates recent bug fixes and enhancements. We added our own implementation of the RTC class, to avoid the hacks we were using with the RTCzero library (issue [#86](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/86). We updated the UML docs (issue [#111](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/111)). We refactored the STM32 classes (issue [#99](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/99) and [#103](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/103)). A few other minor changes ([754f4b](https://github.com/mcci-catena/Catena-Arduino-Platform/commit/754f4bd9048f12edbd89906daea483eec31a9468) and [71d45d0](https://github.com/mcci-catena/Catena-Arduino-Platform/commit/71d45d0882b00cc0f18458a35a1f6b320e106433)).

- v0.12.0 adds support for the 4610, 4611, 4612, and 4801, and relies on the LMIC 2.3 pre-integrated pinmap feature to simplify maintenance and make things more structures. We added more UML diagrams to document the class hierarcy. Now depends on MCCI SAMD BSP 1.2.0 and STM32 2.0.0. A common `Catena::Sleep()` method was added for architecture-neutral low-power standby (issue [#83](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/83)). Added experimental machineQ network support. Various minor bug fixes and enhancements.

- v0.11.0 adds a flash driver for the Catena 4470, adds a flash object for storing BME680 calibration data, and fixes bugs.

- v0.10.0 adds explicit support for the Catena 4470, and fixes minor issues.

- v0.9.0 adds explicit support for the Catena 4460.

- v0.8.1 corrects an issue with the STM32 library (some things were here that beloned in the main Arduino libraries).

- v0.8.0 has some minor changes (add the Catena4551 m101/m102/m103/m104 platform, add the Catena4450 m103/m104), and a flag change which effectively changes the API (hence the bump). We add `CatenaBase::fHasLuxS1113`, which indicates the presence of a SI 1113 Lux sensor (as distinct from the BH1750 or the TSL2561 lux sensor used in the Catena4410). Further, we correct the platform flags for the 4551, as it doesn't have an I2C mux. Also incorporates some minor bug fixes for USB serial.

- v0.7.0 is a major refactoring adding support for the `Catena 4551`, which is based on the STM32L0. Although we think that there are no breaking changes, there might be a few, especially if code relied on structured defined internally to the MCCI-Catena-Arduino library `Catena...` classes.
