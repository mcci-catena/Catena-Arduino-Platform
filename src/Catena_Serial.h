/*

Module:	Catena_Serial.h

Function:
    McciCatena::cSerial

Copyright and License:
    This file copyright (C) 2021 by

        MCCI Corporation
        3520 Krums Corners Road
        Ithaca, NY  14850

    See accompanying LICENSE file for copyright and license information.

Author:
    Terry Moore, MCCI Corporation	April 2021

*/

#pragma once

#ifndef _catena_serial_h_
#define _catena_serial_h_

#include <Catena.h>

namespace McciCatena {

/// \brief Simple architecture that lets us use serial-like ports interchangably.
///
/// \details
///	The Arduino system omittted a virutal base class that all Serial ports
///	can be derived from. This means you can't have a pointer to a "serial-like"
///	thing, which is very inconvenient. Even if we put one in our Arduino
///	variant, it would not help with third-party libraries.
///
///	We need one, however, so we synthesize one, consisting of a base virtual
///	class \c cSerialAbstract accompanied by concrete derived template classes that
///	wrap the actual object and give it virtual methods.
///
/// \note
///	This implementation doesn't provide all stream methods.
///
class cSerialAbstract
	{
public:
        // constructor
        cSerialAbstract() {};

        // neither copyable nor movable.
        cSerialAbstract(const cSerialAbstract&) = delete;
        cSerialAbstract& operator=(const cSerialAbstract&) = delete;
        cSerialAbstract(const cSerialAbstract&&) = delete;
        cSerialAbstract& operator=(const cSerialAbstract&&) = delete;

        virtual void begin(unsigned long baudrate) const = 0;
        virtual void begin(unsigned long baudrate, uint16_t config) const = 0;
        virtual int available() const = 0;
        virtual int read() const = 0;
        virtual size_t write(const uint8_t *buffer, size_t size) const = 0;
        size_t write(const char *buffer, size_t size) const
		{
		return this->write((const uint8_t *)buffer, size);
		}
        virtual void end() const = 0;

        virtual void drainRead() const
                {
                while (this->read() >= 0)
                        /* discard */;
                }
        virtual void drainWrite() const = 0;

        // provide this as a synonym for drainWrite, so that we
        // have the complete Arduino::Uart interface.
        virtual void flush() const
                {
                this->drainWrite();
                }

	// get the underlying stream
	virtual Stream & stream(void) = 0;
        };

///
/// \brief Instantiate a \c cSerial variant for a given Serial-like type
///
/// \param T is class like \ref UART that has UART semantics.
///
/// \details
///	A typical use might be `cSerial<decltype(Serial)> gSerial(Serial);
///	this would declare a \ref cSerial object that is a wrapper for
///	\ref Serial without requiring that you know the type of `Serial`.
///	This is convenient on platforms where Serial changes type based on
///	whether it's hardware, software, USB, etc.
///
template <class T>
class cSerial : public cSerialAbstract
	{
public:
        // constructor
        cSerial(T *pPort) : m_pPort(pPort) {}
        cSerial(T &Port) : m_pPort(&Port) {}

        // neither copyable nor movable.
        cSerial(const cSerial&) = delete;
        cSerial& operator=(const cSerial&) = delete;
        cSerial(const cSerial&&) = delete;
        cSerial& operator=(const cSerial&&) = delete;

        virtual int available() const override
                {
                return this->m_pPort->available();
                }

        virtual void begin(unsigned long ulBaudRate) const override
                {
                this->m_pPort->begin(ulBaudRate);
                }

        virtual void begin(unsigned long ulBaudRate, uint16_t config) const override
                {
                this->m_pPort->begin(ulBaudRate, config);
                }

        virtual void drainWrite() const override
                {
                this->m_pPort->flush();
                }

        virtual int read() const override
                {
                return this->m_pPort->read();
                }

        virtual size_t write(const uint8_t *buffer, size_t size) const override
                {
                return this->m_pPort->write(buffer, size);
                }

        virtual void end() const override
                {
                this->m_pPort->end();
                }

	virtual Stream & stream() override
		{
		return *m_pPort;
		}

private:
        T *m_pPort;
        };

} // namespace McciCatena

#endif /* _catena_serial_h_ */
