
//SPI functions from propware library (extracted for size)

#include "SPI_Fast.h"

#define FC_START(start, end)                  \
  "        fcache #(" end " - " start ")\n\t" \
  "        .compress off\n\t" start ":\n\t"
#define FC_END(end)                       \
  "        jmp __LMM_RET\n\t" end ":\n\t" \
  "        .compress default\n\t"
#define FC_ADDR(to, start) "__LMM_FCACHE_START+(" to " - " start ")"

void shift_out_fast(SPI_Bus *bus, uint32_t bits, uint32_t data)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
  __asm__ volatile(
      FC_START("SpiSendMsbFirstStart%=", "SpiSendMsbFirstEnd%=") "       ror %[_data], %[_bitCount]                                                  \n\t"
                                                                 //"       mov %[_clock], %[_clkDelay]                                                 \n\t"

                                                                 "loop%=:                                                                            \n\t"
                                                                 "       rol %[_data], #1 wc                                                         \n\t"
                                                                 "       muxc outa, %[_mosi]                                                         \n\t"
                                                                 "       xor outa, %[_sclk]                                                          \n\t"
                                                                 "       xor outa, %[_sclk]                                                          \n\t"
                                                                 "       djnz %[_bitCount], #" FC_ADDR("loop%=", "SpiSendMsbFirstStart%=") "         \n\t"

                                                                                                                                           "       or outa, %[_mosi]                                                           \n\t" FC_END("SpiSendMsbFirstEnd%=")
      : [_bitCount] "+r"(bits),
        [_data] "+r"(data)
      : [_mosi] "r"(bus->mosi_mask),
        [_sclk] "r"(bus->sclk_mask));
#pragma GCC diagnostic pop
}

uint32_t shift_in_fast(SPI_Bus *bus, unsigned int bits)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
  unsigned int tempData;
  __asm__ volatile(
      FC_START("SpiReadMsbPhs0Start%=", "SpiReadMsbPhs0End%=") "       ror %[_data], %[_bitCount]              '' move MSB into bit 31                 \n\t"

                                                               "loop%=:                                                                                \n\t"
                                                               "       test %[_miso], ina wc                                                           \n\t"
                                                               "       xor outa, %[_sclk]                                                              \n\t"
                                                               "       rcl %[_data], #1                                                                \n\t"
                                                               "       xor outa, %[_sclk]                                                              \n\t"
                                                               "       djnz %[_bitCount], #" FC_ADDR("loop%=", "SpiReadMsbPhs0Start%=") "              \n\t" FC_END("SpiReadMsbPhs0End%=")
      : [_bitCount] "+r"(bits),
        [_data] "+r"(tempData)
      : [_miso] "r"(bus->miso_mask),
        [_sclk] "r"(bus->sclk_mask));
#pragma GCC diagnostic pop
  return tempData;
}

HUBTEXT void spi_open(SPI_Bus *bus, int mosi, int miso, int sclk)
{
  bus->mosi_mask = (1 << mosi);
  bus->miso_mask = (1 << miso);
  bus->sclk_mask = (1 << sclk);
  DIRA |= bus->mosi_mask;
  DIRA |= bus->sclk_mask;
  DIRA &= ~(bus->miso_mask);
}
