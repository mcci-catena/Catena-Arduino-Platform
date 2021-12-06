/*

Module:	Version_t_toBuffer.cpp

Function:
    McciCatena::Version_t::toBuffer()

Copyright and License:
    This file copyright (C) 2021 by

        MCCI Corporation
        3520 Krums Corners Road
        Ithaca, NY  14850

    See accompanying LICENSE file for copyright and license information.

Author:
    Terry Moore, MCCI Corporation	December 2021

*/

#include <CatenaBase.h>

size_t
McciCatena::Version_t::toBuffer(char *pBuffer, size_t nBuffer) const
    {
    if (pBuffer == nullptr || nBuffer == 0)
        return 0;

    if (nBuffer < kVersionBufferSize)
        {
        pBuffer[0] = '\0';
        return 0;
        }

    // local function to put an 8-bit field
    size_t i = 0;
    auto const putbyte = [pBuffer, nBuffer, &i](char c) -> void
        {
        if (i < nBuffer)
            pBuffer[i] = c;
        ++i;
        };
    auto const putfield = [&putbyte](uint8_t v) -> void
        {
        auto temp = v;
        if (v >= 100)
            {
            putbyte('0' + temp / 100);
            temp %= 100;
            }
        if (v >= 10)
            {
            putbyte('0' + temp / 10);
            temp %= 10;
            }
        putbyte('0' + temp);
        };

    putfield(this->getMajor());
    putbyte('.');
    putfield(this->getMinor());
    putbyte('.');
    putfield(this->getPatch());
    if (this->isPrerelease())
        {
        putbyte('-');
        putbyte('p');
        putbyte('r');
        putbyte('e');
        putfield(this->getPrerelease());
        }
    putbyte('\0');

    // if we might have overflowed the buffer, set the terminator.
    if (i > nBuffer - 1)
        pBuffer[nBuffer-1] = '\0';

    return i;
    }
