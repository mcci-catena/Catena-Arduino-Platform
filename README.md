# The McciCatena Library

This library provides a simple-to-use framework for taking advantage of many of the features of the MCCI Catena Arduino products.

_Apologies_: This document is a work in progress, and is published in this intermediate form in hopes that it will still be better than nothing.

<!-- TOC depthFrom:2 updateOnSave:true -->

- [Overview](#overview)
- [Coding Practices](#coding-practices)
- [Components](#components)
    - [Class `Catena` and header file `Catena.h`](#class-catena-and-header-file-catenah)
    - [Platform Management](#platform-management)
    - [Pollable Interface](#pollable-interface)
    - [LoRaWAN Support](#lorawan-support)
    - [FRAM Storage Management](#fram-storage-management)
        - [FRAM Storage Formats](#fram-storage-formats)
            - [Object Storage Structure](#object-storage-structure)
            - [Bit layout of `uSizeKey`](#bit-layout-of-usizekey)
            - [The FRAM header object](#the-fram-header-object)
        - [Class hierarchy within the FRAM library](#class-hierarchy-within-the-fram-library)
    - [Asynchronous Serial Port Command Processing](#asynchronous-serial-port-command-processing)
    - [`Catena_functional.h`](#catena_functionalh)
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

### Class `Catena` and header file `Catena.h`

`Catena.h` is the main header file for the library. It uses the `#defines` injected by `board.txt` and `platform.txt` from the Arduino environment to create a class named `Catena` derived from the `Catena...` class that is specific to the board for which the software is being built. This allows examples to be source-compatible, no matter which Catena is our target.

### Platform Management

### Pollable Interface

### LoRaWAN Support

### FRAM Storage Management

Many MCCI Catena models include FRAM storage for keeping data across power cycles without worrying about the limited write-tolerance of EEPROM or flash. (FRAM, or ferro-electric RAM, is essentially non-volatile memory that can be freely written. Flash EPROM and EEPROM can be written, but tend to have non-local error properties and limited write durability. They are good for storing code, but troublesome for storing counters, because a location must be updated each time a counter is written.)

The abstract class `cFram` is used to reprent a FRAM-based storage element. It is abstract in that is uses several virtual methods that must be supplied by the concrete class that represents the specific FRAM chip. (For example, `cFram2K` represents a 2k by 8 FRAM.)

#### FRAM Storage Formats

All FRAMs managed by `cFram` use a common object format on the FRAM, defined by the header file `Catena_FramStorage.h`.

- Storage is viewed as a linear sequence of objects.

- Each object uses a common format.

- Each object consists of a common 24-byte header followed by a variable-length storage field.

- Objects are always a multiple of 4 bytes long.

- Objects are identified by "globally unique ID" (or GUID) and "key" (an 8-bit value). In many applications, most objects share a common GUID, and this approach allows for more space-efficient code on systems with limited memory.

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

#### Class hierarchy within the FRAM library

<!-- The following image renders well on github, but doesn't render in the previewer in vscode unless you turn on "insecure content". Of course, github rendering is more important, but this is irksome -->
![Image of cFram -- see assets/cfram.puml](http://www.plantuml.com/plantuml/png/ZLLXRzem4FsUNs5fqihAK3KcJHDKKoiw9CJGAjZwPPgYmpRKKcmZsw5YZF--ivCGGaEoBpHzztn-xt4Nxgm3urAPH3TNv341vNxElX4XOSt9IXiodj_FegkRI87aTWEKancbOWIEAo3x29RM6Q2Eq0Ii9VIH6oii5jt2QUERx2D2klk2To1RWTT1GfPQumTV2zLvDspY22MSW4JyuIr4mortRSti4xPYhQmv1xQxpDfgmL3OrlV2A3pn44Krxc__zdg4ZWt8YBmAoyDPFXT3QKxYelq6pmr66znLOx0e8NPu8zGaQITPzSM8khF1THxFOF1zjKBLPCsTBMq5QS3OI4i9YjcY2Owg4_H0StpXK80S38x_WaijOVeXbPks8zKJyzFel7aBMpuMF2diEIut2KP1Rjrkm_qka1zVI2qrY4S_xSThc6y29kDL-5eyK1kNZ7Lu605zvhz-UrugbFiS-rBwn4rU63Pqu8fbpu4Kh2GQM3YPYmbW6AjNQ85zLSxaurD_4II9fiSXolo5oPXWA0_WeDYTSYNwKxyKtRDTw9uFNG6KSzPjdQtUFOELIOMB_NijdlH7cCJpIn0Gjh7Sr_wq8Wfd9ZKMNuX7QHIL51VdoxJKgM8L93D_YkLnLANY92e6XlbG_fCwvJjRKj4afY0hmUJOqVoHsIc6vVfzligLigz-x7UoFyXwMrOtgUjGhQmZ3IsGrDG2ZENuQabvHIp-raXm9Twx793NE2dCPZnoBUEDV_28l_N3fsmN5jYzmFBrmE3E45oO0WbgY_KQdin5hpe6MBd33SBVpNy0)

<!-- The following image renders well on github, but doesn't render in the previewer in vscode unless you turn on "insecure content". Of course, github rendering is more important, but this is irksome -->
![Image of FRAM Storage objects -- see assets/cframstorage.puml](http://www.plantuml.com/plantuml/png/ZLJRJXin47tVhx3sXPjMY29LA5GLRI8GQjDKYU05eMhYdGrkrjx85mga_7jjxoKsMQA-9CgSCpDdpZXpeJH8RNYP7E-HdoK8VJF-JXY71PAYakHo-cMSx1qd0ZYg5L0aOqhP43GA8EiaeIKeHUYb13xJbNHfwuIG8nI6ava6aOFCl-6pSa7zlC8S2sAOqASVVcaoAGg5-jnXQoj4C_fAAw5qbtcjaPD7zpd2GjtTEshosjTsC8Xod8YiwyDXYRTG6jm_yCqwO85wnbxQSAZNGVQxGUPLLHAcVXW-Hnd7fhWg6RM6v4sydk3gvdf43csHAyCQPAF7U37dxqqocVz6ggVmf_Poa-QqMb4kwEkxsYtZp14QiFjzY_Bkz3vGsuwuV-2bzhJrebjQ7RBadKetCay-rgRMyThQ21Fur83rxeE5nBb3Q-fIXLQhk6oSzm3AUihoFipM78660FBrE6ZwW6gpdoER4A-zVPI1VFk0ysrjZwvtNvEUzs64OWk1EH4D9BHw-ouhRywaw1W1uQmUQMYan2r0lGUuhOwOef9n9i3kxwifYkujAokxSMgR4ypKy5fv4Nchr5Psg2LgNribc6RMnxS6-hswa7k3uQ59Jr9oc6MVIReSmrEwwnmCmYEo-tpuOnKtyat-bnt-VJ_zpJAIzg5fajXvBb3azlR-0m00)

### Asynchronous Serial Port Command Processing

### `Catena_functional.h`

This wrapper allows the C++ `<functional>` header file to be used with Arduino code.

The technical problem is that the `arduino.h` header file defines `min()` and `max()` macros. This causes problems with parsing the `<functional>` header file, at least with GCC.

The solution is a hack: undefine `min()` prior to including `<functional>`, and then redefine them using the well-known definitions.

## Board Support Dependencies

## Other Libraries and Versions Required

| Library | Version | Comments |
|---------|:-------:|----------|
| [arduino-lmic](https://github.com/mcci-catena/arduino-lmic) | 2.1.0 | Earlier versions will fail to compile due to missing `lmic_pinmap::rxtx_rx_polarity` and `lmic_pinmap::spi_freq` fields. |
| [arduino-lorawan](https://github.com/mcci-catena/arduino-lorawan) | 0.3.1 | Needed in order to support the Murata module used in the Catena 4551 |
| [catena-mcciadk](https://github.com/mcci-catena/Catena-mcciadk) | 0.1.2 | Needed for miscellaneous definitions |

## Library Release History

- V0.7.0 is a major refactoring adding support for the `Catena 4551`, which is based on the STM32L0. Although we think that there are no breaking changes, there might be a few, especially if code relied on structured defined internally to the MCCI-Catena-Arduino library `Catena...` classes.
