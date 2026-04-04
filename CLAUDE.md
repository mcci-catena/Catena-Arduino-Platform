# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

MCCI Catena Arduino Platform library -- platform support for MCCI Catena 44xx, 45xx, 46xx, and 48xx IoT systems. Supports SAMD21 (Feather M0-based) and STM32L0 (Murata module-based) architectures. Licensed MIT. Version is tracked in `library.properties`.

## Build and Test

This is an Arduino library. There is no standalone build system. Compilation is done via the Arduino IDE CLI:

```bash
# Verify a sketch for a specific board (SAMD example):
arduino --verify --board mcci:samd:mcci_catena_4450 examples/catena_hello/catena_hello.ino

# Verify for STM32L0:
arduino --verify --board mcci:stm32:mcci_catena_4610 examples/catena_hello/catena_hello.ino
```

Board packages are installed from `https://github.com/mcci-catena/arduino-boards/raw/master/BoardManagerFiles/package_mcci_index.json`.

Required library dependencies (must be installed or cloned alongside): MCCI FRAM I2C, arduino-lmic, arduino-lorawan, Catena-mcciadk, RTCZero, MCCI LTR-329ALS.

CI runs via Travis (.travis.yml) doing compile-only verification across 14+ board variants and 6 LoRaWAN regions. No unit test framework; testing is compilation-based using the example sketches.

## Architecture

### Platform Selection via Preprocessor

`src/Catena.h` is the single entry point (`#include <Catena.h>`). It uses build-time defines (`MCCI_CATENA_4450`, `ARDUINO_MCCI_CATENA_4610`, etc.) to select the correct platform class via `#if`/`#elif` chain, setting `CATENA_H_SUPER_`. The final `Catena` class inherits from the selected platform.

### Class Hierarchy

```
CatenaBase (abstract base -- src/CatenaBase.h)
  CatenaSamd21
    CatenaFeatherM0
      Catena4410, Catena4420
    CatenaWingFram2k
      Catena4450, Catena4460, Catena4470
  CatenaStm32 (src/CatenaStm32.h)
    CatenaStm32L0
      Catena455x -> Catena4551
      Catena461x -> Catena4610, Catena4611, Catena4612, Catena4617, Catena4618
      Catena463x -> Catena4630
      Catena480x -> Catena4801, Catena4802
```

Each platform class provides `CatenaName()`, platform GUID tables, and CPU-ID-to-platform mapping tables.

### Source Layout

- `src/` -- public headers (`.h`) included by sketches
- `src/lib/` -- implementation (`.cpp`) files
- `src/lib/samd/` -- SAMD-specific code, with subdirs per board (e.g., `catena4450/`)
- `src/lib/stm32/stm32l0/` -- STM32L0-specific code, with subdirs per board family
- `examples/` -- example sketches (also serve as CI compile tests)
- `extra/test/` -- additional test sketches

### Key Subsystems

- **Polling engine** (`cPollableInterface`, `cPollableObject`, `cPollingEngine`): cooperative event-driven scheduling. Objects register with the engine and get `poll()` calls.
- **Command stream** (`cCommandStream`, `cStreamLineCollector`): non-blocking serial command processor with extensible command registration.
- **FRAM storage** (`cFram`, `cFramStorage`): persistent configuration in I2C FRAM. Stores platform identity (GUID), EUI64, operating flags.
- **Platform identity**: CPU unique ID maps to platform GUID via per-board tables. `CatenaBase::GetPlatformForID()` walks the table.
- **FSM template** (`cFSM<TParent, TState>`): finite state machine requiring enum states with `stInitial`, `stNoChange`, `stFinal`.
- **Logging** (`cLog`): severity-based log output.
- **Peripheral drivers**: Si1133 light sensor, Mb85rc64ta FRAM, Mx25v8035f flash, AT25SF081 flash.

### Global Access Pattern

`CatenaBase::pCatenaBase` is a static pointer set at `begin()`, providing global access to the framework instance.

## Coding Conventions

- Namespace: `McciCatena` for all library code.
- Platform classes: `Catena` + 4-digit model number.
- Utility/component classes: lowercase `c` prefix (e.g., `cFram`, `cTimer`, `cCommandStream`).
- Driver classes: `Catena_DeviceName` (e.g., `Catena_Si1133`).
- Methods: camelCase, some with uppercase first letter for public APIs (`GetSysEUI()`, `SafePrintf()`).
- Members: `m_` prefix for private/protected member variables.
- Enums: `enum class` with descriptive names; flag members use `f` prefix (e.g., `fHasLoRa`, `fUnattended`).
- Include guards: both `#ifndef _CATENA_MODULENAME_H_` and `#pragma once`.
- File headers: formal block with Module, Function, Copyright, Author fields.
- `using Super = <parent_class>` typedef in each derived class.
- Copyright: MCCI Corporation.
- `dot_a_linkage=true` in library.properties -- the library compiles to a static archive.
