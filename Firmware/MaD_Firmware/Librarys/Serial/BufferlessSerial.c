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
void uart_read(BLSerial *serial, const unsigned int bytes, uint8_t *buffer)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
    unsigned int bits = 8;
    unsigned int clock;
    unsigned int clockDelayhalf = serial->baudCNT / 4;
    unsigned int tempData = 0;
    unsigned int finalrot = 24;

    for (int i = 0; i < bytes; i++)
    {
        bits = 8;
        __asm__ volatile(
            FC_START("ReadStart%=", "ReadEnd%=") //Start of read
            "       ror %[_data], %[_bitCount]              '' move MSB into bit 8                 \n\t"
            "       waitpeq %[_rx],%[_rx]                                                             \n\t"
            "       mov %[_clock], CNT                                                     \n\t"
            "       add %[_clock], %[_clkDelay]                                                               \n\t"
            "       add %[_clock], %[_clkDelayHalf]                                           \n\t"
            "loop%=:                                                                                \n\t"
            "       waitcnt %[_clock], %[_clkDelay]                                                 \n\t"
            "       testn %[_rx], ina wc                                                             \n\t" //tests sets c flag if _rx&ina has odd number of 1 bits
            "       rcr %[_data], #1                                                                \n\t"
            "       djnz %[_bitCount], #" FC_ADDR("loop%=", "ReadStart%=") "                        \n\t" //decrement counter and jump to loop if counter isnt zero
                                                                           "       waitcnt %[_clock], %[_clkDelay]                                                 \n\t"
                                                                           "       ror %[_data], %[_finalRotate]  \n\t" //rotate bits into lsb
            FC_END("ReadEnd%=")
            : [_bitCount] "+r"(bits),
              [_clock] "+r"(clock),
              [_data] "+r"(buffer[i])
            : [_rx] "r"(serial->rx_mask),
              [_clkDelay] "r"(serial->baudCNT),
              [_finalRotate] "r"(finalrot),
              [_clkDelayHalf] "r"(clockDelayhalf));
#pragma GCC diagnostic pop
    }
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
                                                                                                               "andn outa, %[_tx]\n\t"
                                                                                                               "       waitcnt %[_clock], %[_clkDelay]                                             \n\t" FC_END("writeEnd%=")
        : [_bitCount] "+r"(bits),
          [_data] "+r"(data),
          [_clock] "+r"(clock)
        : [_tx] "r"(serial->tx_mask),
          [_clkDelay] "r"(serial->baudCNT));
#pragma GCC diagnostic pop
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