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

///
/// \brief Simple architecture that lets us use serial-like ports interchangably.
///
/// \details
///	The Arduino system omitted a virtual base class that all Serial ports
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
        /// \brief constructor
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
        /// \brief constructor in case caller has a pointer to the underlying port.
        cSerial(T *pPort) : m_pPort(pPort) {}

        /// \brief constructor in case caller has the LV of the underlying port.
        cSerial(T &Port) : m_pPort(&Port) {}

        // neither copyable nor movable.
        cSerial(const cSerial&) = delete;
        cSerial& operator=(const cSerial&) = delete;
        cSerial(const cSerial&&) = delete;
        cSerial& operator=(const cSerial&&) = delete;

        /// \brief return the number of bytes available in the read buffer.
        virtual int available() const override
                {
                return this->m_pPort->available();
                }

        /// \brief initialize the port, setting the baudrate.
        virtual void begin(unsigned long ulBaudRate) const override
                {
                this->m_pPort->begin(ulBaudRate);
                }

private:
        //
        // The following section deals with the polymorphism in underlying serial ports:
        // some have port::begin(baudrate, config), others only have begin(). We use
        // C++ template metaprogramming to allow us to use the advanced APIs if available,
        // otherwise to ignore the config parameter.
        //
        // We follow Walter Brown's method https://youtu.be/a0FliKwcwXE?t=2276 for querying
        // properties at compile time.
        //
        // First, the default `hasBeginWithConfig`. The compiler will choose this template
        // if no template is found that is well-formed and more specific, and this will
        // result in a structure that is a specialiation of std::false_type.
        //
        template <class, class = void>
        struct hasBeginWithConfig
                : /* inherit from  */ std::false_type /* so it has ::value == false */
                { /* no extra contents */ };

        //
        // Second, we define a version that will only be well-formed if the target class
        // has U::begin(unsigned long, uint16_t), and it will be essentially identical to
        // std::true_type.
        //
        template <class U>
        struct hasBeginWithConfig<
                        U,
                        std::void_t<
                                /* packing a list of types, so take the type of... */
                                decltype(
                                        /* check whether we can invoke begin() with our two params */
                                        std::declval<U>().begin(
                                                (unsigned long)0,
                                                (std::uint16_t)0
                                                )
                                        )
                                >
                        >
                : /* inherit from */ std::true_type /* so it has ::value == true */
                { /* no extra contents */ };

        //
        // Now overload beginWithConfig() with two versions, one that calls the 2-arg
        // version of begin(), the other that calls the one-arg version.
        //
        // If we had C++ 2017, we could use if constexpr(), but it's not much worse to have
        // overloads with two functions.
        //
        // This matches if the third argment is of type std::true_type.
        //
        template <class U>
        static void beginWithConfig(U *pT, unsigned long ulBaudRate, uint16_t config, std::true_type)
                {
                pT->begin(ulBaudRate, config);
                }

        //
        // This overload matches if the the third argument is of type std::false_type, and
        // uses the one-argument begin().
        //
        template <typename U>
        static void beginWithConfig(U* pT, unsigned long ulBaudRate, uint16_t config, std::false_type)
                {
                pT->begin(ulBaudRate);
                }

public:
        //
        // finally, the begin() method here uses the hasBeginWithConfig<> template
        // to select the approprialte beginWithConfig() overload.
        //

        ///
        /// \brief initialize port, setting baud rate and configuring.
        ///
        virtual void begin(unsigned long ulBaudRate, uint16_t config) const override
                {
                this->beginWithConfig(
                        this->m_pPort,
                        ulBaudRate,
                        config,
                        hasBeginWithConfig<T> /* create object */ {}
                        );
                }

        ///
        /// \brief wait for the write buffer to be drained to serial port.
        ///
        virtual void drainWrite() const override
                {
                this->m_pPort->flush();
                }

        ///
        /// \brief read one byte from a port.
        ///
        /// \returns next byte (in 0..255) or a negative number if read buffer is empty.
        ///
        virtual int read() const override
                {
                return this->m_pPort->read();
                }

        /// \brief write a buffer, and return the number of bytes actually written.
        virtual size_t write(const uint8_t *buffer, size_t size) const override
                {
                return this->m_pPort->write(buffer, size);
                }

        /// \brief invoke end() method of concrete port.
        virtual void end() const override
                {
                this->m_pPort->end();
                }

        /// \brief return pointer to underlying port viewed as a stream.
	virtual Stream & stream() override
		{
		return *m_pPort;
		}

private:
        T *m_pPort;     ///< pointer to concrete UART.
        };

} // namespace McciCatena

#endif /* _catena_serial_h_ */
