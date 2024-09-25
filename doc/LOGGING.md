# Application Note: Logging Data to FRAM

<!-- markdownlint-disable MD033 -->
<!-- markdownlint-capture -->
<!-- markdownlint-disable -->
<!-- TOC depthfrom:2 updateonsave:true -->

- [Introduction](#introduction)
- [Assumptions](#assumptions)
        - [Message loss scenarios](#message-loss-scenarios)
- [Basic Design](#basic-design)
- [APIs](#apis)
        - [Catena_FramRingBuf.h](#catena_framringbufh)
        - [class McciCatena::FramRingBuffer_t](#class-mccicatenaframringbuffer_t)
        - [class RecoverableUplinkQueue_t<uplinkType>](#class-recoverableuplinkqueue_tuplinktype)
        - [typedef RecoverableUplinkQueue_t<uplinkType>::size_type](#typedef-recoverableuplinkqueue_tuplinktypesize_type)
        - [typedef RecoverableUplinkQueue_t<uplinkType>::sequence_type](#typedef-recoverableuplinkqueue_tuplinktypesequence_type)
        - [RecoverableUplinkQueue_t<uplinkType>::initializeFromFram](#recoverableuplinkqueue_tuplinktypeinitializefromfram)
        - [RecoverableUplinkQueue_t<uplinkType>::size](#recoverableuplinkqueue_tuplinktypesize)
        - [RecoverableUplinkQueue_t<uplinkType>::getBufferSize](#recoverableuplinkqueue_tuplinktypegetbuffersize)
        - [RecoverableUplinkQueue_t<uplinkType>::peek_head](#recoverableuplinkqueue_tuplinktypepeek_head)
        - [RecoverableUplinkQueue_t<uplinkType>::put_tail](#recoverableuplinkqueue_tuplinktypeput_tail)
        - [RecoverableUplinkQueue_t<uplinkType>::newSequenceNumber](#recoverableuplinkqueue_tuplinktypenewsequencenumber)
- [Discussion](#discussion)
        - [What changes must I make to the message protocol between device and cloud?](#what-changes-must-i-make-to-the-message-protocol-between-device-and-cloud)
                - [Should I add a downlink message to reset the buffer?](#should-i-add-a-downlink-message-to-reset-the-buffer)
        - [What changes must I make to an existing device application?](#what-changes-must-i-make-to-an-existing-device-application)
                - [How should my device application find messages with sequence numbers](#how-should-my-device-application-find-messages-with-sequence-numbers)
        - [What changes must I make to my cloud application?](#what-changes-must-i-make-to-my-cloud-application)
        - [What about multiple recoverable message sequences, e.g. on different LoRaWAN ports?](#what-about-multiple-recoverable-message-sequences-eg-on-different-lorawan-ports)
        - [Why not use the uplink counter to detect missing messages?](#why-not-use-the-uplink-counter-to-detect-missing-messages)

<!-- /TOC -->
<!-- markdownlint-restore -->
<!-- Due to a bug in Markdown TOC, the table is formatted incorrectly if tab indentation is set other than 4. Due to another bug, this comment must be *after* the TOC entry. -->

## Introduction

In many situations, users may not be able to accept the inherent lossiness of uplink data transmits. Even if the device sends a confirmed uplink, the confirmation comes from the network server, not from the consuming application. The transmission still might be dropped before it gets to permanent storage in the application.

MCCI Catena devices for LPWAN applications have an FRAM storage module that is able to store small quantities of data without worrying about wear leveling or wear out. In combination with a suitable application design, this can be used to implement a reasonable amount of data retention and playback.

## Assumptions

### Message loss scenarios

We cater to two different scenarios for message loss.

1. **Sporadic message loss due to interference or congestion**. In MCCI's testing, this rate is pretty low and predictable; we observed 3% loss in an application with 100 devices transmitting every 6 minutes and a single gateway.

2. **Bulk message loss due to persistent network problems**. This could be due to a gateway being down, due to lack of gateway connection to the Internet, due to problems with the LoRaWAN network server, or due to problems with the application server. In this case, a large group of consecutive messages may be lost.

In case #2, users will often try to reset the devices and gateways to restore service, and they won't always be able to observe when service has been restored. The application and protocol must be designed to be robust in the face of determined human intervention.

## Basic Design

Two entities cooperate in this design. They are:

1. the firmware in the Catena device that transmits messages, or the "**device application**".
2. the software in the application that receives messages, or the "**cloud application**".

We distinguish between "one-shot" and "recoverable" uplinks. One-shot uplinks are not recorded in FRAM and cannot be re-transmitted via this protocol.  Recoverable uplinks are uniquely identified for recovery, and the application can later request that they be retransmitted (as long as they remain in memory).

The device application maintains a sliding window of recoverable uplinks in the FRAM on the device.

The device application also maintains a sequence counter for reliable uplink messages. This sequence counter is incremented for each message and enough number of bits of the counter are transmitted with each uplink, so that the cloud application can both determine when a message has been dropped, and know the identifier of the message that has been dropped.

The sliding window always advances. When the device is initialized, the window is empty, but as recoverable uplinks are transmitted, they are appended to the window. Once the window becomes full, the oldest message is discarded prior to appending a new message.

When the cloud application determines that a recoverable uplink must be retransmitted, the cloud application sends a downlink to the device application, including the desired sequence number of the recoverable uplink. In response, the device searches the recoverable uplink queue and retransmits the message with the requested sequence number.

Because the sliding window always advances, it is possible that by the time the device receives the request for a given recoverable uplink, the message may have been overwritten. If so, the device responds in a suitable way (as determined by the use case) -- for example, it might transmit a "not available" message, or it might ignore the request.

## APIs

### `Catena_FramRingBuf.h`

This header file defines the APIs described in this section.


### `class McciCatena::FramRingBuffer_t`

This is the concrete class underlying the abstract class template.

### `class RecoverableUplinkQueue_t<uplinkType>`

This class template is used to generate a global that is used for subsequence operations. This must be a singleton, because the corresponding object in the FRAM is also a singleton.

### `typedef RecoverableUplinkQueue_t<uplinkType>::size_type`

This unsigned integral type is used for recording the sizes of objects (and things computed from the same) in the FRAM buffer. It's defined to be `uint16_t`.

### `typedef RecoverableUplinkQueue_t<uplinkType>::sequence_type`

This unsigned integral type is used for sequence numbers. It's defined to be `uint16_t`.

### `RecoverableUplinkQueue_t<uplinkType>::initializeFromFram()`

This method should be called once, during `setup()` processing. The argument `version` is user defined; if not equal to the version number in the FRAM image of the object, then the FRAM image is assumed not to be compatible with this version of the firmware, and is discarded.  (Similarly, if the size of `uplinkType` in the running firmware is different than the size recorded in the FRAM, the previous FRAM image is assumed to be incompatible and is discarded).

### `RecoverableUplinkQueue_t<uplinkType>::size()`

This method returns the current number of entries stored in the queue. It is useful for iterating over the queue.

### `RecoverableUplinkQueue_t<uplinkType>::getBufferSize()`

This method returns the number of bytes allocated in total for storing data.

### `RecoverableUplinkQueue_t<uplinkType>::peek_head()`

This method indexes from the head of the saved queue, and returns the sequence number and the data buffer. The explicit result is `true` if the index is less than `pQueue->size()`, and `false` otherwise.

### `RecoverableUplinkQueue_t<uplinkType>::put_tail()`

This method appends `uplinkType` item to the the tail of the queue with an associated sequence number. It drops the head of the queue if necessary to create room. It returns `true` if all the FRAM operations were successful, `false` otherwise.

### `RecoverableUplinkQueue_t<uplinkType>::newSequenceNumber()`

This method assigns a new sequence number, and updates the FRAM so that the next successful call will return a different sequence number, even if the system reboots in between.

The method returns `true` if all FRAM operations success, `false` otherwise. If `false`, the sequence number out parameter is not changed.

## Discussion

### What changes must I make to the message protocol between device and cloud?

1. You must draw a distinction between "one-shot" and "recoverable" uplinks.

2. You must change the over-the-air data structure of a "recoverable uplink" as compared to one-shot uplinks: recoverable uplinks must include a new field for the sequence number, and one-shot uplinks must not contain that field. There are many ways to do this. If using the MCCI scheme of a bit map to indicate presence/absence of optional fields, you could define a new field (with its bit) for the sequence number. For simplicity, we recommend use of a 16-bit sequence number in the message over the air.

   As always, you must bear in mind the US 11-byte limitation for SF10 messages.

3. You must add one or more downlinks to allow the cloud application to request retransmission of stored recoverable uplinks. The recommended change is a message that carries 4 bytes of payload, a starting sequence number and an ending sequence number. If the ending sequence number is less than the starting sequence number, then the device should retransmit all messages with sequence numbers in the ranges [start..`0xFFFF`] and [0..end].

4. You must define the retransmit format to be used when re-transmitting recoverable uplinks. This format must include enough information for the cloud application to reconstruct the original *time* of the recovered uplink, so it can put it in the database at the appropriate time. It's up to you how to do this; you could record a timestamp as part of the recoverable uplink, or you could simply use the known update period to approximate the original time of the uplink. This must not be distinguishable from a "first time" transmit. One way to do this is to use a different LoRaWAN port for re-transmissions.

#### Should I add a downlink message to reset the buffer?

You should not ordinarily reset the recoverable uplink buffer. It is therefore creating a dangerous temptation to add such a message to the protocol. However, because of bugs, it might be necessary to reset the buffer via a downlink; so you must use your judgment as to whether to implement this.

### What changes must I make to an existing device application?

Here's a list that's not necessarily exhaustive.

1. Create a data structure for your uplink messages that's either fixed in size, or self-sizing with a fixed maximum size. Note that the design here requires that the recoverable uplinks stored in FRAM have a fixed size. So the usual message with optional fields used over the air *is not* used for FRAM storage. (You may still use variable length over the air; but if so, you must convert the fixed-length storage to variable at the appropriate time.) For the purposes of discussion, we'll use the name `UserData_t` for this data type, but you should use a more informative name in your application.

2. Define a global variable of type `RecoverableUplinkQueue_t<UserData_t>`. For the purposes of this discussion, we'll call it `gRecoverableUplinks`.

3. During `setup()`, call `gRecoverableUplinks.initialize()` to set things up.

4. When you want to generate a recoverable uplink sequence number, call `gRecoverableUplinks.issueSequenceNumber()` to generate it.

5. When you send a recoverable uplink, include a unique sequence number in the message, and then create a `UserData_t` object from the uplinked data. Call `gRecoverableUplinks.putTail(sequenceNumber, userData)` to record the data.

6. **Make sure your application doesn't send recoverable uplinks too often.**  We often send a flurry of uplink messages after system boot, to assist with commissioning systems. *Do not make these messages recoverable.*  Otherwise if the user gets into a commissioning loop because of other problems, the commissioning uplinks will overwrite valuable older data. You must rate limit the creation of recoverable uplinks at all times and in all circumstances.

    Best practice is not to record a recoverable uplink until after you've completed a service cycle. I.e., if the device sends messages every 8 hours, don't send a recoverable uplink until the first eight hours have passed.

7. **Consider adding a polling cycle after receiving a downlink**. The MCCI Model 4811 automatically starts sending Class A poll messages every so often whenever it receives a command downlink from the cloud application. This makes the 4811 much more responsive to the cloud app. Otherwise on devices with 8 hour uplink cycles, you need to wait 8 hours for each control downlink to be transmitted.

8. **Consider transmitting the size of the ring buffer (in messages) at start up**.  This allows the cloud to auto-configure its recovery algorithm.

9. **Consider transmitting the current sequence number at start up**. This allows the cloud to get resynchronized with the device, if needed.

10. **You must define regression tests**. This is complicated, and so you must define suitable test sequences so you can confirm that the device on its own is working correctly.

#### How should my device application find messages with sequence numbers

If asked to retransmit one or more messages, your code should do something like this:

```c++
extern RecoverableUplinkQueue_t<uplinkType> gQueue;

bool retransmit_matching_uplink(sequence_type start, sequence_type end)
    {
    uplinkType item;
    sequence_type seqno;
    const bool no_wraparound = start <= end;

    for (size_type i = 0; gQueue.peek_head(seqno, item, i); ++i)
	{
	if (no_wraparound && start <= seqno && seqno <= end)
	    do_retransmit(item);
	else if (!noWraparound && (start <= seqno || seqno <= end))
	    do_retransmit(item);
	else
	    /* skip */;
	}
    }
```

In other words, you iterate over all items in the queue, from oldest to newest, and consider the sequence number of each.

- If the range of numbers given is normal (`start` <= `end`), then we treat this as a normal range, and we transmit any item that is in the range `start` <= item's sequence number <= `end`.

- Otherwise, the range given is assumed to include the wrap-around due to integer overflow. For example if `start` is `0xFFFE` and `end` is `1`, this is interpreted as a request to transmit any message with sequence number `0xFFFE`, `0xFFFF`, `0`, or `1`. Note that in this case, `start` will be greater then `end`, and we can use `(start <= seqno || seqno <= end)` to match all the desired sequence numbers.

### What changes must I make to my cloud application?

1. You must define a mechanism to discover missing uplinks.

2. You must define a policy for fetching missing uplinks. You don't want to do this too often, but you must detect and request both single messages and blocks of missing messages. If you request a missing message and the device tells you that the message is not available, don't request it again.

3. You must define how to set the timestamp when storing retransmitted uplinks, so that data is stored properly in the database.

4. You must define suitable regression tests, so you can inject various test data sequences into the could application and confirm that it requests the appropriate recoverable uplinks from the device, injects them with the appropriate timestamp into the database, and deals properly with lost uplinks in response to recoverable uplink requests.

### What about multiple recoverable message sequences, e.g. on different LoRaWAN ports?

This is a good idea, but is not supported now, because of the limitations in the FRAM object implementation.

### Why not use the uplink counter to detect missing messages?

The LoRaWAN protocol includes an uplink counter that is used for message deduplication. Unfortunately, this counts messages at the mac layer, not at the application layer; and the mac layer is owned by the network, not by the application. The network can send messages that require responses; the receiving application will not be aware of these messages and so can't easily determine whether an application message was dropped or not.
