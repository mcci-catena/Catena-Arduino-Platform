# TO-DO List

<!-- TOC depthFrom:2 updateOnSave:true -->

- [Useful info](#useful-info)
- [General changes](#general-changes)
- [Possibly-breaking changes](#possibly-breaking-changes)
- [STM32 changes](#stm32-changes)
- [Commands](#commands)

<!-- /TOC -->

## Useful info

- [x] update `keywords.txt`.  Used `arduino-keywords` with a filter, as follows:

    ```console
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

- [ ] Add example apps

## Possibly-breaking changes

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

8. add reference to [mcci-catena-provision](https://github.com/mcci-catena/mcci-catena-provision)

9. Write a test app that tests the radio at all channels and all drs.

10. add command echo option (so can see what you've done)

11. If things are configured but join isn't set, don't try to join (bug).

12. if appkey is changed, reset nwskey and appskey (and request a join)

13. pull out the older Catena code and support for non-FRAM systems.
