#include "BufferlessSerial.h"
#define invert_rx 1
#define invert_tx 2

#define FC_START(start, end)                    \
    "        fcache #(" end " - " start ")\n\t" \
    "        .compress off\n\t" start ":\n\t"
#define FC_END(end)                         \
    "        jmp __LMM_RET\n\t" end ":\n\t" \
    "        .compress default\n\t"
#define FC_ADDR(to, start) "__LMM_FCACHE_START+(" to " - " start ")"

char uart_read(BLSerial *serial)
{
    //while()
}

void uart_write(BLSerial *serial, uint8_t p_data)
{
    unsigned int bits = 10;
    unsigned int data = (1 << bits) + (p_data << 1) + 1; //1xxxxxxxx1
    if ((serial->mode & invert_tx) == invert_tx)
    {
        data = ~data; //setting tx to low
        printf("invert tx\n");
    }
    unsigned int clock;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
    __asm__ volatile(
        FC_START("writeStart%=", "writeEnd%=") "       ror %[_data], %[_bitCount]                                                  \n\t"
                                               "       mov %[_clock], %[_clkDelay]                                                 \n\t"
                                               "       add %[_clock], CNT                                                          \n\t"
                                               "loop%=:                                                                            \n\t"
                                               "       rol %[_data], #1 wc                                                         \n\t"
                                               "       muxc outa, %[_tx]                                                         \n\t"
                                               "       waitcnt %[_clock], %[_clkDelay]                                             \n\t"
                                               "       djnz %[_bitCount], #" FC_ADDR("loop%=", "writeStart%=") "                   \n\t" FC_END("writeEnd%=")
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
        printf("invert tx\n");
    }
    else
    {
        OUTA |= serial->tx_mask; //setting tx to high
        printf("reg tx\n");
    }
}