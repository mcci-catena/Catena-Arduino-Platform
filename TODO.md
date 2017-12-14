# TO-DO List

<!-- TOC depthFrom:2 updateOnSave:true -->

- [Merging with Catena4551 library](#merging-with-catena4551-library)
- [General changes](#general-changes)
- [Possibly-breaking changes](#possibly-breaking-changes)
- [STM32 changes](#stm32-changes)
- [Commands](#commands)

<!-- /TOC -->

## Merging with Catena4551 library

- [x] move CatenaSamd21::PLATFORM_FLAGS to CatenaBase.

- [x] move the Stm32 flags to CatenaBase.

- [x] expand tabs (use blanks only for indentation).

- [x] find the flag definitions that are based on CatenaSamd21:: and rename to CatenaBase::

- [x] move `McciCatena::CATENA_PLATFORM` to `CatenaBase.h`.

- [x] Change Catena_Platforms.h to use CatenaBase.h.

- [x] Add 455x GUIDs to main library Catena_Guids.h

- [x] Put CatenaBase and other Catena* into McciCatena namespace.

- [x] Add to main library:

  - [x] `Catena_Mb85rc64ta.h`

  - [x] `Catena_Fram8k.h`

  - [x] `Catena_Mx25v8035f.h`

  - [x] all the Catena 4451 code.

- [x] move McciCatena::CATENA_CPUID_TO_PLATFORM to `McciCatena::CatenaSamd21::CATENA_CPUID_TO_PLATFORM` and rename it to something like `CPUID_PLATFORM_MAP`.

  - [x] similar move for stm32.

- [x] Move `CatenaSamd21::m_pPlatform` and CatenaStm32 ditto to CatenaBase. Refactor initialization so that m_pPlatform is initialized by CatenaBase.

- [x] move `CatenaSamd21::PlatformFlags_GetModNumber()` to `CatenaBase::PlatformFlags_GetModNumber()`, ditto `PlatformFlags_GetModNumber()` and other method functions.

    - [x] Remove cloned functions in STM32

- [x] update `keywords.txt`.  Used `arduino-keywords` with a filter, as follows:
    ```shell
    $ pip install arduinokeywords
    $ arduino-keywords -o /c/tmp src
    $ cat /c/Users/tmm/Documents/Arduino/libraries/MCCI-Catena4410-Arduino-Library/keywords.txt /c/tmp/keywords.txt |\
        sed -e 's/[ \t][ \t]*/\t/g' \
            -e /::/d -e '/^operator=/d' |\
        LC_ALL=C sort >/c/tmp/keywords2.txt -k 1,2 -u
    ```

## General changes

- [ ] Eliminate proprietary notices. (This can be done on an on-going basis, but we ought to do a major pass.)

- [ ] Expand all tabs.

- [ ] Rename `McciCatena::CATENA_PLATFORM` to `McciCatena::CatenaBase::PLATFORM`?

- [ ] Merge remaining 455x material into main library.

- [ ] Add example apps

- [ ] Use -D MCCI_... symbols from boards.txt, and a "ThisCatena.h" (or just "Catena.h") to get a ThisCatena (or Catena) class in scope. So examples can be cross platform.

## Possibly-breaking changes

- [ ] Renaming the library to McciCatena instead of Catena4410.

- [ ] change 'flags' to be method functions; we want to portably query, but we may need to store different capabilities based on the platform.

## STM32 changes

- [ ] Change the STM32 UniqueID to match CPU (96-bit). The type is defined in a hw-specific header file, so should be OK.

## Commands

1. Add "system configure" and "lorawan configure" variants that display all the settable paramters.

2. Extend help to show the settable paramters

3. Add "lorawan reset" which gives us a way to clear out most of the settings

4. Add "system configure reset" which keeps the platform guid, syseui, and operating flags but resets everything else.

5. Add "lorawan configure DataRate n" to set the data rate

6. Change lmic to allow transmit power to be set manually; and add "lorawan configure txpower #" to set the dB.

7. Allow enabling of ADR (and test adr)

8. Add a local command to set the update rate, and (like Lancaster) to make the update rate fast for a little while

9. change the mote-provisioning tool to support configuring Catenas, and to support registering the catena

10. Write a test app that tests the radio at all channels and all drs.

11. reorganize code in sensor1 app

12. add echo option (so can see what you've done)

9. change the mote-provisioning tool to support configuring Catenas, and to support registering the catena

10. Write a test app that tests the radio at all channels and all drs.

11. reorganize code in sensor1 app

12. add echo option (so can see what you've done)

9. change the mote-provisioning tool to support configuring Catenas, and to support registering the catena

10. Write a test app that tests the radio at all channels and all drs.

11. reorganize code in sensor1 app

12. add echo option (so can see what you've done)

9. change the mote-provisioning tool to support configuring Catenas, and to support registering the catena

10. Write a test app that tests the radio at all channels and all drs.

11. reorganize code in sensor1 app

12. add echo option (so can see what you've done)

9. change the mote-provisioning tool to support configuring Catenas, and to support registering the catena

10. Write a test app that tests the radio at all channels and all drs.

11. reorganize code in sensor1 app

12. add echo option (so can see what you've done)

9. change the mote-provisioning tool to support configuring Catenas, and to support registering the catena

10. Write a test app that tests the radio at all channels and all drs.

11. reorganize code in sensor1 app

12. add echo option (so can see what you've done)

9. change the mote-provisioning tool to support configuring Catenas, and to support registering the catena

10. Write a test app that tests the radio at all channels and all drs.

11. reorganize code in sensor1 app

12. add echo option (so can see what you've done)

9. change the mote-provisioning tool to support configuring Catenas, and to support registering the catena

10. Write a test app that tests the radio at all channels and all drs.

11. reorganize code in sensor1 app

12. add echo option (so can see what you've done)

9. change the mote-provisioning tool to support configuring Catenas, and to support registering the catena

10. Write a test app that tests the radio at all channels and all drs.

11. reorganize code in sensor1 app

12. add echo option (so can see what you've done)

13. If things are configured but join isn't set, don't try to join (bug).

14. if appkey is changed, reset nwskey and appskey (and request a join)

15. Add example sketches.

16. pull out the older Catena code and support for non-FRAM systems.
