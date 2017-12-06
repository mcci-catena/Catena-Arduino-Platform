# TO-DO List

## Merging with Catena4551 library

- [x] move CatenaSamd21::PLATFORM_FLAGS to CatenaBase.

- [x] move the Stm32 flags to CatenaBase.

- [x] expand tabs (use blanks only for indentation).

- [ ] find the flag definitions that are based on CatenaSamd21:: and rename to CatenaBase::

- [ ] move McciCatena::CATENA_CPUID_TO_PLATFORM to `McciCatena::CatenaSamd21::CATENA_CPUID_TO_PLATFORM` and rename it to something like `CPUID_PLATFORM_MAP`.

- [ ] move `McciCatena::CATENA_PLATFORM` to `CatenaBase.h`. Perhaps it should be `CatenaBase::PLATFORM` instead of a standalone name.

- [ ] move `CatenaSamd21::PlatformFlags_GetModNumber()` to `CatenaBase::PlatformFlags_GetModNumber()`, ditto `PlatformFlags_GetModNumber()` and other method functions.

- [ ] rebuild.

- [ ] change 'flags' to be method functions; we want to portably query, but we may need to store different capabilities based on the platform.

### STM32 changes

- [ ] Change the STM32 UniqueID to match CPU (96-bit). The type is defined in a hw-specific header file, so should be OK.

## Commands

1. Add "system configure" and "lorawan configure" variants that display 
all the settable paramters.

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
