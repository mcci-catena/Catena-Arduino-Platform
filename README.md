# The MCCI Catena Arduino Platform Library

This library provides a simple-to-use framework for taking advantage of many of the features of the MCCI Catena&reg; Arduino products.

_Apologies_: This document is a work in progress, and is published in this intermediate form in hopes that it will still be better than nothing.

[![GitHub release](https://img.shields.io/github/release/mcci-catena/Catena-Arduino-Platform.svg)](https://github.com/mcci-catena/Catena-Arduino-Platform/releases/latest) [![GitHub commits](https://img.shields.io/github/commits-since/mcci-catena/Catena-Arduino-Platform/latest.svg)](https://github.com/mcci-catena/Catena-Arduino-Platform/compare/v0.23.0...master) [![Build Status](https://travis-ci.com/mcci-catena/Catena-Arduino-Platform.svg?branch=master)](https://travis-ci.com/mcci-catena/Catena-Arduino-Platform)

<!-- markdownlint-disable MD033 -->
<!-- markdownlint-capture -->
<!-- markdownlint-disable -->
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
		- [GUIDs for the Catena 480x family](#guids-for-the-catena-480x-family)
			- [Catena 4801](#catena-4801)
			- [Catena 4802](#catena-4802)
		- [GUIDs for Adafruit Feather M0s](#guids-for-adafruit-feather-m0s)
	- [Polling Framework](#polling-framework)
		- [Making a class pollable](#making-a-class-pollable)
		- [Using pollable objects in sketches](#using-pollable-objects-in-sketches)
	- [Finite State Machine (FSM) Framework](#finite-state-machine-fsm-framework)
		- [Getting ready](#getting-ready)
		- [Defining the state `enum class`](#defining-the-state-enum-class)
		- [Identify the parent class](#identify-the-parent-class)
		- [Add the state type to the parent class](#add-the-state-type-to-the-parent-class)
		- [Define the FSM instance in the parent class](#define-the-fsm-instance-in-the-parent-class)
		- [Declare a method function in the parent class](#declare-a-method-function-in-the-parent-class)
		- [Implement the FSM dispatch function](#implement-the-fsm-dispatch-function)
		- [Implement the FSM initialization](#implement-the-fsm-initialization)
	- [The general time/date class `McciCatena::cDate`](#the-general-timedate-class-mccicatenacdate)
		- [Interval Seconds](#interval-seconds)
		- [`cDate` calendar types](#cdate-calendar-types)
		- [`cDate` properties](#cdate-properties)
		- [`cDate` methods](#cdate-methods)
		- [Timekeeping, solar days, leap seconds](#timekeeping-solar-days-leap-seconds)
	- [LoRaWAN Support](#lorawan-support)
		- [Sending an uplink message](#sending-an-uplink-message)
		- [Registering to receive downlink messages](#registering-to-receive-downlink-messages)
		- [LoRaWAN Class Structure](#lorawan-class-structure)
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
	- [Clock Management and Calibration](#clock-management-and-calibration)
	- [Watchdog Timer](#watchdog-timer)
		- [SafeDelay()](#safedelay)
	- [Si1133 driver](#si1133-driver)
	- [`cTimer` Timer object](#ctimer-timer-object)
		- [Catena_Timer.h header file and initialization](#catena_timerh-header-file-and-initialization)
		- [cTimer begin() and end()](#ctimer-begin-and-end)
		- [Checking for `cTimer` events](#checking-for-ctimer-events)
		- [`cTimer` Utility routines](#ctimer-utility-routines)
	- [`Catena_functional.h`](#catena_functionalh)
	- [`cDownload`](#cdownload)
	- [`cBootloaderApi`](#cbootloaderapi)
	- [`cSerial`](#cserial)
- [Command Summary](#command-summary)
	- [Standard commands](#standard-commands)
	- [STM32L0 commands](#stm32l0-commands)
	- [FRAM commands](#fram-commands)
	- [LoRaWAN commands](#lorawan-commands)
		- [LoRaWAN Parameters](#lorawan-parameters)
- [Adding your own commands](#adding-your-own-commands)
- [Example sketches](#example-sketches)
	- [`catena_hello`](#catena_hello)
	- [`catena_hello_lora`](#catena_hello_lora)
	- [`catena_usercommand`](#catena_usercommand)
	- [`catena_fsm`](#catena_fsm)
- [Board Support Dependencies](#board-support-dependencies)
- [Other Libraries and Versions Required](#other-libraries-and-versions-required)
- [Release History](#release-history)
- [Meta](#meta)
	- [License](#license)
	- [Support Open Source Hardware and Software](#support-open-source-hardware-and-software)
	- [Trademarks](#trademarks)

<!-- /TOC -->
<!-- markdownlint-restore -->
<!-- Due to a bug in Markdown TOC, the table is formatted incorrectly if tab indentation is set other than 4. Due to another bug, this comment must be *after* the TOC entry. -->

## Overview

## Coding Practices

In order to assist people who are not everyday readers and writer of C++, this library adopts some rules.

1. All names are in the `McciCatena` namespace.

2. In classes with elaborate hierarchy, we normally define a private synonym of `Super` which refers to the parent class. This is done so that we can change parent/child relationships without breaking code.

3. We tend to use the `m_...` prefix on the names of class member fields.

4. We tend to use `this->m_...` to refer to class members (rather than omitting `this->`). We do this for emphasis, and to avoid visual ambiguity.

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
`Catena4802`          | `Catena4802.h`          | MCCI Catena 4802 Murata-based board with Modbus and Temperature sensor.

### Class derivation

The following figures gives the class derivation hierarchy for the board classes.

The tree is too big to show in one diagram here. So we split according to the two families: STM32-based CPUs, and SAMD-based CPUS.

<!--

For all these, see assets/CatenaBase.plantuml. for some bizarre reason GitHub won't render these unless image links are http (not https).

Each image is structured as a reference to the image, plus a link. Both must be updated; only the link may be https; the image must be http.

Also note that the link is SVG, but the image is PNG.

-->

#### STM32 Classes

The first figure just gives relationships; the second has details about the members of each class.

**Catena STM32 Class Relationships:**
<!-- $enableStm32 = 1, $enableSamd = 0,  $enableMembers = 0 -->

[![**Catena STM32 Class Relationships**:](http://www.plantuml.com/plantuml/png/xLbhRzf85_xC_8gf-kfjakO2fBcekvLA2OdG0a50jX-g2etj0tXZptXdngHiD_zzZmUCBy4uLTLj8ksN1i_pdCjvE7DcSDyhpQGEVOySlIAaAvpGWxUL9jF0sGUcu33m63SukGovhLt1tv9AFrgywNQeX0b8u3PG9KA9VoP2qa6hST5j7VeEZIYsnvIYZikcalc7f4AQ8bX8TphJw6W8G9bj2pzWVE7oAUsqcwtUi4KdhWV8ROHw9YGIHo3bWdQ5aB1FkyrccpQ538HaKKQLdfY3Ru6azMhjd92UqA3IQRe-cuAYBezowrwOt9W4EWKEw08SQYrem9IEKj0pLvduoDU2LFGCgmCvH_TCqvdMmTkZexkxkxKqXvZ_KVomJnbtADPgMKjdM8O45Nh6zysWemuf7SqYJqnHBQW5r14y2e4Hc4UrwuFQftUkdg5g4wEl9fKvyq8i0HmNqpMPHRxfd4cNMHuY5dZYxX1T0yLyV3IA2aw-35AvSprq3_ZFGeJq3dDufIW7SC0v9EJL4I4l_WSyyjOrQYhwZbRNQqFj7zTnfPQiCDyn5C8PvXmm6sZNjjrbzz2lX31BQSbilMg2fAqGfFGrWY62hPlswPkn5KQrZlKl3n1yGEsoo0rt_moXVR4s-_Hv4npRnZ3mqAVN_TQWCMhthiQNdSRLC07wdSReydhGJQr_LPffrxuFKV7Z-bXJx2cDJUvVUcognbUWSS_qmaYkdTsCXIM4Hst1bOOlWSpQlHwt5TgXy2kZkN0TEcGJw6EwUh9QDTOh4JwiwgHx67EuKBWQ1unLlb_ocurHgzSOnyKOWtLq8TiN3-nCT6J5giNXPqoYTRA-53D6IuEjY6lf4BeEels6JemCcU8BmBHrHiFDLLWmTNc1CCjwLbtOOpxix5A3R78qz022Xuuk9VFftXHqz26TVo0yB_qiCH6bGLvRVu2jLmXpd2RkJSQTEFs-57DNkO9ZsKEzyC3KcLzSaKrB8TwmWZIbpISYnxZMZ1LGJ7Ozq4CsXo6eYB0T7MgcCnaWZ25ojZ689NR4W7raFAuqjozIGs0lI1uYpij4w8o6MKzvrZHfZ54qbWfPpI1iEvkezX2aoxm1C6THySUf9mDdvnD5Wjb-IzE-y3mKkCMnxm2nzFDqvLrBO7wRupeq4GPR2udKzUSbRLNbaermvZcY7CHx0S7iNZ5emORqnq5Siw4PDHIRGx9Da_LfPbGOmIBudhnm9tGdC_Vn9DZTFE0Zl5wZNxDRXvhsIE_FrCAwCZ47ALq7kapThbctULROz839YBDFayKXwCIiOARbz_RZMkvWMTXWr4Jf1zesYW37uvaaoYHdPB4qXl0abIaNeg3eqdQu1APd8BjLh3i-QHkzHkVwQjnlzrB7xqNxgZrgT9BL1xfCFC4qZNRcxnRJwV7zCZ_iyzFFd4mFHsAfhPaJnjYCjKRl5XOhCJQN-n6DeiEXVliZwtsTFuaI1xZqGGZT527NE_-tue_BKhSrkRrA6tlyKZuSEMkdtzVz7Uo3VB7VKL5g7nMhLK0ALOQKdmtbOfsNYsM0JFauFv8fKj14RqvEllpGzYcIuRueeTFQKqle-8abT5xzMGcLJELCbczgkF-A4qhVb3TVb5y-UaOQTjdbwo2-5nQvdTwELrx7lw7R3JRD_atxOdtZ6NSRVJetuKOY3lZfymOxvVu5OqrvdXn9cXEj_fzetoxQ2OhsI7cAB_BVAaR-0hopHvVhyzXKVVgykwsmcFUJL3lzVjL-h2WFCZvxwXcNNrToPs_jiGNzYpAVrfvPOs36jMUNSVtPPNpsx38-VsuP7p-tdNTUhJsxZBVll8ABiCkVG2sb5_3KAyzV_puuoBmcApkQ2emDepnk4iSOvz_853AIbnWaPnApuXyJs_4wuZc7MS996U6qZ71MVQI4ovMyXfqgoJp5XFXN-1RqjBOLFTw6uez5Gf8ekQWraeJ8XQmMGrkiwiNGMJ5qJf9QShLLIL98hh1QCRJksFluoyhzV-CxMjqbCzS1wY-NOeFt-2VqlNy0)](https://www.plantuml.com/plantuml/svg/xLbhRzf85_xC_8gf-kfjakO2fBcekvLA2OdG0a50jX-g2etj0tXZptXdngHiD_zzZmUCBy4uLTLj8ksN1i_pdCjvE7DcSDyhpQGEVOySlIAaAvpGWxUL9jF0sGUcu33m63SukGovhLt1tv9AFrgywNQeX0b8u3PG9KA9VoP2qa6hST5j7VeEZIYsnvIYZikcalc7f4AQ8bX8TphJw6W8G9bj2pzWVE7oAUsqcwtUi4KdhWV8ROHw9YGIHo3bWdQ5aB1FkyrccpQ538HaKKQLdfY3Ru6azMhjd92UqA3IQRe-cuAYBezowrwOt9W4EWKEw08SQYrem9IEKj0pLvduoDU2LFGCgmCvH_TCqvdMmTkZexkxkxKqXvZ_KVomJnbtADPgMKjdM8O45Nh6zysWemuf7SqYJqnHBQW5r14y2e4Hc4UrwuFQftUkdg5g4wEl9fKvyq8i0HmNqpMPHRxfd4cNMHuY5dZYxX1T0yLyV3IA2aw-35AvSprq3_ZFGeJq3dDufIW7SC0v9EJL4I4l_WSyyjOrQYhwZbRNQqFj7zTnfPQiCDyn5C8PvXmm6sZNjjrbzz2lX31BQSbilMg2fAqGfFGrWY62hPlswPkn5KQrZlKl3n1yGEsoo0rt_moXVR4s-_Hv4npRnZ3mqAVN_TQWCMhthiQNdSRLC07wdSReydhGJQr_LPffrxuFKV7Z-bXJx2cDJUvVUcognbUWSS_qmaYkdTsCXIM4Hst1bOOlWSpQlHwt5TgXy2kZkN0TEcGJw6EwUh9QDTOh4JwiwgHx67EuKBWQ1unLlb_ocurHgzSOnyKOWtLq8TiN3-nCT6J5giNXPqoYTRA-53D6IuEjY6lf4BeEels6JemCcU8BmBHrHiFDLLWmTNc1CCjwLbtOOpxix5A3R78qz022Xuuk9VFftXHqz26TVo0yB_qiCH6bGLvRVu2jLmXpd2RkJSQTEFs-57DNkO9ZsKEzyC3KcLzSaKrB8TwmWZIbpISYnxZMZ1LGJ7Ozq4CsXo6eYB0T7MgcCnaWZ25ojZ689NR4W7raFAuqjozIGs0lI1uYpij4w8o6MKzvrZHfZ54qbWfPpI1iEvkezX2aoxm1C6THySUf9mDdvnD5Wjb-IzE-y3mKkCMnxm2nzFDqvLrBO7wRupeq4GPR2udKzUSbRLNbaermvZcY7CHx0S7iNZ5emORqnq5Siw4PDHIRGx9Da_LfPbGOmIBudhnm9tGdC_Vn9DZTFE0Zl5wZNxDRXvhsIE_FrCAwCZ47ALq7kapThbctULROz839YBDFayKXwCIiOARbz_RZMkvWMTXWr4Jf1zesYW37uvaaoYHdPB4qXl0abIaNeg3eqdQu1APd8BjLh3i-QHkzHkVwQjnlzrB7xqNxgZrgT9BL1xfCFC4qZNRcxnRJwV7zCZ_iyzFFd4mFHsAfhPaJnjYCjKRl5XOhCJQN-n6DeiEXVliZwtsTFuaI1xZqGGZT527NE_-tue_BKhSrkRrA6tlyKZuSEMkdtzVz7Uo3VB7VKL5g7nMhLK0ALOQKdmtbOfsNYsM0JFauFv8fKj14RqvEllpGzYcIuRueeTFQKqle-8abT5xzMGcLJELCbczgkF-A4qhVb3TVb5y-UaOQTjdbwo2-5nQvdTwELrx7lw7R3JRD_atxOdtZ6NSRVJetuKOY3lZfymOxvVu5OqrvdXn9cXEj_fzetoxQ2OhsI7cAB_BVAaR-0hopHvVhyzXKVVgykwsmcFUJL3lzVjL-h2WFCZvxwXcNNrToPs_jiGNzYpAVrfvPOs36jMUNSVtPPNpsx38-VsuP7p-tdNTUhJsxZBVll8ABiCkVG2sb5_3KAyzV_puuoBmcApkQ2emDepnk4iSOvz_853AIbnWaPnApuXyJs_4wuZc7MS996U6qZ71MVQI4ovMyXfqgoJp5XFXN-1RqjBOLFTw6uez5Gf8ekQWraeJ8XQmMGrkiwiNGMJ5qJf9QShLLIL98hh1QCRJksFluoyhzV-CxMjqbCzS1wY-NOeFt-2VqlNy0 "Click for SVG image of Class Relationships")

**Catena STM32 Class Hierarchy (full detail):**
<!-- $enableStm32 = 1, $enableSamd = 0,  $enableMembers = 1 -->
[![**Catena STM32 Class Hierarchy (full detail):**](http://www.plantuml.com/plantuml/png/xLbRRzf857vcViLKNMsJD1SWpKNThLHAI8GM20AoVQWgDBOFu8qzuvqPaxBT_FSz7Z2-14EYgcsGzgN1yttdSZxEd1dSzqepgKFV8qUl26aB9_JWRQdED73sWIau33p63KukGsvhL_2tfDID5c_QBIfX11Au3LI9KEAVaP2qrwXTj1k7laCZYkqnfQZZih5a_Y4faRe8Pj8TJpGwwWDGPjl23nYVkNnCMyrwezDlq97h0N9hePu8YSG1I3cZRI4axDDslTwaTI438LcKKQaZfk1R86crN3adf2Cqg7IQhi_6eAZBezpQ5oOt9e6EWGEw08TQCnemfQCKzCHL9ZxojI0bFS7gG4xHFTDqedNmzkZexkvkASqXvdyKF-nJnXsAjLhMt1cM8K65dl5zqskfGqe7ayWJKrGBQW5r18z2O0JcKUtweFRfdQidgDh8wAj9QSgy44i0nyLqJMQHRpfbqcMMXuW5dhWxHDT0CHyVZQA2ao-3bEvS3zq3_ZCJ8Rt375ufoW4SS0u9UNL4o8jVW4VUsaPDHT_HodAjh_tZQdQ5-Gukb0dX37CEc0sqRTlkl7leLqAOfRHajbuqGT9M25Bw6i4GmSPDy_JDq0gZMeVwjmS8Fg1sMUI6kt-7qBnOcdtwl0gERMCOUEZJwswZLniqErV3orRjgfy0tLPjS7dTQwVMlohDj6lVXwZuSNMeAVQKnYRtBpqsLiChqBXdEc4aryvkni8ImgEsu4h3bq1cxLuFcmhjKFY5qLIu3ksp4NGnNJrQh4RMiGWV5dNIFOpPdobSZGF6Aj_F-VNQeD6f3UDYZC4okf3DYmTs9ZgoOjNYy1CcqJfPduiPenk3BOXBwH2w3gBzXawC39dY2y2qTKR3rLLOC7PvWJ3pUXPTs64-xEnIWwnorFS0WeUEBYNpwTuOTFGXdNyWF2_zB34HfK5UMt-1hHS8Svmwxat6dJZzhXHJLxc2OzbzFVF0r9bVN91DIo5Uiu8qfSb78iUuremLK4ns7T1zDeK-g8Ym7krhfZCP88mXShOnY2MsH8zzP3nNQRmlKaDWBqWU8ixBHEYCXbbFUTOuQOn1D9OAMVKWR3ghg7OVfCky7Z1dLlA7gIS3P-SJHOBPVdFJhl0y5BZ1iUy0iVJpUE5TIs1-a-Cwr146Mmk9rENdEMrHvPoDSEOveXp4Um71x5unQC66z8T1NBEX6JKKcqAoJPFrSMPK6C4Y-9wyS4Tq9pFtyIJONJtW8xnQeL-pMuUQzaZlpzJ2iZ8n1IbT1zfCtIvPjtbMMFU0oOYpJvF5FkZ4h62cvVVsurhkO5vORr17wNlQDee0n-EP9CeaPsInD8RmB9NPB4H1qHlRuHAOde1ibx7k-AIjTMgjwwjXjzb97RyNpQlceDPALX_eCl84qpJQcNzQJAV7zylyiCzFFtCoFHo96stDd336PgpLEhSmMuYnkjoFQ1GT3jNRxrdlw_n9b3Z0fGz2wBe8kTxvrOe_pajTr-Js8ctiyKlvSEGidNvRzxUm3_B5VaD5gNrKe5OnALGPKdusR1RhVBDO1iYKZ_CZcI85JVJcvEJBTssV8XdkYn8whJotX8wVMKBdvP-Lq8QfdCdoJGNtNt52wPloweloooVCI6HlkdmTnFV28hVZs-72w_0FTBl2fjwzQLuiRpp3TgrBfoRSGCG1FtrUOQVSVs2eAS-J8qbpebN_5-tfefsWQ8-Kf_WY_rGnyXVWdJqwNv_6fkhJvzrrXSMydwJQwTDL-x6YFCZuxBbdlFcwaZjxAuyjw0VAV5hPiiR0Z2jRbt5rwp8-sxgCpxSju-DjstddvShMPRn-vnLSX5t-36hPU05FlVBy_T-3Wynhiar7Ku6nOMoEcyGnnlatCeMCv2K6oPd4hFZ7n7gyYdZEOPPmiebmkebmLdwaXFELl8QjCiaynOJuL_WQzBIo5ZrUX-AFHKAIABce5PA4o8Ki5qDhhAh5q5anT4wIMdArbKbIIAwmIZ6qxDZx-ClA_N_ZErhU9HFN0UhFbsA3z_WdzBt_0000)](https://www.plantuml.com/plantuml/svg/xLbRRzf857vcViLKNMsJD1SWpKNThLHAI8GM20AoVQWgDBOFu8qzuvqPaxBT_FSz7Z2-14EYgcsGzgN1yttdSZxEd1dSzqepgKFV8qUl26aB9_JWRQdED73sWIau33p63KukGsvhL_2tfDID5c_QBIfX11Au3LI9KEAVaP2qrwXTj1k7laCZYkqnfQZZih5a_Y4faRe8Pj8TJpGwwWDGPjl23nYVkNnCMyrwezDlq97h0N9hePu8YSG1I3cZRI4axDDslTwaTI438LcKKQaZfk1R86crN3adf2Cqg7IQhi_6eAZBezpQ5oOt9e6EWGEw08TQCnemfQCKzCHL9ZxojI0bFS7gG4xHFTDqedNmzkZexkvkASqXvdyKF-nJnXsAjLhMt1cM8K65dl5zqskfGqe7ayWJKrGBQW5r18z2O0JcKUtweFRfdQidgDh8wAj9QSgy44i0nyLqJMQHRpfbqcMMXuW5dhWxHDT0CHyVZQA2ao-3bEvS3zq3_ZCJ8Rt375ufoW4SS0u9UNL4o8jVW4VUsaPDHT_HodAjh_tZQdQ5-Gukb0dX37CEc0sqRTlkl7leLqAOfRHajbuqGT9M25Bw6i4GmSPDy_JDq0gZMeVwjmS8Fg1sMUI6kt-7qBnOcdtwl0gERMCOUEZJwswZLniqErV3orRjgfy0tLPjS7dTQwVMlohDj6lVXwZuSNMeAVQKnYRtBpqsLiChqBXdEc4aryvkni8ImgEsu4h3bq1cxLuFcmhjKFY5qLIu3ksp4NGnNJrQh4RMiGWV5dNIFOpPdobSZGF6Aj_F-VNQeD6f3UDYZC4okf3DYmTs9ZgoOjNYy1CcqJfPduiPenk3BOXBwH2w3gBzXawC39dY2y2qTKR3rLLOC7PvWJ3pUXPTs64-xEnIWwnorFS0WeUEBYNpwTuOTFGXdNyWF2_zB34HfK5UMt-1hHS8Svmwxat6dJZzhXHJLxc2OzbzFVF0r9bVN91DIo5Uiu8qfSb78iUuremLK4ns7T1zDeK-g8Ym7krhfZCP88mXShOnY2MsH8zzP3nNQRmlKaDWBqWU8ixBHEYCXbbFUTOuQOn1D9OAMVKWR3ghg7OVfCky7Z1dLlA7gIS3P-SJHOBPVdFJhl0y5BZ1iUy0iVJpUE5TIs1-a-Cwr146Mmk9rENdEMrHvPoDSEOveXp4Um71x5unQC66z8T1NBEX6JKKcqAoJPFrSMPK6C4Y-9wyS4Tq9pFtyIJONJtW8xnQeL-pMuUQzaZlpzJ2iZ8n1IbT1zfCtIvPjtbMMFU0oOYpJvF5FkZ4h62cvVVsurhkO5vORr17wNlQDee0n-EP9CeaPsInD8RmB9NPB4H1qHlRuHAOde1ibx7k-AIjTMgjwwjXjzb97RyNpQlceDPALX_eCl84qpJQcNzQJAV7zylyiCzFFtCoFHo96stDd336PgpLEhSmMuYnkjoFQ1GT3jNRxrdlw_n9b3Z0fGz2wBe8kTxvrOe_pajTr-Js8ctiyKlvSEGidNvRzxUm3_B5VaD5gNrKe5OnALGPKdusR1RhVBDO1iYKZ_CZcI85JVJcvEJBTssV8XdkYn8whJotX8wVMKBdvP-Lq8QfdCdoJGNtNt52wPloweloooVCI6HlkdmTnFV28hVZs-72w_0FTBl2fjwzQLuiRpp3TgrBfoRSGCG1FtrUOQVSVs2eAS-J8qbpebN_5-tfefsWQ8-Kf_WY_rGnyXVWdJqwNv_6fkhJvzrrXSMydwJQwTDL-x6YFCZuxBbdlFcwaZjxAuyjw0VAV5hPiiR0Z2jRbt5rwp8-sxgCpxSju-DjstddvShMPRn-vnLSX5t-36hPU05FlVBy_T-3Wynhiar7Ku6nOMoEcyGnnlatCeMCv2K6oPd4hFZ7n7gyYdZEOPPmiebmkebmLdwaXFELl8QjCiaynOJuL_WQzBIo5ZrUX-AFHKAIABce5PA4o8Ki5qDhhAh5q5anT4wIMdArbKbIIAwmIZ6qxDZx-ClA_N_ZErhU9HFN0UhFbsA3z_WdzBt_0000 "Click for SVG image of Class Hierarchy")

#### SAMD Classes

The first figure just gives relationships; the second has details about the members of each class.

**Catena SAMD Class Relationships:**
<!-- $enableStm32 = 0, $enableSamd = 1,  $enableMembers = 0 -->
[![**Catena SAMD Class Relationships**:](http://www.plantuml.com/plantuml/png/xLbjRzf84FxC_egjUhecQLw0D2_gNQLIGY9qG12GwuUgGcjx05_iNT_kcfJhvR_VUC7u9HWILMs3T5-QU9vdNkR9x7YXxvLcKeU-HuvU4T8MJkZ1srATQU3i0rDm67YC6vnSXjpMhk1lIQaR6M_QBIfX11Au3LI9KEAFaP2qrwXTj1k7laCZYkqnfQZZih5a_Y4faRe8Pj8TJpG6wWDGPjl23nYVkNnCMyrwezDlq97h0N9hePu8YSG1I3cZRI4axDDslTwaTI438LbKKQaZfk1R86crN3adf2Cqg7IPhi_6eAZBezhQ5wOs9e6EWGE608TQCnemfQCIzCHL9Z_ojI0bFS7kG4unFDDqedNmzkZexkvkASqXrdyKF-nJnXsAlLhMF1Ys8K65debzqskfGqe7aoWIKrGBQW5r18zIO0RcKUtweFRfdQidgDh8wAj9QSgy45i0nyLoJMLHR3fbqcMMXuW5dhWxnD10iHuVdQA6apy6ATsv7eO7_6ScGdg7DRnIb0Cuu1mIykg8a1U_08-YjOsQYhwZvQMjh_tZQjR2V0SD5K8umve3PWDjsxOxdnxwbH1cAIsPhHT3a8mLWfI-HZ14i77JF7qpjCAerw7-xG623wXT5hdXxjyXD2-MRfy-hu9ZtnW67lhqkjlerGRDpjNmibMxwYT0jrKRN5xtsYdxLwMPTktx41K_hWurnPdIEEJ-fSV6QdW56izC9upasjdDU5X2UDGMN6du4iYit-jXKw4V2hzmcWhNeNqsWYwMgqSBQ-GTY_1XqIVTmvpzcK9hd31M-NxEhzS6ZKvj63TZ79RPXMnUFF0psP4LgnQddp29tebzBcQCRaos82wbG-WwYCuR1Z4oP9el0DFU6GrNTs71sEK5mipxMKnXX_cmiqiDiYfGtmC87WQwbCodUsFGqI_f-WFXUUbdYOKe3VBQ-WjilK2OuzJnR3BknELtfPYwoXKSs-xhcGUcpxnnGJOZXNZD2jAK9X-97EDQCrP0CTLrGFVP5FgW8i9wjA-PpbI0CAR8-SOWjjWIFVQHyLodyRb8BO6z87c8E2yJeJCQPYFbMUDaC0RHMYfarOEmwQoYsdsGBlDwm9nPoH-cdWoStKyK2URupLsxmlDGu0R7kGF4qi_ZHNGjWVbDZdQe8mos5XAfoz_dj4MNSpR2cUU8SX2V1GIpPyMW1GVI7mPnpuPcr51i2ianJUpZpAemWaNmFNdXZkXEPk_ZujzTlU0Zl5gXNxD7XvhnI9_Fb67PcPY2bAu3RQPkbwpRF2kiUy1anDcdYR4FEd4hs6dvi_suaJkODzORr57wdhQD8i3nzKmIPP9tPB4qXl0ibTaiH67J6yRX4fYUW6oNiU_uJLlhr5hNLyDkivDw_Luqhvg3MYknFj1bv0ccQNGo_xIOJg_lb_bbdzz-vitqS2Lkz3LxmZYRjLRhtC9i8SRgTZ-YK_HogDv-ptvVvzz4IG0qVH12rqN8zSwlLlphlDLrGsuloiOPlvGFLyuowBTDVmld8D_iDtIKEaS5QXMJGfKXvNV3Ph7EDujbW4pxk3-IBL8mH6zEJhvyr_4fak6-gA3JodChwFYP5NHU_ba5RTZAcIhVLF3y5HUKlYcllYY_VCAEHFQcozT1V2yi2Zk-7IwY3l_0i2jywjsRviNonZDirxfqQj8DH9pmq-SLVihz1uQQyZmvaZGdMlL_qPuksWcAzaXvYY_oJnKZVm7UsQDp-pHsLPy-xwvhBERz9DLEdwxQZnNbGSLdpx_YJTULtBktyjY6Vg3CfvKj6moikB9r5LUthkAphQluVDigFjwsatTUhcnTnUjFNi55sENFefSD5_3KLvw__tjmaFcQRDEhgS3PC3RdJV8Ov_mtCeMCv4iCadE9MV67YVLu5V5Sm2pXP1FXT1FXhFn82UUMl8QjCiaynOJuK_WQzBIo5ZrUX-A7HKAIBBci5PAaoAKi5qDhlAh5q5anT4wINdAzbKdIIAwnIZ6qdDZx-8-L-__6TxIyIoQk0zIVcsA7z_WZzBt_0000)](https://www.plantuml.com/plantuml/svg/xLbjRzf84FxC_egjUhecQLw0D2_gNQLIGY9qG12GwuUgGcjx05_iNT_kcfJhvR_VUC7u9HWILMs3T5-QU9vdNkR9x7YXxvLcKeU-HuvU4T8MJkZ1srATQU3i0rDm67YC6vnSXjpMhk1lIQaR6M_QBIfX11Au3LI9KEAFaP2qrwXTj1k7laCZYkqnfQZZih5a_Y4faRe8Pj8TJpG6wWDGPjl23nYVkNnCMyrwezDlq97h0N9hePu8YSG1I3cZRI4axDDslTwaTI438LbKKQaZfk1R86crN3adf2Cqg7IPhi_6eAZBezhQ5wOs9e6EWGE608TQCnemfQCIzCHL9Z_ojI0bFS7kG4unFDDqedNmzkZexkvkASqXrdyKF-nJnXsAlLhMF1Ys8K65debzqskfGqe7aoWIKrGBQW5r18zIO0RcKUtweFRfdQidgDh8wAj9QSgy45i0nyLoJMLHR3fbqcMMXuW5dhWxnD10iHuVdQA6apy6ATsv7eO7_6ScGdg7DRnIb0Cuu1mIykg8a1U_08-YjOsQYhwZvQMjh_tZQjR2V0SD5K8umve3PWDjsxOxdnxwbH1cAIsPhHT3a8mLWfI-HZ14i77JF7qpjCAerw7-xG623wXT5hdXxjyXD2-MRfy-hu9ZtnW67lhqkjlerGRDpjNmibMxwYT0jrKRN5xtsYdxLwMPTktx41K_hWurnPdIEEJ-fSV6QdW56izC9upasjdDU5X2UDGMN6du4iYit-jXKw4V2hzmcWhNeNqsWYwMgqSBQ-GTY_1XqIVTmvpzcK9hd31M-NxEhzS6ZKvj63TZ79RPXMnUFF0psP4LgnQddp29tebzBcQCRaos82wbG-WwYCuR1Z4oP9el0DFU6GrNTs71sEK5mipxMKnXX_cmiqiDiYfGtmC87WQwbCodUsFGqI_f-WFXUUbdYOKe3VBQ-WjilK2OuzJnR3BknELtfPYwoXKSs-xhcGUcpxnnGJOZXNZD2jAK9X-97EDQCrP0CTLrGFVP5FgW8i9wjA-PpbI0CAR8-SOWjjWIFVQHyLodyRb8BO6z87c8E2yJeJCQPYFbMUDaC0RHMYfarOEmwQoYsdsGBlDwm9nPoH-cdWoStKyK2URupLsxmlDGu0R7kGF4qi_ZHNGjWVbDZdQe8mos5XAfoz_dj4MNSpR2cUU8SX2V1GIpPyMW1GVI7mPnpuPcr51i2ianJUpZpAemWaNmFNdXZkXEPk_ZujzTlU0Zl5gXNxD7XvhnI9_Fb67PcPY2bAu3RQPkbwpRF2kiUy1anDcdYR4FEd4hs6dvi_suaJkODzORr57wdhQD8i3nzKmIPP9tPB4qXl0ibTaiH67J6yRX4fYUW6oNiU_uJLlhr5hNLyDkivDw_Luqhvg3MYknFj1bv0ccQNGo_xIOJg_lb_bbdzz-vitqS2Lkz3LxmZYRjLRhtC9i8SRgTZ-YK_HogDv-ptvVvzz4IG0qVH12rqN8zSwlLlphlDLrGsuloiOPlvGFLyuowBTDVmld8D_iDtIKEaS5QXMJGfKXvNV3Ph7EDujbW4pxk3-IBL8mH6zEJhvyr_4fak6-gA3JodChwFYP5NHU_ba5RTZAcIhVLF3y5HUKlYcllYY_VCAEHFQcozT1V2yi2Zk-7IwY3l_0i2jywjsRviNonZDirxfqQj8DH9pmq-SLVihz1uQQyZmvaZGdMlL_qPuksWcAzaXvYY_oJnKZVm7UsQDp-pHsLPy-xwvhBERz9DLEdwxQZnNbGSLdpx_YJTULtBktyjY6Vg3CfvKj6moikB9r5LUthkAphQluVDigFjwsatTUhcnTnUjFNi55sENFefSD5_3KLvw__tjmaFcQRDEhgS3PC3RdJV8Ov_mtCeMCv4iCadE9MV67YVLu5V5Sm2pXP1FXT1FXhFn82UUMl8QjCiaynOJuK_WQzBIo5ZrUX-A7HKAIBBci5PAaoAKi5qDhlAh5q5anT4wINdAzbKdIIAwnIZ6qdDZx-8-L-__6TxIyIoQk0zIVcsA7z_WZzBt_0000 "Click for SVG image of SAMD Class Relationships")

**Catena SAMD Class Hierarchy (full detail):**
<!-- $enableStm32 = 0, $enableSamd = 1,  $enableMembers = 1 -->
[![**Catena SAMD Class Hierarchy (full detail):**](http://www.plantuml.com/plantuml/png/xLbhRzf85_xC_8gf-kfjakO2fBcekvLA2OdG0a50jX-g2etj0tXZptXdngHiD_zzZmSRNu8XLTLj8ksN9ZxFSoxdoPdZWRvNcaaT-XuvUaL8LpYX1syhJQQ1imzCmM7WCMvmSXboMxk2loMLVcIywNQeX0b8u3PG9KA9FoP2qa6hST5j7VeEZIYsnvIYZikcalc7f4AQ8bX8TphJ66W8G9bj2pzWVE7oAUsqcwtUi4KdhWV8ROHw9YGIHo3bWdQ5aB1FkyrccpQ538HaKKMLdfY3Ru6azMhjd92UqA2oPRe-cuAYBezgwrwOsfW4EWKE608SQYrem9IEIj0pLvdyoDU2LFGCkmCvn_1CqvdMmTkZexkxkvKqXrZ_KVAmJnbtAFPgMSjWs8O45NeczisWemuf7SsYI4nHBQW5r14yIe4Pc4UrwuFQftUkdg5g4wEl9fKvyq9i0HmNopMLHR7fd4cNMHuY5dZYxX130yLwV7IA6ax_6AHovteO7l2VXGZf7TRmIb4Eu81pICYh8q9U_0zu5Arhr5Jq7QsjR4Fj7zVHKaqjp7SCXN26DGVC1jgrRNSvFVGh8SnIMZ9RnqEGZXM2bBv6C4ImTTC-VJEsmgZNiVxb0O8Fg5qUkU7kdo6qBrPkdpwlWnFV10OU-lIwtneqHktUrVYoqxWQfa2_qnXTNW-w6VjNfPbsxViG5J-kZpN5cT8uvFwbnwPgV0KQpqmlZEJQsSrvM49ur1PSQVWIoBpVwt5ReHyA7plDXUlG8Pj07ylLazXgl6CHFiHzqZtCELmej2O94vNlb_ncOzJgDSP9CyPXbLt8ziK3FvCTMObgIVePa-YTsfTY9kZMP35n9Hr2rq5qtZ28aI7NV0cOzSvfkBuB2wOkBn5csKyyXJtcmyukDSYwG4CF87WOw58od-vDGKU_PEiFXETbdoKMeZJ8Q-iFi7MCCCTfujbat4dAxqinTvKhEBOzr0iFJ9z5Oqms8uLummgob3QVY0BZMZDMGZ7LzK0FsHo6e2B2PdIecSvLW32cAFWc8BRO4GFsaV6aqyAvo2o1lI1zY3WlKw5p6kOZ5LdJT316qLegPJM3iEsiezf3a2xp1i2SHSKVPvuCdDzF50dc-2rT-yBpKE0Mnxa3nBBFqpYwbi3yDaSxD146Mmk9rDNlIrhSvPADSEuveXmaPm71_5anQCc1zCT1qhEX6JKKcqDwJ5FxDBSgZ60HV4zUk1EwazlxkFXtro_u2A_NwDVyqQ5cFBBdCsDOTIRc8ANhG9UfsnNhjiWAcnum6N7sQMeSWaxTIdPQyMm_RkGEbeqDHasKVg1jemXmV3sJL9dq7LakZI4yIMMs2r7IzDPnk0IcPo2xLUmxUTCsUetEzTMut-vbNhyNxQlsgD59hGzqcNY2QHgTpDyjfhFh-sLnsHUtdtapFLo9MttDdZ3EPgqrUhUmYCLOt-v7T8fU3lNR7zdlw-ARA0s0fWz2wAO8kTxvlvNykcnrqv3RSTauuvVouSfP1Vs-wU_W71IR_OwECkUeNAqIKgXofE9ks2xM-NQn391h7_T7kaLAXkZDoScN7pe-PJBSbnLqMdjg1Hq_iOBEgp-beErRELVbcngUl_A2iZVbzHVbbu_UaOQzxV9raDmBoy9ERyTnrF5l67QDJxD_qxvOtNZ6tKQVpaswaKWIVlgyna-vVy5OKruaHv8MHAl_9zgtYtQ2eZrIdlAB_BUAKRm0x-pHfNsUkAfFdtStDPRmVf9gfz-lseyLvK75Pq-zuktNbSAxj_ROXlv5cKzhpsmmiEBQiwku_kmgFdjs5Py_jugFdzl9Ew_Mdbt5cqzUoKNOvK-WbwqNyCnNdh_-VN2G-vgi_DMqqTamjkTDynZduZSofOpqImnIS4bOoOU9pNWTyKB0FE5a6-5q6-6i-aW9bvQYXfqgoJqbXEHJ-0RqjBOHFTw4ueT5Gj8iXQmragOefAoMGnkywkNGMJbqJj9U2hrLITf8eR5QERIQsFlaZrNu_yPtjBfBPgu3r5-Q4eVt-2FqlNy0)](https://www.plantuml.com/plantuml/svg/xLbhRzf85_xC_8gf-kfjakO2fBcekvLA2OdG0a50jX-g2etj0tXZptXdngHiD_zzZmSRNu8XLTLj8ksN9ZxFSoxdoPdZWRvNcaaT-XuvUaL8LpYX1syhJQQ1imzCmM7WCMvmSXboMxk2loMLVcIywNQeX0b8u3PG9KA9FoP2qa6hST5j7VeEZIYsnvIYZikcalc7f4AQ8bX8TphJ66W8G9bj2pzWVE7oAUsqcwtUi4KdhWV8ROHw9YGIHo3bWdQ5aB1FkyrccpQ538HaKKMLdfY3Ru6azMhjd92UqA2oPRe-cuAYBezgwrwOsfW4EWKE608SQYrem9IEIj0pLvdyoDU2LFGCkmCvn_1CqvdMmTkZexkxkvKqXrZ_KVAmJnbtAFPgMSjWs8O45NeczisWemuf7SsYI4nHBQW5r14yIe4Pc4UrwuFQftUkdg5g4wEl9fKvyq9i0HmNopMLHR7fd4cNMHuY5dZYxX130yLwV7IA6ax_6AHovteO7l2VXGZf7TRmIb4Eu81pICYh8q9U_0zu5Arhr5Jq7QsjR4Fj7zVHKaqjp7SCXN26DGVC1jgrRNSvFVGh8SnIMZ9RnqEGZXM2bBv6C4ImTTC-VJEsmgZNiVxb0O8Fg5qUkU7kdo6qBrPkdpwlWnFV10OU-lIwtneqHktUrVYoqxWQfa2_qnXTNW-w6VjNfPbsxViG5J-kZpN5cT8uvFwbnwPgV0KQpqmlZEJQsSrvM49ur1PSQVWIoBpVwt5ReHyA7plDXUlG8Pj07ylLazXgl6CHFiHzqZtCELmej2O94vNlb_ncOzJgDSP9CyPXbLt8ziK3FvCTMObgIVePa-YTsfTY9kZMP35n9Hr2rq5qtZ28aI7NV0cOzSvfkBuB2wOkBn5csKyyXJtcmyukDSYwG4CF87WOw58od-vDGKU_PEiFXETbdoKMeZJ8Q-iFi7MCCCTfujbat4dAxqinTvKhEBOzr0iFJ9z5Oqms8uLummgob3QVY0BZMZDMGZ7LzK0FsHo6e2B2PdIecSvLW32cAFWc8BRO4GFsaV6aqyAvo2o1lI1zY3WlKw5p6kOZ5LdJT316qLegPJM3iEsiezf3a2xp1i2SHSKVPvuCdDzF50dc-2rT-yBpKE0Mnxa3nBBFqpYwbi3yDaSxD146Mmk9rDNlIrhSvPADSEuveXmaPm71_5anQCc1zCT1qhEX6JKKcqDwJ5FxDBSgZ60HV4zUk1EwazlxkFXtro_u2A_NwDVyqQ5cFBBdCsDOTIRc8ANhG9UfsnNhjiWAcnum6N7sQMeSWaxTIdPQyMm_RkGEbeqDHasKVg1jemXmV3sJL9dq7LakZI4yIMMs2r7IzDPnk0IcPo2xLUmxUTCsUetEzTMut-vbNhyNxQlsgD59hGzqcNY2QHgTpDyjfhFh-sLnsHUtdtapFLo9MttDdZ3EPgqrUhUmYCLOt-v7T8fU3lNR7zdlw-ARA0s0fWz2wAO8kTxvlvNykcnrqv3RSTauuvVouSfP1Vs-wU_W71IR_OwECkUeNAqIKgXofE9ks2xM-NQn391h7_T7kaLAXkZDoScN7pe-PJBSbnLqMdjg1Hq_iOBEgp-beErRELVbcngUl_A2iZVbzHVbbu_UaOQzxV9raDmBoy9ERyTnrF5l67QDJxD_qxvOtNZ6tKQVpaswaKWIVlgyna-vVy5OKruaHv8MHAl_9zgtYtQ2eZrIdlAB_BUAKRm0x-pHfNsUkAfFdtStDPRmVf9gfz-lseyLvK75Pq-zuktNbSAxj_ROXlv5cKzhpsmmiEBQiwku_kmgFdjs5Py_jugFdzl9Ew_Mdbt5cqzUoKNOvK-WbwqNyCnNdh_-VN2G-vgi_DMqqTamjkTDynZduZSofOpqImnIS4bOoOU9pNWTyKB0FE5a6-5q6-6i-aW9bvQYXfqgoJqbXEHJ-0RqjBOHFTw4ueT5Gj8iXQmragOefAoMGnkywkNGMJbqJj9U2hrLITf8eR5QERIQsFlaZrNu_yPtjBfBPgu3r5-Q4eVt-2FqlNy0 "Click for SVG image of SAMD Class Hierarchy")

### Platform Management

The hardware supported by this platform is generally similar. The architecture allows for the following kinds of variation (as outlined in the class hierarchy):

1. CPU differences (Cortex M0, RISC-V, etc.)
2. SOC differences (SAMD21, STM32L0, etc.)
3. PC-board differences (different sensors, power supply, capabilities)
4. Optional component population differences (pull-up resistor values, etc.)
5. Externally-connected sensors (one-wire temperature sensors, etc.)

Items 1-3 are to some degree known at compile time, based on the Catena model chosen. However, it's inconvenient to update the BSP for every possible modification, so we allow some variation at run time, guided by the contents of FRAM.

The system is identified to the software by a platform object, of type `CATENA_PLATFORM`. Several platform objects are built into the firmware image, based on the known variations for component population and external sensors. The appropriate platform object is located at boot time by the Catena Arduino Platform framework. Some values representing possibly variation are stored as `PlatformFlags` in the `CATENA_PLATFORM`. This variable is of type`PLATFORM_FLAGS`.

#### Platform GUIDs

Each `CATENA_PLATFORM` has a unique identification. This is a 128-bit binary number called a GUID (or [UUID](https://en.wikipedia.org/wiki/Universally_unique_identifier)), generated by MCCI during the system design process. The platform GUIDs are defined in the header file [`Catena_Guids.h`](./src/Catena_Guids.h). For convenience, here's a table of the known GUIDs.

All of these names begin with the string `GUID_HW_`, so we omit that from the tables below.

The M101 and M102 designations are used by the [Catena-Sketches](https://github.com/mcci-catena/Catena-Sketches) family of applications to determine what external sensors are available. This lets them avoid trying to poll external hardware unless the appropriate platform configuration is set. The well known configurations are:

- **M101**:  external sensors on screw terminals are contact closures, and should be totalized.
- **M102**: external sensors on screw terminals are a mixture of one-wire temperature sensors and two-wire soil temperature/humidity probes.

The designations "M103" and "M104" are reserved for use by MCCI.

For boards with FRAM, the appropriate platform GUID should be selected and programmed into FRAM using the command `system configure platformguid`, followed by the GUID value. For boards without FRAM, the library has provisions for tying the GUID to the CPU serial number. Contact MCCI for details.

The tables below were generated from `Catena_Guids.h` using a script, and then hand annotated. The script is a one-line shell command using `awk`:

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
`CATENA_4610_M102` | `18252b1c-3c0d-403e-8012-224d96c5af06` | Catena 4610 M102, configured for environmental monitoring
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

The Catena 4630 is a variant of the Catena 4610. It deletes the Si1131 light sensor, and adds an IDT ZMOD4410 atmospheric gas sensor, plus a connection for an external Plantower PMS7003 PM2.5/dust sensor.

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

#### GUIDs for the Catena 480x family

##### Catena 4801

Name | GUID | Description
-----|------|------------
`CATENA_4801_BASE` | `10ea7e25-a4a4-45fd-8959-c04a6a5d7f95` | Base Catena 4801, assuming no modifications or customizations.

##### Catena 4802

Name | GUID | Description
-----|------|------------
`CATENA_4802_BASE` | `daaf345e-b5d5-4a32-a303-3ac70b81d260` | Base Catena 4802, assuming no modifications or customizations.

#### GUIDs for Adafruit Feather M0s

MCCI also uses this library with Feather M0s without MCCI hardware. These GUIDs are useful in that situation.

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

You also, of course, must supply an implementation of `UserClass::poll()`.

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

- `TState::stNoChange` does not correspond to any state. Instead, it's the value returned by the dispatch routine when the state is not to be changed. (Returning the current state will cause the FSM to transition from the current state to the current state.)

- `TState::stInitial` is the initial state of your FSM.

- `TState::stFinal` is the final state of your FSM. Once the FSM reaches this state, it will remain there until another call to `fsm.init()` is made.

We'll use the [coin-operated turnstile](https://en.wikipedia.org/wiki/Finite-state_machine#Example:_coin-operated_turnstile) example from Wikipedia to make things more concrete.

#### Getting ready

The author usually starts by drawing a diagram, labeled with the states and transitions.

For example, here's the turnstile state diagram:

[![**Coin Operated Turnstile State Diagram](http://www.plantuml.com/plantuml/png/XP9FRvmm4CNFpAUOMwGg2srltKELY6oaL4iKvS-fxS7WMR1gR6IFboBrmtTW5RHQLUe5u_6ytxndk8ci0gVUGd45K7cTB6sp-vUAVgj-i9GFLhdb7EwJQzXujuNiQIw-LNiCTA10hY6CFWLP3ZvW8t8KBXDgezgW-XmoAFqm1TDsBFeN8bHDu_j1kScu5lSFvUxnPOS7O-M48UkOXxZT5aLhk4jrBhr5tpHcqmZMgQ9SbirjqCauln53BADxcNERkFD1XhnI21DMtWUwngei7x3qOV11pI6oRybE-FdZfoy0ZvufdgSolMefed7ulBkjxdPvhr45mfOSYPmqrXCEAl9idJiJJxwDOmyPTuIHmf621C4vXwGOnt6zoINB--OQbTCeTrJN9nX15YWckx3VdlSnHtpjPfAAo1vhGktTF48cA8jiUehNE1hkK9l3yZaOigEoIIAGDc9tSJQpyQY2KRMbA1phnrpGXEAd5z5xuBjg3WpPQApWIHwJJpYAXwk8ZaYJpW6k2e3l7txYPlCL8-zzyqlcRENrmHasoW8iVy1wjcxVd3qLO9LTCEZub687PvMTup0LE0kN69o2YsmiNJ9c-4eflN5d3POEB4spQV7P9TP-T3-4Ss_SoQ-e_mUJseMfvFvbwu9r6Gx_h0xWXfnElOM_)](https://www.plantuml.com/plantuml/svg/XP9FRvmm4CNFpAUOMwGg2srltKELY6oaL4iKvS-fxS7WMR1gR6IFboBrmtTW5RHQLUe5u_6ytxndk8ci0gVUGd45K7cTB6sp-vUAVgj-i9GFLhdb7EwJQzXujuNiQIw-LNiCTA10hY6CFWLP3ZvW8t8KBXDgezgW-XmoAFqm1TDsBFeN8bHDu_j1kScu5lSFvUxnPOS7O-M48UkOXxZT5aLhk4jrBhr5tpHcqmZMgQ9SbirjqCauln53BADxcNERkFD1XhnI21DMtWUwngei7x3qOV11pI6oRybE-FdZfoy0ZvufdgSolMefed7ulBkjxdPvhr45mfOSYPmqrXCEAl9idJiJJxwDOmyPTuIHmf621C4vXwGOnt6zoINB--OQbTCeTrJN9nX15YWckx3VdlSnHtpjPfAAo1vhGktTF48cA8jiUehNE1hkK9l3yZaOigEoIIAGDc9tSJQpyQY2KRMbA1phnrpGXEAd5z5xuBjg3WpPQApWIHwJJpYAXwk8ZaYJpW6k2e3l7txYPlCL8-zzyqlcRENrmHasoW8iVy1wjcxVd3qLO9LTCEZub687PvMTup0LE0kN69o2YsmiNJ9c-4eflN5d3POEB4spQV7P9TP-T3-4Ss_SoQ-e_mUJseMfvFvbwu9r6Gx_h0xWXfnElOM_ "Click for SVG view")

#### Defining the state `enum class`

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

#### Add the state type to the parent class

Add the type you defined above to the parent class. For example:

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

#### Implement the FSM initialization

Somewhere in your initialization for `Turnstile`, add the following code. For example, if `Turnstile` has a `Turnstile::begin()` method, you could write:

```c++
void Turnstile::begin() {
  // other init code...

  // set up FSM
  this->m_fsm.init(*this, fsmDispatch);

  // remaining init code...
}
```

### The general time/date class `McciCatena::cDate`

When logging data, we frequently need to keep time on a scale that is correlated with other devices. Although the Arduino environment provides interval times based on the `seconds()`, `millis()` and `micros()` APIs, there's no built-in concept of calendar time. The `cDate` class provides calendar time objects and the ability to perform conversions between calendar time and interval time. The `cDate` object is also an important component for clock drivers.

```c++
#include <Catena_Date.h>

// allocate a date object, initially invalid
McciCatena::cDate myDate;
```

#### Interval Seconds

It's common to compare intervals and transmit timestamps using a simple up-counter. Traditional Posix systems count seconds since 1970-01-01 00:00:00Z; GPS systems count seconds since 1980-01-06 00:00:00Z. These base times are commonly called "epochs". Times can be (theoretically) in the past (negative) or future (positive) relative to the epoch. For a variety of reasons, we call times based on the Posix epoch "Common times"; we call times based on the GPS epoch "GPS times". The types `CommonTime_t` and `GpsTime_t` are used to record times in common and GPS times.  Both are of type `std::int64_t`. Both have a range of designated values that are valid; this range is chosen to allow any valid `CommonTime_t` to be converted to `GpsTime_t` and vice versa.

```c++
constexpr cDate::CommonTime_t cDate::kCommonTimeInvalid;
constexpr cDate::CommonTime_t cDate::kGpsTimeInvalid;

constexpr bool cDate::isCommonTimeValid(CommonTime_t);
constexpr bool cDate::isGpsTimeValid(GpsTime_t);

constexpr cDate::CommonTime_t cDate::getCommonTime(GpsTime_t);
constexpr cDate::GpsTime_t cDate::getGpsTime(CommonTime_t);
```

Many numerical values of `std::int64_t` are not valid times. The library uses `kCommonInvalidTime` and `kGpsInvalidTime` when it needs to create an invalid time, but it (and clients of the library) should use `isCommonTimeValid()` or `isGpsTimeValid()` to check whether a given time is in fact valid.

`getCommonTime()` and `getGpsTime()` convert valid times between the two systems, handling invalid cases.

#### `cDate` calendar types

The types `cDate::Year_t`, `cDate::Month_t`, `cDate::Day_t`, `cDate::Hour_t`, `cDate::Minute_t`, `cDate::Second_t` are used to represent years (from 0 to 65535), months (from 1 to 12), days (from 1 to 28, 29, 30, or 31, depending on the month and year), hours (0 to 23), minutes (0 to 59), and seconds (0 to 59). The year zero corresponds to ISO-8601 year zero. We use, technically speaking, a [proleptic Gregorian calendar](https://en.wikipedia.org/wiki/Proleptic_Gregorian_calendar) with [astronomical year numbering](https://en.wikipedia.org/wiki/Astronomical_year_numbering) (i.e., year zero).

#### `cDate` properties

```c++
bool cDate::isValid() const;
```

This function returns `true` if the entries in the `cDate` object are valid, `false` otherwise.

```c++
cDate::Year_t cDate::year() const;
cDate::Monty_t cDate::month() const;
cDate::Day_t cDate::day() const;
cDate::Hour_t cDate::hour() const;
cDate::Minute_t cDate::minute() const;
cDate::Second_t cDate::second() const;
```

These functions return the various fields of the date.

```c++
cDate::CommonTime_t cDate::getCommonTime() const;
cDate::GpsTime_t cDate::getGpsTime() const;
```

These functions return the `CommonTime_t` or `GpsTime_t` equivalent of the date object.

#### `cDate` methods

```c++
bool cDate::setDate(Year_t y, Month_t m, Day_t d);
```

Set the date portion of the `cDate` instance (only if a valid date is passed). Return `true` if and only if the date was updated.

```c++
bool cDate::setTime(Hour_t h, Minute_t m, Second_t s);
```

Set the time portion of the `cDate` instance (only if a valid time is passed). Return `true` if and only if the time was updated. Time is set in zone `UTC+0`.

```c++
bool cDate::setCommonTime(CommonTime_t commonTime);
bool cDate::setGpsTime(GpsTime_t gpsTime);
```

Set the date and time of the `cDate` instance from the common or GPS time stamp. Returns `false` if the incoming timestamp is invalid, or if the specified time is out of range.

#### Timekeeping, solar days, leap seconds

This section is provided for background, and can be skipped if you're not interested in the theory behind the implementation.

Timekeeping is a thorny topic for scientific investigations, because one day is not exactly 86,400 seconds long. Obviously, the difference between two instants, measured in seconds, is independent of calendar system, but converting the time of each instant into ISO date and time is **not** independent of the calendar. Worse is that computing systems (e.g. POSIX-based systems) focus more on easy, deterministic conversion from a time serial number to UTC time, and so assume that there are exactly 86400 seconds/day. In UTC time, the solar calendar date is paramount; leap-seconds are inserted or deleted as needed to keep UTC mean solar noon aligned with astronomical mean solar noon.

In effect, the computer observes a sequence of seconds. We need to correlate them to calendar time, and we need to interpret know the interval between instances. Let's call the sequence of seconds _interval time_, as opposed to _calendar time_.

Let's also define an important property of sequences of seconds -- "interval-preserving" sequences are those in which, if T1 and T2 are interval second numbers, (T2 - T1) is equal to the number of ITU seconds between the times T1 and T2.

Real-time calendar clocks typically measure intervals using a mixed-radix system (year/month/day hour:minute:second). This looks much like UTC calendar time, but in fact doesn't include leap seconds, and is a pure interval counter (with inconvenient arithmetic).

There are (at least) three ways of relating interval time to calendar time.

1. Keep interval time interval-preserving, and convert to calendar time as if days were exactly 86,400 seconds long. (GPS is such a time scale.) Differences between instants (in seconds) are in ITU seconds.
2. Keep interval time interval-preserving, but convert to calendar time accounting for leap seconds (most days are 86,400 seconds long, but some days are 86,399 seconds long, others are 86,401 seconds long). (UTC is such a time scale.) Differences between instants (in seconds) are in ITU seconds.
3. Make interval time _not_ interval-preserving by considering leap seconds. A day with 86,401 ITU seconds will have two seconds numbered 86,399; a day with 86,399 ITU seconds will not have a second numbered 86,399. Convert to date/time as if days were exactly 86,400 seconds long. The difference between two instants (in interval time) is not guaranteed to be accurate in ITU seconds. This is how POSIX time works.

Steve Allen's website has a number of good discussions, including:

- [Issues involved in computer time stamps and leap seconds][T1]
- [Two kinds of Time, Two kinds of Time Scales][T2]

[T1]: https://www.ucolick.org/~sla/leapsecs/picktwo.html
[T2]: https://www.ucolick.org/~sla/leapsecs/twokindsoftime.html

The onboard real-time clocks provided by various Catena platforms count intervals in "calendar" time, and are set by people (again in "calendar" time) from watches etc. that run from UTC or a derivative. We avoid the additional complication of local time zones by assuming that the user will use GMT (UTC+0, or "Zulu" time) We will assume that the user can input the time in Zulu time and that the battery-backed RTC is recording time in Zulu time.

Therefore, we use a timescale that simply states that days have 86,400 seconds. In effect, we choose option 1 above. In our applications, we think that this will be good enough. If we ever start to use LoRaWAN ("GPS") time, we assume that the network will be able to send us the information needed to convert to calendar time as needed. This may add a little complication but it's future complication and we'll deal with all this if the need arises.

### LoRaWAN Support

The Catena Arduino Platform includes C++ wrappers for LoRaWAN support, based on the MCCI version of the [Arduino LMIC](https://github.com/mcci-catena/Arduino-LMIC)  library and MCCI's [Arduino LoRaWAN](https://github.com/mcci-catena/Arduino-LoRaWAN) library. It includes command processing from the `Serial` console for run-time (not compile-time) provisioning, and uses the non-volatile storage provided by the Catena FRAM to store connection parameters and uplink/downlink counts.

The `Catena::LoRaWAN` class is derived from the `Arduino_LoRaWAN` class defined by `<Arduino_LoRaWAN.h>`.

The example [`catena_hello_lora.ino`](examples/catena_hello_lora/) is a complete working example.

To use LoRaWAN in a sketch, do the following.

1. Instantiate the global Catena object, with the name `gCatena`.

    ```c++
    #include <Catena.h>

    using namespace McciCatena; // to save typing

    Catena gCatena;  // instantiate the Catena platform object.
    ```

2. Instantiate the global LoRaWAN object, with the name `gLoRaWAN`:

    ```c++
    Catena::LoRaWAN gLoRaWAN;  // the LoRaWAN function.
    ```

3. In your setup function, initialize gCatena, gLoRaWAN, and register gLoRaWAN as a pollable object (see [Polling Framework](#polling-framework)).

    ```c++
    void setup() {
      // other things

      // set up Catena platform.
      gCatena.begin();

      // set up LoRaWAN
      gLoRaWAN.begin(&gCatena);
      gCatena.registerObject(&gLoRaWAN);

      // other things
    }
    ```

#### Sending an uplink message

Use the `Catena::LoRaWAN::SendBuffer()` method to send an uplink message. Usually it's best to send it with an asynchronous callback, so that's what we'll show.

Definitions:

```c++
typedef void (Arduino_LoRaWAN::SendBufferCbFn)(
  void *pClientData,
  bool fSuccess
);

bool Catena::LoRaWAN::SendBuffer(
  const std::uint8_t *pUplinkBuffer,
  size_t nBuffer,
  Arduino_LoRaWAN::SendBufferCbFn *pDoneFn,
  void *pClientData,
  bool fConfirmed,
  std::uint8_t port
);
```

`SendBuffer` attempts to start the transmission of a buffer. This attempt might fail for several reasons, for example:

- A transmission might already be in progress.
- The LoRaWAN system might not be properly provisioned with device identity and suitable keys.
- The LoRaWAN system might be shut down.

If the transmission is not accepted, `SendBuffer()` returns `false`.

If the transmission is accepted, then the following steps are taken:

- `pDoneFn` and `pClientData` are saved internally for use when the transmission completes.
- The data from `pUplinkBuffer` is copied into an internal buffer (so you can immediately start reusing the buffer in your own code).
- If the device is provisioned for OTAA mode, and the device is not yet joined to a network, a JOIN attempt is initiated; and the message is transmitted if the JOIN attempt succeeds.
- The message is transmitted on the uplink port specified by `port`. If `fConfirmed` is `true`, a confirmed (acknowledged) uplink is used; otherwise unconfirmed uplinks are used.
- Control then returns to the caller (with the result `true`).

When the transmission attempt finishes, the LoRaWAN subsystem calls `pDoneFn(pClientData, fSuccess)`, with `fSuccess` true if the uplink seemed to be successful. Success means different things in different circumstances.

- For an unconfirmed uplink, success means that the device is joined to a network, and was able to transmit the message without any local errors. If not joined, and the join attempt fails, then `fSuccess` will be false.
- For a confirmed uplink, success means that the message was sent, *and* a confirmation downlink was received. Failure doesn't necessarily mean that the network didn't receive the message; it only means that we didn't get an acknowledgement.

#### Registering to receive downlink messages

Receiving a message is a somewhat passive operation. The client registers a callback with `gLoRaWAN`; later, whenever a downlink message is received, the client's callback is called.

```c++
typedef void Arduino_LoRaWAN::ReceivePortBufferCbFn(
  void *pCtx,
  uint8_t uPort,
  const uint8_t *pBuffer,
  size_t nBuffer
  );

void Arduino_LoRaWAN::SetReceiveBufferBufferCb(
  ReceivePortBufferCbFn *pReceivePortBufferFn,
  void *pCtx
  );
```

#### LoRaWAN Class Structure

In order to allow code to be portable across networks and regions, we've done a lot of work with abstraction classes. If you're curious, here's a somewhat simplified diagram (click on the diagram to get an enlarged SVG version).

[![**LoRaWAN class structure**](http://www.plantuml.com/plantuml/png/fPVFRjj64CRFcgSO-E9BH2KkQjXg3n4K1tLXkQdje4TZIOx8hSbTOl_45TAyUybXrn0TSmd5nvtTtt3c-xWQyRrrmZXVLx0u1jZet5UqdAs58oNkh_ID-6jrdJILK7m6FdgLEQdL4cQVkk3dpHKQsf8XbH5QxKr6kDK6-mpBvVyfCAk4jPXBKHXH9m1htUoCB4hNPhebGf5bkcw4sabLuDNb-kBwzWAtiY8K3cQbSurTBXUBGhhIfqbxTb5dcPnd_9X5_xJvokHUAZrlItDj4JN0ohjIc_OPTsJC3ZTQ6tg3c_Nw4jVQDDg8hflPxxxQuSdRT-S0rzgHtMzFrg8WYrBXpSLgy-4YgNCKXh0WHIrEEQOxR8HrNUMkbBOlMogdOUPAGalc2naK3hissYOU7n-V9ELMmk8D2fLZgr2Qzia8PeQihpZtvvihcm0SBm1AcHDItRWTrbIdP2n08x87KH064OBomk9Hrt3OmB1n9LCZpEu8lu98hJCYSxrDuMJm7oRsxskHbLBHd_UfiCGFJ99Xy0XW0lNs_DtFWHr69-790frJWzh2cZEF0UJFJi_s2GuSHOX13q-1627yaE10dB3d9pyDY2uG8wGwEptT9pWG8nxC-SdRVO83CR4c7LXbJbRIxGOs38ASFeOE30bcn96X8xoOGeQMy68A6Nh2YobaQ0elff2XAy6HE3DfoszKILyFJ0cXO4a4Uq7TE07jb45N4ZhTu5QekLHul81dX_7NV-Vp-8zC71bU--dJGz4YI4G-RYtKo4t5wfzeaRM4-7QR8ab6Ku3IZf0dB6g5bLGFW7X3XMnNbYhAkiab77PZjvjNJjO4sCsoomyNg4stKGkLToW1gHoxZ6FsAdAFsZom27SbtRKr5FQw3x8O8ywC-yRt-t0kI7Sm9_oFSNpT3-VwEtyurrzyW579-BLuYOHHjGv3X3yOOOqEGrYUmn1MPXJXLpDSSNubenEAzOzFebTbOE5VbK7ursPWMrwLWLrwJORmEiULPbF7XrnSv1-5XP-6uz9Em75L9-2ue76Ohq8_klWYZ0-rwDHZTSVeeVGZVmNuBu3s3lKMaoGCaTZmV35Z05-UUHZ6K6JgZOozv6dklVI34wzbJFmeDQ9cd85l04hKP1kH4MwoJFQVm_WrJEliawugaLRqH_etPQwFFlz4trCF46zrJQnIHVzqLPSopqdzqktub7coc9Eey56w4dFw8ZC2R16rbONllwkn4_BZpMe3YDwAObzGU5PKzwfzLn8a8qsp8jyV_mRlIUM-hlu3)](https://www.plantuml.com/plantuml/svg/fPVFRjj64CRFcgSO-E9BH2KkQjXg3n4K1tLXkQdje4TZIOx8hSbTOl_45TAyUybXrn0TSmd5nvtTtt3c-xWQyRrrmZXVLx0u1jZet5UqdAs58oNkh_ID-6jrdJILK7m6FdgLEQdL4cQVkk3dpHKQsf8XbH5QxKr6kDK6-mpBvVyfCAk4jPXBKHXH9m1htUoCB4hNPhebGf5bkcw4sabLuDNb-kBwzWAtiY8K3cQbSurTBXUBGhhIfqbxTb5dcPnd_9X5_xJvokHUAZrlItDj4JN0ohjIc_OPTsJC3ZTQ6tg3c_Nw4jVQDDg8hflPxxxQuSdRT-S0rzgHtMzFrg8WYrBXpSLgy-4YgNCKXh0WHIrEEQOxR8HrNUMkbBOlMogdOUPAGalc2naK3hissYOU7n-V9ELMmk8D2fLZgr2Qzia8PeQihpZtvvihcm0SBm1AcHDItRWTrbIdP2n08x87KH064OBomk9Hrt3OmB1n9LCZpEu8lu98hJCYSxrDuMJm7oRsxskHbLBHd_UfiCGFJ99Xy0XW0lNs_DtFWHr69-790frJWzh2cZEF0UJFJi_s2GuSHOX13q-1627yaE10dB3d9pyDY2uG8wGwEptT9pWG8nxC-SdRVO83CR4c7LXbJbRIxGOs38ASFeOE30bcn96X8xoOGeQMy68A6Nh2YobaQ0elff2XAy6HE3DfoszKILyFJ0cXO4a4Uq7TE07jb45N4ZhTu5QekLHul81dX_7NV-Vp-8zC71bU--dJGz4YI4G-RYtKo4t5wfzeaRM4-7QR8ab6Ku3IZf0dB6g5bLGFW7X3XMnNbYhAkiab77PZjvjNJjO4sCsoomyNg4stKGkLToW1gHoxZ6FsAdAFsZom27SbtRKr5FQw3x8O8ywC-yRt-t0kI7Sm9_oFSNpT3-VwEtyurrzyW579-BLuYOHHjGv3X3yOOOqEGrYUmn1MPXJXLpDSSNubenEAzOzFebTbOE5VbK7ursPWMrwLWLrwJORmEiULPbF7XrnSv1-5XP-6uz9Em75L9-2ue76Ohq8_klWYZ0-rwDHZTSVeeVGZVmNuBu3s3lKMaoGCaTZmV35Z05-UUHZ6K6JgZOozv6dklVI34wzbJFmeDQ9cd85l04hKP1kH4MwoJFQVm_WrJEliawugaLRqH_etPQwFFlz4trCF46zrJQnIHVzqLPSopqdzqktub7coc9Eey56w4dFw8ZC2R16rbONllwkn4_BZpMe3YDwAObzGU5PKzwfzLn8a8qsp8jyV_mRlIUM-hlu3 "Click for SVG version")

As the diagram shows, Catena::LoRaWAN objects are primarily `Arduino_LoRaWAN` derivatives, but they also can be viewed as `McciCatena::cPollableObject` instances, and therefore can participate in [polling](#polling-framework).

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

There is an escape clause. If bit 31 of the first `uint32_t`is set, the object is not "standard". In such a case, the contents of the object after the standard header cannot be used for a standard data payload (as defined above). This may be desirable payloads that are written only once, when the FRAM is initialized; but it leaves redundancy management to the client.

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
| 31 | `fNonStandard` | `cFramStorage::NONSTD_MASK` | If zero, the object's payload uses the redundant scheme described above; the payload size is necessarily limited to 32767 byes. If non-zero, the object's payload uses a client-supplied encoding and representation; but can use up to 256 k bytes (since the object size can represent up to 256 k bytes) |

##### The FRAM header object

An FRAM store managed by this library is expected to begin with a header object. A header object is identified by the well-known GUID `{1DE7CDCD-0647-4B3C-A18D-8138A3D9613F}` and the key `kHeader` (zero).

The header object carries a single 4-byte (`uint32_t`) payload, which is interpreted as the end-of-storage address -- the offset of the first byte on the FRAM that is not used for object storage. If an object is added to the store, this pointer is updated after the new object object has been fully committed. The new object is not permanently committed until the end-of-storage pointer is atomically updated.

#### Adding FRAM objects

1. Determine the GUID and key you want to use. If you are adding the item as part of the Catena library, you can use the GUID `GUID_FRAM_CATENA_V1(WIRE)`, `{1DE7CDCD-0647-4B3C-A18D-8138A3D9613F}`; add the key to `McciCatena::cFramStorage::StandardKeys`, defined in `Catena_FramStorage.h`.

   There is no presentable way to use a non-standard GUID; several changes must be made in `Catena_Fram.cpp` to enable this.

2. Ultimately, the metadata for your new object is represented by a 32-bit value of type `cFramStorage::StandardItem`. The constructor takes three (optionally four) arguments:

   - `uKey`, the 8-bit key value
   - `uSize`, the 16-bit object size. (If your object is variable size, you must specify a maximum size, and the actual size of the object must be represented as part of the object data somehow.)
   - `fNumber`, a Boolean value. If true, then the value represents a little-endian value; if false, big-endian. This is used for displays and the command interpreter.
   - Optionally `fReplicated` (assumed `true`), which controls whether the replicated data-storage scheme should be used.

3. Find the table `McciCatena::cFramStorage::vItemDefs[]` in `Catena_FramStorage.cpp`, and add your `StandardItem` value at the appropriate offset.

4. To query the value of your object, you can use `gCatena.getFram()->getField(uKey, Value)`; this is a templated function which will set Value according toe the current value stored for `uKey`.

   - You may also use `gCatena.getFram()->getField(uKey, (uint8_t *)&buffer, sizeof(buffer))`.

5. To set the value of your object, you can use `gCatena.getFram()->saveField(uKey, Value)`; this is a templated function which will write Value to the object identified by `uKey`.

   - You may also use `gCatena.getFram()->saveField(uKey, (const uint8_t *)&buffer, sizeof(buffer))`.

#### Class hierarchy within the FRAM library

<!-- The following image renders well on GitHub, but doesn't render in the previewer in Visual Studio Code unless you turn on "insecure content". Of course, GitHub rendering is more important, but this is irksome -->
[![Image of `cFram` -- see `assets/cfram.plantuml`](http://www.plantuml.com/plantuml/png/ZLLXRzem4FsUNs5fqihAK3KcJHDKKoiw9CJGAjZwPPgYmpRKKcmZsw5YZF--ivCGGaEoBpHzztn-xt4Nxgm3urAPH3TNv341vNxElX4XOSt9IXiodj_FegkRI87aTWEKancbOWIEAo3x29RM6Q2Eq0Ii9VIH6oii5jt2QUERx2D2klk2To1RWTT1GfPQumTV2zLvDspY22MSW4JyuIr4mortRSti4xPYhQmv1xQxpDfgmL3OrlV2A3pn44Krxc__zdg4ZWt8YBmAoyDPFXT3QKxYelq6pmr66znLOx0e8NPu8zGaQITPzSM8khF1THxFOF1zjKBLPCsTBMq5QS3OI4i9YjcY2Owg4_H0StpXK80S38x_WaijOVeXbPks8zKJyzFel7aBMpuMF2diEIut2KP1Rjrkm_qka1zVI2qrY4S_xSThc6y29kDL-5eyK1kNZ7Lu605zvhz-UrugbFiS-rBwn4rU63Pqu8fbpu4Kh2GQM3YPYmbW6AjNQ85zLSxaurD_4II9fiSXolo5oPXWA0_WeDYTSYNwKxyKtRDTw9uFNG6KSzPjdQtUFOELIOMB_NijdlH7cCJpIn0Gjh7Sr_wq8Wfd9ZKMNuX7QHIL51VdoxJKgM8L93D_YkLnLANY92e6XlbG_fCwvJjRKj4afY0hmUJOqVoHsIc6vVfzligLigz-x7UoFyXwMrOtgUjGhQmZ3IsGrDG2ZENuQabvHIp-raXm9Twx793NE2dCPZnoBUEDV_28l_N3fsmN5jYzmFBrmE3E45oO0WbgY_KQdin5hpe6MBd33SBVpNy0)](https://www.plantuml.com/plantuml/svg/ZLLXRzem4FsUNs5fqihAK3KcJHDKKoiw9CJGAjZwPPgYmpRKKcmZsw5YZF--ivCGGaEoBpHzztn-xt4Nxgm3urAPH3TNv341vNxElX4XOSt9IXiodj_FegkRI87aTWEKancbOWIEAo3x29RM6Q2Eq0Ii9VIH6oii5jt2QUERx2D2klk2To1RWTT1GfPQumTV2zLvDspY22MSW4JyuIr4mortRSti4xPYhQmv1xQxpDfgmL3OrlV2A3pn44Krxc__zdg4ZWt8YBmAoyDPFXT3QKxYelq6pmr66znLOx0e8NPu8zGaQITPzSM8khF1THxFOF1zjKBLPCsTBMq5QS3OI4i9YjcY2Owg4_H0StpXK80S38x_WaijOVeXbPks8zKJyzFel7aBMpuMF2diEIut2KP1Rjrkm_qka1zVI2qrY4S_xSThc6y29kDL-5eyK1kNZ7Lu605zvhz-UrugbFiS-rBwn4rU63Pqu8fbpu4Kh2GQM3YPYmbW6AjNQ85zLSxaurD_4II9fiSXolo5oPXWA0_WeDYTSYNwKxyKtRDTw9uFNG6KSzPjdQtUFOELIOMB_NijdlH7cCJpIn0Gjh7Sr_wq8Wfd9ZKMNuX7QHIL51VdoxJKgM8L93D_YkLnLANY92e6XlbG_fCwvJjRKj4afY0hmUJOqVoHsIc6vVfzligLigz-x7UoFyXwMrOtgUjGhQmZ3IsGrDG2ZENuQabvHIp-raXm9Twx793NE2dCPZnoBUEDV_28l_N3fsmN5jYzmFBrmE3E45oO0WbgY_KQdin5hpe6MBd33SBVpNy0 "SVG image of cFram hierarchy")

<!-- The following image renders well on GitHub, but doesn't render in the previewer in Visual Studio Code unless you turn on "insecure content". Of course, GitHub rendering is more important, but this is irksome -->
[![Image of FRAM Storage objects -- see `assets/cframstorage.plantuml`](http://www.plantuml.com/plantuml/png/ZLJRJXin47tVhx3sXPjMY29LA5GLRI8GQjDKYU05eMhYdGrkrjx85mga_7jjxoKsMQA-9CgSCpDdpZXpeJH8RNYP7E-HdoK8VJF-JXY71PAYakHo-cMSx1qd0ZYg5L0aOqhP43GA8EiaeIKeHUYb13xJbNHfwuIG8nI6ava6aOFCl-6pSa7zlC8S2sAOqASVVcaoAGg5-jnXQoj4C_fAAw5qbtcjaPD7zpd2GjtTEshosjTsC8Xod8YiwyDXYRTG6jm_yCqwO85wnbxQSAZNGVQxGUPLLHAcVXW-Hnd7fhWg6RM6v4sydk3gvdf43csHAyCQPAF7U37dxqqocVz6ggVmf_Poa-QqMb4kwEkxsYtZp14QiFjzY_Bkz3vGsuwuV-2bzhJrebjQ7RBadKetCay-rgRMyThQ21Fur83rxeE5nBb3Q-fIXLQhk6oSzm3AUihoFipM78660FBrE6ZwW6gpdoER4A-zVPI1VFk0ysrjZwvtNvEUzs64OWk1EH4D9BHw-ouhRywaw1W1uQmUQMYan2r0lGUuhOwOef9n9i3kxwifYkujAokxSMgR4ypKy5fv4Nchr5Psg2LgNribc6RMnxS6-hswa7k3uQ59Jr9oc6MVIReSmrEwwnmCmYEo-tpuOnKtyat-bnt-VJ_zpJAIzg5fajXvBb3azlR-0m00)](https://www.plantuml.com/plantuml/svg/ZLJRJXin47tVhx3sXPjMY29LA5GLRI8GQjDKYU05eMhYdGrkrjx85mga_7jjxoKsMQA-9CgSCpDdpZXpeJH8RNYP7E-HdoK8VJF-JXY71PAYakHo-cMSx1qd0ZYg5L0aOqhP43GA8EiaeIKeHUYb13xJbNHfwuIG8nI6ava6aOFCl-6pSa7zlC8S2sAOqASVVcaoAGg5-jnXQoj4C_fAAw5qbtcjaPD7zpd2GjtTEshosjTsC8Xod8YiwyDXYRTG6jm_yCqwO85wnbxQSAZNGVQxGUPLLHAcVXW-Hnd7fhWg6RM6v4sydk3gvdf43csHAyCQPAF7U37dxqqocVz6ggVmf_Poa-QqMb4kwEkxsYtZp14QiFjzY_Bkz3vGsuwuV-2bzhJrebjQ7RBadKetCay-rgRMyThQ21Fur83rxeE5nBb3Q-fIXLQhk6oSzm3AUihoFipM78660FBrE6ZwW6gpdoER4A-zVPI1VFk0ysrjZwvtNvEUzs64OWk1EH4D9BHw-ouhRywaw1W1uQmUQMYan2r0lGUuhOwOef9n9i3kxwifYkujAokxSMgR4ypKy5fv4Nchr5Psg2LgNribc6RMnxS6-hswa7k3uQ59Jr9oc6MVIReSmrEwwnmCmYEo-tpuOnKtyat-bnt-VJ_zpJAIzg5fajXvBb3azlR-0m00 "SVG image of cFramStorage class hierarchy")

### Asynchronous Serial Port Command Processing

The Catena Arduino platform provides both an asynchronous command-line collection object and a full command parser.

The `Catena::begin()` method normally creates a command parser instance that's linked to a command parser instance. For

#### Collecting lines asynchronously from streams

The header file `Catena_StreamLineCollector.h` defines the class `cStreamLineCollector`. This class is a `cPollableObject`, and as such is polled automatically by the governing `cPollingEngine`. A read is launched by calling `cStreamLineCollector::readAsync()`, passing a callback function, a buffer (base and size), and a context handle. When a command has been accumulated, the specified callback function is called according to the following prototype:

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

Multiple command tables can be registered dynamically; this allows modules to add commands as they are initialized. There's no need to edit a central command table.

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

A command function may operate synchronously or asynchronously.

#### Command stream methods for use by functions

Command stream functions may call any of these functions:

- `pThis->printf()` formats results to pass back to the command source.
- `pThis->getuint32()` scans an argument and converts to `uint32_t`.
- `pThis->completeCommand(CommandStatus)` signals the completion of an [asynchronous command](asynchronous-command-functions).

#### Synchronous Command Functions

A synchronous command function does all of its work in the initial function call, and returns a status code.  The status code can be any value _except_ `CommandStatus::kPending`. Synchronous commands must not call `pThis->completeCommand(CommandStatus)`.

#### Asynchronous Command Functions

An asynchronous command function allows for work to continue after the initial function call. The main command function typically has two parts.

1. The first part of the command is normally coded synchronously; it checks parameters, etc., and returns non-`kPending` status. In this part of the command, there's no chance of `pThis->completeCommand` being called.

2. The second part of the command is coded asynchronously. The asynchronous paths each call `pThis->completeCommand()` when all work has been done. Once the function has established at least one asynchronous completion path, the main function must return `kPending` (and must ensure that all the completion paths call `completeCommand()`).

### Clock Management and Calibration

On some platforms, the system clock needs to be calibrated explicitly in order for the real-time ticks from `micros()` and `millis()` to be accurate. Do this by calling `uint32_t gCatena.CalibrateSystemClock()`. This function updates the clock calibration, and returns a platform-specific value indicative of the calibration. On platforms that don't support (or that don't need) calibration, a dummy implementation is provided that returns 0.

### Watchdog Timer

The independent watchdog is used to detect and resolve malfunctions due to software failures. It triggers a reset sequence when it is not refreshed within the expected time-window (we use 26 seconds time-window). Along with the watchdog timer, we use `SafeDelay()` function.

#### SafeDelay()

It serves as an alternative to the Arduino `delay()` function. Its purpose is to refresh the watchdog time-window, thus preventing any potential resets during delay operations within the application. Like Arduino `delay()`, it accepts milliseconds as a parameter.

### Si1133 driver

The library includes a simple driver for the SiLabs 1133 light sensor found on many Catena boards.

The header file is `Catena_Si1133.h`.  It defines the class `Catena_Si1133`.

The constructor, `Catena_Si1133()`, takes no arguments.

Call `Catena_Si1133::begin()` prior to using the sensor, and `Catena_Si1133::end()` to shut it down.

`Catena_Si1133::configure()` configures one channel of the sensor. It has one of two forms. The original form allows you to choose from pre-configured measurement profiles. Prototype:

```c++
bool configure(uint8_t uChannel, uint8_t uMode, uMeasurementCount = 0);
```

Up to six channels may be configured. `uMode` is one of the following: `CATENA_SI1133_MODE_NotUsed` to configure a channel as not used, `CATENA_SI1133_MODE_SmallIR` to use the small IR sensor, `CATENA_SI1133_MODE_MediumIR` to use the medium IR sensor, `CATENA_SI1133_MODE_LargeIR` to use the large IR sensor, `CATENA_SI1133_MODE_White` to use the regular white sensor,
`CATENA_SI1133_MODE_LargeWhite`  to use the large white sensor,
`CATENA_SI1133_MODE_UV` to use the ultraviolet sensor,
`CATENA_SI1133_MODE_UVDeep` to use the deep UV sensor.

`uMeasurementCount` is zero to use the channel in forced mode, and non-zero to have the channel run in autonomous mode.

An advanced form provided complete flexibility. A special object is defined, `Catena_Si1133::ChannelConfiguration_t`, which can represent all aspects of a measurement. To set up a measurement configuration, write something like this:

```c++
auto const measConfig = Catena_Si1133::ChannelConfiguration_t()
    .setAdcMux(Catena_Si1133::InputLed_t::LargeWhite)
    .setSwGainCode(7)
    .setHwGainCode(4)
    .setPostShift(1)
    .set24bit(true)
    .setCounter(Catena_Si1133::ChannelConfiguration_t::CounterSelect_t::MeasCount2);
```

This creates a value, `measConfig`, that defines a measurement of the large-white LED, with software gain 7, hardware gain 4, and a post-shift of 1. The measurement, if periodic, will be driven by counter 2. (In forced mode, the counter settings are ignored.)

A number of methods allow you to modify and query `ChannelConfiguration_t` values.

Parameter | Setter | Getter | Type | Comments
----------|--------|--------|------|----------
ADC input | `.setAdcMux()` | `.getAdcMux()` | `Catena_Si1133::InputLed_t` |
Software gain code | `.setSwGainCode()` | `.getSwGainCode()` | `uint8_t` | This is log2 of the gain.
Hardware gain code | `.setHwGainCode()` |  `.getHwGainCode()` | `uint8_t` | This is log2 of the gain.
High range select | `.setHsig()` | `.getHsig()` | `bool` | High range divides gain by 14.5
Interrupt Threshold | `.setInterruptThreshold()` | `.getInterruptThreshold()` | `Catena_Si1133::Threshold_t` | Either no interrupt, or one of three threshold registers.
Post-shift | `.setPostShift()` | `.getPostShift()` | `uint8_t` | Divides measurement by 2^n.
Set 24-bit mode | `.set24bit()` | `.get24bit()` | `bool` | If true, select 24-bit mode, otherwise 16-bit.
Periodic-mode counter | `.setCounter()` | `.getCounter()` | `Catena_Si1133::CounterSelect_t` | Either no counter, or one of three measurement counters.

To define a channel using a `ChannelConfiguration_t` object, call the following method:

```c++
bool configure(uint8_t uChannel, ChannelConfiguration_t config, uMeasurementCount);
```

In the advanced method, `uChannel` and `uMeasurementCount` have the same meaning as in the pre-configured method.

```c++
bool start(bool fOneTime = false);
```

Start a repeated or one-time measurement.

```c++
bool isOneTimeReady();
```

Return `true` if a one-time measurement is complete. All the measurements must have completed.

```c++
uint32_t readChannelData(uint8_t uChannel = 0);
```

Read the value for the specified channel.

```c++
void readMultiChannelData(uint16_t *pChannelData, uint32_t nChannel);
void readMultiChannelData(uint32_t *pChannelData, uint32_t nChannel);
```

Read `nChannel` channels of data, starting from channel 0, into the table at `pChannelData`. If all measurements are 16 bits, then the 16-bit form may be used. If any measurement is 24 bit, then the 32-bit form should be used.

### `cTimer` Timer object

Timer objects are used to simplify the implementation of periodic events.

#### Catena_Timer.h header file and initialization

```c++
#include <Catena_Timer.h>
```

This header file contains all the definitions for the `cTimer` class.

The constructor takes no arguments.  To create a timer named `myTimer`, write:

```c++
McciCatena::cTimer myTimer;
```

#### cTimer begin() and end()

Timers are initially stopped. To start a timer, call:

```c++
bool cTimer::begin(std::uint32_t nMillis);
```

This method initializes the timer to run with a period of `nMillis` milliseconds. The timer automatically restarts each time the period elapses; so it's like a clock that ticks every `nMillis` milliseconds.

To stop a timer, call:

```c++
void cTimer::end();
```

#### Checking for `cTimer` events

To check whether a timer has ticked, call one of the following:

```c++
bool cTimer::isready();

std::uint32_t cTimer::readTicks();

std::uint32_t cTimer::peekTicks() const;

std::uint32_t cTimer::getRemaining() const;
```

`isready()` returns true if the timer has ticked at least once since the last time `isready()` or `readTicks()` was called.  `readTicks()` returns the number of ticks that have occurred since the last time `readTicks()` or `isready()` was called.  Both of these, in effect, reset the tick counter.

`peekTicks()` returns the number of ticks since the last call to `readTicks()` or `isready()`, but doesn't reset the tick counter.

`getRemaining()` returns the number of milliseconds remaining in the current timer cycle.

#### `cTimer` Utility routines

```c++
std::uint32_t cTimer::getInterval() const;

std::uint32_t cTimer::setInterval(std::uint32_t nMillis);

void cTimer::retrigger();
```

`getInterval()` returns the number of milliseconds per timer tick.

`setInterval()` changes the timer period to a new value, but doesn't change the base time of the current period. If the period is lengthened, then the next tick occurs relative to the base time plus the new period. If the period is shortened, ticks will immediately occur to cover any ticks between the base time of the period and now.

`retrigger()` sets the base of the current period to the current time, and resets any pending ticks.

### `Catena_functional.h`

This wrapper allows the C++ `<functional>` header file to be used with Arduino code.

The technical problem is that the `arduino.h` header file defines `min()` and `max()` macros. This causes problems with parsing the `<functional>` header file, at least with GCC.

The solution is a hack: undefine `min()` prior to including `<functional>`, and then redefine them using the well-known definitions.

### `cDownload`

This class may be instantiated as a general-purpose wrapper for supporting the MCCI Trusted Bootloader on STM32L0 platforms. It has two primary entry points. `cDownload::evStartSerialDownload()` starts a download over a serial port, which is assumed to support 8-bit clean data transport. `cDownload::evStart()` starts an abstract download; the client supplies a `cDownload:Request_t` object which includes callbacks for fetching data. An instance of the `cBootloaderApi` is also required.

### `cBootloaderApi`

This class may be instantiated as a singleton to provide the interface to the Trusted Bootloader. It allows access to the APIs exported by the bootloader, as well as to the application structures that accompany the bootloader.

### `cSerial`

This class provides an abstract wrapper for various Arduino Serial-like classes so that a single pointer can be used to refer to any of the possibilities (hardware serial, USB serial, software serial, etc.) It's intended primarily for use by the downloader, so doesn't provide 100% of the `Serial`-like methods.

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
| `system version` | display the board type, and versions of the required libraries. Includes the MCCI Arduino BSP version, if known. |

### STM32L0 commands

| Command | Description |
|-------------|----------------------------------|
| `system calibrate` | calibrate the system clock and print the result.

### FRAM commands

| Command | Description |
|-------------|----------------------------------|
| `fram reset` _[_ `hard` _]_ | reset the contents of the FRAM. A soft reset assumes that the data structures are correct, and resets values to defaults. A hard reset invalidates the FRAM, so that the next boot will fully reconstruct it. |
| `fram dump` _[ base [ len ] ]_ | dump the contents of FRAM, starting at _base_ for _len_ bytes. If _len_ is absent, a length of 32 bytes is assumed. If _base_ is also absent, then 32 bytes are dumped starting at byte zero.

### LoRaWAN commands

The following commands are added by the Catena LoRawAN module.

| Command | Description |
|-------------|----------------------------------|
| `lorawan configure` | Display all LoRaWAN parameters.
| `lorawan configure` _`param` [ `value` ]_  | Display or set a LoRaWAN parameter.
| `lorawan join` | unjoin if joined, then start a new join session.

#### LoRaWAN Parameters

These parameters are generally not loaded into the LMIC immediately. They are primarily used at boot time and at join time.

| Command     | Target device type | Description |
|-------------|---------------------|----------------------------------|
`lorawan configure` | either | Display all the parameters.
`lorawan configure deveui` _[ value ]_  | OTAA | Set the devEUI for this device to _value_, a 64-bit EUI given in big-endian (natural) form.
`lorawan configure appeui` _[ value ]_  | OTAA |Set the AppEUI for this device to _value_, a 64-bit EUI given in big-endian (natural) form.
`lorawan configure appkey` _[ value ]_  | OTAA |Set the application key for this device to _value_, a 128-bit value given in big-endian (natural) form.
`lorawan configure nwkskey` _[ value ]_ | ABP | Set the network session key for this device (the network session key) to _value_.  For OTAA devices, this reflects the value saved after them most recent join.
`lorawan configure appskey` _[ value ]_ | ABP |Set the application session key for this device (the application session key) to  _value_. For OTAA devices, this reflects the value saved after them most recent join.
`lorawan configure devaddr` _[ value ]_ | either | Set the device address, a 32-bit number, in big-endian form. **_Setting `devaddr` to zero on an OTAA device will cause the LMIC to try to rejoin after the next restart._** For OTAA devices, this reflects the value saved after them most recent join.
`lorawan configure netid` _[ value ]    | either | Set the network ID, in big-endian form. For OTAA devices, this reflects the value saved after them most recent join.
`lorawan configure fcntup` _[ value ]_  | either | the current uplink frame count, `FCntUp` in the LoRaWAN spec.
`lorawan configure fcntdown` _[ value ]_ | either | the current downlink frame count, `FCntDown` in the LoRaWAN spec.
`lorawan configure join` _[ value ]_ | either | if zero, the provisioning data will _not_ be loaded into the LMIC at startup. Older versions of the [`arduino-lorawan`](https://github.com/mcci-catena/arduino-lorawan) might still allow transmits to cause the device to start trying to join, but it will use invalid credentials.

## Adding your own commands

Here's a step-by-step procedure. There's a fully worked example, [`catena_usercommand`](#catenausercommand).

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

- Create the top-level structure that represents the command table to the parser. This cannot be `const`, because internal fields are used for linkage (to avoid run-time memory allocation in the library).

```c++
// a top-level structure wraps the above and connects to the system table
// it optionally includes a "first word" so you can for sure avoid name clashes
// with commands defined by the framework.
static cCommandStream::cDispatch sMyCommands(
    sMyCommmandTable,           // this is the pointer to the table
    sizeof(sMyCommmandTable),   // this is the size of the table
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

If the LoRaWAN system is not provisioned, the application enters an idle loop; you can use the LoRaWAN commands to set things up.

### `catena_usercommand`

This sketch is very similar to `catena_hello`. It shows how to add a user-defined command, `application hello`, that prints "`Hello, world!`".

### `catena_fsm`

This sketch demonstrates the use of the Catena FSM class to implement the `Turnstile` example described in [Finite State Machine Framework](#finite-state-machine-fsm-framework).

## Board Support Dependencies

## Other Libraries and Versions Required

| Library | Recommended Version | Minimum Version | Comments |
|---------|:-------:|:----:|----------|
| [`arduino-lmic`](https://github.com/mcci-catena/arduino-lmic) | 4.0.0 | 3.99.0-3 | Earlier versions will fail to compile due to missing `lmic_pinmap::rxtx_rx_polarity` and `lmic_pinmap::spi_freq` fields. |
| [`arduino-lorawan`](https://github.com/mcci-catena/arduino-lorawan) | 0.9.1 | 0.9.1-pre1 | Needed for bug fixes in session state save/restore. |
| [`catena-mcciadk`](https://github.com/mcci-catena/Catena-mcciadk) | 0.2.1 | 0.1.2 | Needed for miscellaneous definitions |

## Release History

- v0.23.0 includes the following changes.
  - fix [#350](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/350): add library support for STM32Lxx watchdog timer (v0.23.0-pre1)

- v0.22.0 includes the following changes.

  - fix [#346](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/346): add support to Catena4618 and Catena 4612 with LTR329 sensor (v0.22.0-pre7)
  - fix [#342](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/342): add support to Catena4610 version-2 (v0.22.0-pre5).
  - fix [#321](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/321): add 24-bit float encoding support (v0.22.0-pre1).
  - fix [#319](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/319): accomodate missing two-arg `UsbSerial::begin()` (v0.21.3-pre1).

- v0.21.2 includes the following changes, non breaking, all bug fixes.

  - Check SessionState for validity when fetching ABP info ([#312](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/312), `v0.21.2-pre1`).
  - Set LMIC clock error for STM32 even when running from HSI clock; we encountered a test board that needed this and conclude that it's generally a good idea ([#313](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/313), `v0.21.2-pre2`).

- v0.21.1 includes the following changes; only #305 is an API extension and it's non-breaking.

  - fix [#304](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/304): invalidate library cache as part of FRAM reset processing (version 0.21.1-2).
  - fix [#305](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/305): make it a little easier to debug FRAM code. 
  - fix [#300](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/300), [#307](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/307): fix build issues for SAMD21 boards. (version 0.21.1-1, 0.21.1-3)

- v0.21.0 includes the following changes.

  - fix [#286](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/286): support enhanced state save/restore for Arduino LoRaWAN library. Requires Arduino LoRaWAN v0.9.0-1 or later.
  - fix [#287](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/287): switch to semantic versioning, which means that bottom 8 bits must be compared using macros, rather than directly.
  - fix [#288](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/288): correct bugs in Si1133 driver.
  - fix [#294](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/294): make Flash driver more `gCatena.poll()` friendly.
  - fix [#293](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/293): don't clobber user's write buffer in `Catena_Mx25v8035f::programPage()`.
  - remove the global `gpCatenaBase` in favor of the static `CatenaBase::pCatenaBase`.
  - add an API to fetch a pointer to the line collector, to allow commands to collect additional input.
  - add firmware download support, and support for the Trusted Bootloader. Firmware download can come from the serial port, or it can come from other (arbitrary) sources.
  - fix [#295](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/295): rearrange log flags for more utility.
  - fix [#296](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/293): add access to AppInfo block and signing info related to running app and the bootloader.

- v0.20.1 includes the following changes.

  - fix [#275](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/275): add `cTotalizer::setDebounce()`, allowing the debounce time to be adjusted from 50ms. This can also be done in the constructor. (Version 0.20.0.30.)
  - fix [#279](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/279): system could sleep forever if the beginning and end of a time range straddled the end of a month. Also fix [#280](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/280), adding ability to set the time (version 0.20.0.20).
  - fix [#273](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/273): in very low light conditions, with 24-bit measurements, Si1133 can generate negative light readings. Detect such values and force to zero (version 0.20.0.10).

- v0.20.0 includes the following changes.

  - Fix [#268](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/268): adds support to the new board Catena 4802 (version 0.19.0.40).
  - Fix [#266](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/266): clear EOC flag after every analog read (version 0.19.0.30).
  - Fix [#261](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/261): add support for low level FRAM driver `Catena_Mb85rc64ta::read()` and `write()` to read/write > 255 bytes (version 0.19.0.30).
  - Fix [#260](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/269): add more control to the Si1133 library (version 0.19.0.20).
  - Fix [#253](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/253): add `AdcStart()` before each `AdcGetValue()` to read channel value (version 0.19.0.10).

- v0.19.0 includes the following changes.

  - [#248](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/248) Add interactive command `lorawan configure` to display all the parameters.

- v0.18.1 includes the following changes.
  - [#247](https://github.com/mcci-catena/Catena-Arduino-Platform/pull/247) Add a generic application block to FRAM map

- v0.18.0 includes the following changes.

  - [#240](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/240) Change to requiring MCCI_FRAM_I2C library.
  - [#241](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/241) List required libraries in `library.properties`.
  - [#238](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/238), [#236](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/237) Add flash-based mfg data (0.17.0.60).
  - [#233](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/233), [#230](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/230) Add input line editing and echo control (0.17.0.50).
  - [#277](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/277) Slow down polling of the Si1133; accumulate interrupt reports.
  - [#228](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/228) add `cDate` class and parser (0.17.0.30).
  - [#222](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/222) Use `Arduino_LoRaWAN_network` consistently as base for `LoRaWAN` classes. Requires Arduino_LoRaWAN 0.6.0.20 or later. (0.17.0.20)
  - [#213](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/213) Vbat voltage read is inaccurate on STM32L0 platform. (0.17.0.12)
  - [#211](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/211) `Catena_Si1133` cannot be copied or moved. (0.17.0.11)
  - [#208](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/208) Add `McciCatena::cTimer` class to simplify timing. (0.17.0.10)
  - [#209](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/209) Fix error in Si1133 driver that caused UV measurements to be wrong. Add an API to query whether a forced measurement is done.

- v0.17.0 includes the following changes

  - [#129](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/129) Improve accuracy of `CatenaSTM32L0::Sleep()` timing.
  - [#197](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/197), [#204](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/204) Add `system version` command.
  - [#199](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/199) extract key-making script to own file and adapt for latest `ttnctl`, update comments. See [mcci-catena-provision](https://github.com/mcci-catena/mcci-catena-provision) for USB-based provisioning, using a variant of the same script.
  - [#202](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/202) Add system clock calibration for STM32L0 platforms, and add `system calibrate` command. Version 0.16.0.50.

- v0.16.0 includes the following changes.

  - [#189](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/189) Add Catena FSM class, example, documentation.
  - [#190](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/190) document the polling framework.
  - [#191](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/191) Document the LoRaWAN framework.
  - [#192](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/192) Document platform GUIDs for 4617, 4618, 4630.
  - [#187](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/187), [#184](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/184), [#183](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/183), [#182](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/182) CI improvements.
  - [#185](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/185) eliminate warnings from `gLog.isenabled()` declaration.
  - Miscellaneous documentation cleanups.

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

  - [#65](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/65) refactors the STM32/SAMD code so that code common for the two ports only appears once.
  - [#117](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/117) adds `Catena::ReadAnalog()` method for STM32 that reads the analog reference and adjust the raw ADC counts.
  - [#119](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/119) corrects an error that prevented provisioning on MCCI Catena 4612 and 4801 boards.

- v0.13.0 incorporates recent bug fixes and enhancements. We added our own implementation of the RTC class (issue [#86](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/86). We updated the UML docs (issue [#111](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/111)). We refactored the STM32 classes (issue [#99](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/99) and [#103](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/103)). A few other minor changes ([754f4b](https://github.com/mcci-catena/Catena-Arduino-Platform/commit/754f4bd9048f12edbd89906daea483eec31a9468) and [71d45d0](https://github.com/mcci-catena/Catena-Arduino-Platform/commit/71d45d0882b00cc0f18458a35a1f6b320e106433)).

- v0.12.0 adds support for the 4610, 4611, 4612, and 4801, and relies on the LMIC 2.3 pre-integrated pin map feature to simplify maintenance and make things more structures. We added more UML diagrams to document the class hierarchy. Now depends on MCCI SAMD BSP 1.2.0 and STM32 2.0.0. A common `Catena::Sleep()` method was added for architecture-neutral low-power standby (issue [#83](https://github.com/mcci-catena/Catena-Arduino-Platform/issues/83)). Added experimental machineQ network support. Various minor bug fixes and enhancements.

- v0.11.0 adds a flash driver for the Catena 4470, adds a flash object for storing BME680 calibration data, and fixes bugs.

- v0.10.0 adds explicit support for the Catena 4470, and fixes minor issues.

- v0.9.0 adds explicit support for the Catena 4460.

- v0.8.1 corrects an issue with the STM32 library (some things were here that belonged in the main Arduino libraries).

- v0.8.0 has some minor changes (add the Catena4551 m101/m102/m103/m104 platform, add the Catena4450 m103/m104), and a flag change which effectively changes the API (hence the bump). We add `CatenaBase::fHasLuxS1113`, which indicates the presence of a SI 1113 Lux sensor (as distinct from the BH1750 or the TSL2561 lux sensor used in the Catena4410). Further, we correct the platform flags for the 4551, as it doesn't have an I2C mux. Also incorporates some minor bug fixes for USB serial.

- v0.7.0 is a major refactoring adding support for the `Catena 4551`, which is based on the STM32L0. Although we think that there are no breaking changes, there might be a few, especially if code relied on structured defined internally to the MCCI-Catena-Arduino library `Catena...` classes.

## Meta

### License

This repository is released under the [MIT](./LICENSE) license. Commercial licenses are also available from MCCI Corporation.

### Support Open Source Hardware and Software

MCCI invests time and resources providing this open source code, please support MCCI and open-source hardware by purchasing products from MCCI, Adafruit and other open-source hardware/software vendors!

For information about MCCI's products, please visit [store.mcci.com](https://store.mcci.com/).

### Trademarks

MCCI and MCCI Catena are registered trademarks of MCCI Corporation. All other marks are the property of their respective owners.
