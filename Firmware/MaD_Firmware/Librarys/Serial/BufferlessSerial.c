#include "BufferlessSerial.h"

/**
 * @brief UART Serial implementation without cog or buffer. Used SPI functions from propware library to build inline snippits.
 * 
 */

#define invert_rx 1
#define invert_tx 2

#define FC_START(start, end)                    \
    "        fcache #(" end " - " start ")\n\t" \
    "        .compress off\n\t" start ":\n\t"
#define FC_END(end)                         \
    "        jmp __LMM_RET\n\t" end ":\n\t" \
    "        .compress default\n\t"
#define FC_ADDR(to, start) "__LMM_FCACHE_START+(" to " - " start ")"

/**
 * @brief 
 * @TODO Add timeout to how long we wait for data
 * @param serial 
 * @return char 
 */
char uart_read(BLSerial *serial)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
    unsigned int bits = 8;
    unsigned int clock;
    unsigned int tempData;
    __asm__ volatile(
        FC_START("readStart%=", "readEnd%=") "       ror %[_data], %[_bitCount]              '' move MSB into bit 31                 \n\t"
                                             "       mov %[_clock], %[_clkDelay]                                                     \n\t"
                                             "       add %[_clock], CNT                                                              \n\t"
                                             "       test %[_rx], ina wc                                                             \n\t"
                                             "       waitcnt %[_clock], %[_clkDelay]                                                 \n\t"
                                             "loop%=:                                                                                \n\t"
                                             "       rcr %[_data], #1                                                                \n\t"
                                             "       waitcnt %[_clock], %[_clkDelay]                                                 \n\t"
                                             "       djnz %[_bitCount], #" FC_ADDR("loop%=", "readStart%=") "              \n\t" FC_END("readEnd%=")
        : [_bitCount] "+r"(bits),
          [_clock] "+r"(clock),
          [_data] "+r"(tempData)
        : [_rx] "r"(serial->rx_mask),
          [_clkDelay] "r"(serial->baudCNT));
#pragma GCC diagnostic pop
    return tempData;
}

/**
 * @brief Uart write command lsb first with tx inverted
 * @TODO add non inverted tx mode
 * @param serial Bufferless Serial Object to write to
 * @param p_data The data to write
 */
void uart_write(BLSerial *serial, uint8_t p_data)
{
    unsigned int bits = 8;
    unsigned int data = ~p_data;
    unsigned int clock;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
    __asm__ volatile(
        FC_START("writeStart%=", "writeEnd%=") "       mov %[_clock], CNT                                                \n\t"
                                               "       add %[_clock], %[_clkDelay]                                                          \n\t"
                                               "        xor OUTA,%[_tx]                                                                    \n\t"
                                               "       waitcnt %[_clock], %[_clkDelay]                                             \n\t"
                                               "loop%=:                                                                            \n\t"
                                               "       ror %[_data], #1 wc                                                         \n\t"
                                               "       muxc outa, %[_tx]                                                         \n\t"
                                               "       waitcnt %[_clock], %[_clkDelay]                                             \n\t"
                                               "       djnz %[_bitCount], #" FC_ADDR("loop%=", "writeStart%=") "                   \n\t"
                                                                                                               "" FC_END("writeEnd%=")
        : [_bitCount] "+r"(bits),
          [_data] "+r"(data),
          [_clock] "+r"(clock)
        : [_tx] "r"(serial->tx_mask),
          [_clkDelay] "r"(serial->baudCNT));
#pragma GCC diagnostic pop
    OUTA &= ~(serial->tx_mask);
}

void uart_start(BLSerial *serial, int rx, int tx, int mode, int baud)
{
    serial->mode = mode;
    serial->tx_mask = (1 << tx);
    serial->rx_mask = (1 << rx);
    serial->baudCNT = CLKFREQ / baud; //number of clk cycles between bytes
    DIRA &= ~(serial->rx_mask);       //setting rx as input
    DIRA |= serial->tx_mask;          //setting tx as output
    if ((mode & invert_tx) == invert_tx)
    {
        OUTA &= ~(serial->tx_mask); //setting tx to low
    }
    else
    {
        OUTA |= serial->tx_mask; //setting tx to high
    }
}