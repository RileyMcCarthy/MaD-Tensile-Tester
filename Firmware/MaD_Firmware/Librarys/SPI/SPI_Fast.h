/**
 * @file        PropWare/serial/spi/spi.h
 *
 * @author      David Zemon, Modified by Riley McCarthy
 *
 * @copyright
 * The MIT License (MIT)<br>
 * <br>Copyright (c) 2013 David Zemon<br>
 * <br>Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:<br>
 * <br>The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.<br>
 * <br>THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*Propware SPI library shift out/in functions isolated and simplified for a specific use*/
#ifndef SPIFAST_H
#define SPIFAST_H
#include "propeller.h"
#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct spi_st
    {
        int mosi_mask;
        int miso_mask;
        int sclk_mask;
        int frequency;
    } SPI_Bus;

    void shift_out_fast(SPI_Bus *bus, uint32_t bits, uint32_t data);
    uint32_t shift_in_fast(SPI_Bus *bus, unsigned int bits);
    void spi_open(SPI_Bus *bus, int mosi, int miso, int sclk);
#ifdef __cplusplus
}
#endif
#endif