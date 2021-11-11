//**************************************************************//
/*
File Name : Ra8876_Lite.cpp                                   
Author    : RAiO Application Team                             
Edit Date : 09/13/2017
Version   : v2.0  1.modify bte_DestinationMemoryStartAddr bug 
                  2.modify display_sdram_initial Auto_Refresh
                  3.modify display_pll_initial 
*/
//**************************************************************//
#include "RA8876.h"

static unsigned char GTP_CFG_DATA[] =
    {

        0x63, 0x00, 0x04, 0x58, 0x02, 0x0A, 0x3D, 0x00,
        0x01, 0x08, 0x28, 0x0F, 0x50, 0x32, 0x03, 0x05,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17,
        0x19, 0x1D, 0x14, 0x90, 0x2F, 0x89, 0x23, 0x25,
        0xD3, 0x07, 0x00, 0x00, 0x00, 0x02, 0x03, 0x1D,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x19, 0x32, 0x94, 0xD5, 0x02,
        0x07, 0x00, 0x00, 0x04, 0xA2, 0x1A, 0x00, 0x90,
        0x1E, 0x00, 0x80, 0x23, 0x00, 0x73, 0x28, 0x00,
        0x68, 0x2E, 0x00, 0x68, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x16, 0x15, 0x14, 0x11, 0x10, 0x0F, 0x0E, 0x0D,
        0x0C, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x01,
        0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x28,
        0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20,
        0x1F, 0x1E, 0x1C, 0x1B, 0x19, 0x14, 0x13, 0x12,
        0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0A, 0x08,
        0x07, 0x06, 0x04, 0x02, 0x00, 0xFF, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x71, 0x01

};

Error display_begin(Display *display, int reset, int xnscs, int spi_mosi, int spi_miso, int spi_clk, int i2c_clk, int i2c_sda)
{
  display->reset = reset;
  display->xnscs = xnscs;

  display->spi_clk = spi_clk;
  display->spi_mosi = spi_mosi;
  display->spi_miso = spi_miso;

  display->i2cBus.setup(i2c_clk, i2c_sda, 100, 1); //1.5k pullup

  int i2c_addr = 0x5d;
  display->i2c_addr_write = (i2c_addr << 1) & 0b11111110;
  display->i2c_addr_read = (i2c_addr << 1) | 0b00000001;

  _pinl(display->reset); //reset low
  _waitms(2);            //wait 2ms
  _pinh(display->reset); //reset high
  _waitms(2);            //wait 20ms

  _waitms(1000); //wait 1s

  display_gt9271_send_cfg(display, (uint8_t *)GTP_CFG_DATA, sizeof(GTP_CFG_DATA));

  _pinh(display->xnscs); //xnscs high
  _pinl(spi_clk);        //sck low

  _pinh(reset);          //reset high
  _waitms(1);            //wait 1ms
  _pinl(display->reset); //reset low
  _waitms(1);            //wait 1ms
  _pinh(reset);          //reset high
  _waitms(10);           //wait 10ms

  //read ID code must disable pll, 01h bit7 set 0
  lcdRegDataWrite(display, 0x01, 0x08);
  _waitms(100); //wait 100ms

  if ((lcdRegDataRead(display, 0xff) != 0x76) && (lcdRegDataRead(display, 0xff) != 0x77))
  {
    printf("Display not found\n");
    return DISPLAY_NOT_FOUND;
  }
  Error err;
  if ((err = display_initialize(display)) != SUCCESS)
  {
    printf("Error:%d\n", err);
    return err;
  }
  return SUCCESS;
}
//**************************************************************//

Error display_initialize(Display *display)
{
  if (!display_pll_initial(display))
  {
    print("PLL initial fail!");
    return DISPLAY_PLL_FAIL;
  }
  if (!display_sdram_initial(display))
  {
    print("SDRAM initial fail!");
    return DISPLAY_SDRAM_FAIL;
  }
  lcdRegWrite(display, RA8876_CCR); //01h
  lcdDataWrite(display, RA8876_PLL_ENABLE << 7 | RA8876_WAIT_NO_MASK << 6 | RA8876_KEY_SCAN_DISABLE << 5 | RA8876_TFT_OUTPUT24 << 3 | RA8876_I2C_MASTER_DISABLE << 2 | RA8876_SERIAL_IF_ENABLE << 1 | RA8876_HOST_DATA_BUS_SERIAL);

  lcdRegWrite(display, RA8876_MACR); //02h
  lcdDataWrite(display, RA8876_DIRECT_WRITE << 6 | RA8876_READ_MEMORY_LRTB << 4 | RA8876_WRITE_MEMORY_LRTB << 1);

  lcdRegWrite(display, RA8876_ICR); //03h
  lcdDataWrite(display, RA8877_LVDS_FORMAT << 3 | RA8876_GRAPHIC_MODE << 2 | RA8876_MEMORY_SELECT_IMAGE);

  lcdRegWrite(display, RA8876_MPWCTR); //10h
  lcdDataWrite(display, RA8876_PIP1_WINDOW_DISABLE << 7 | RA8876_PIP2_WINDOW_DISABLE << 6 | RA8876_SELECT_CONFIG_PIP1 << 4 | RA8876_IMAGE_COLOCR_DEPTH_16BPP << 2 | TFT_MODE);

  lcdRegWrite(display, RA8876_PIPCDEP); //11h
  lcdDataWrite(display, RA8876_PIP1_COLOR_DEPTH_16BPP << 2 | RA8876_PIP2_COLOR_DEPTH_16BPP);

  lcdRegWrite(display, RA8876_AW_COLOR); //5Eh
  lcdDataWrite(display, RA8876_CANVAS_BLOCK_MODE << 2 | RA8876_CANVAS_COLOR_DEPTH_16BPP);

  lcdRegDataWrite(display, RA8876_BTE_COLR, RA8876_S0_COLOR_DEPTH_16BPP << 5 | RA8876_S1_COLOR_DEPTH_16BPP << 2 | RA8876_S0_COLOR_DEPTH_16BPP); //92h

  /*TFT timing configure*/
  lcdRegWrite(display, RA8876_DPCR); //12h
  lcdDataWrite(display, XPCLK_INV << 7 | RA8876_DISPLAY_OFF << 6 | RA8876_OUTPUT_RGB);

  lcdRegWrite(display, RA8876_PCSR); //13h
  lcdDataWrite(display, XHSYNC_INV << 7 | XVSYNC_INV << 6 | XDE_INV << 5);

  display_lcd_horizontal_width_vertical_height(display, HDW, VDH);
  display_lcd_horizontal_non_display(display, HND);
  display_lcd_hsync_start_position(display, HST);
  display_lcd_hsync_pulse_width(display, HPW);
  display_lcd_vertical_non_display(display, VND);
  display_lcd_vsync_start_position(display, VST);
  display_lcd_vsync_pulse_width(display, VPW);

  /*image buffer configure*/
  display_image_start_address(display, PAGE1_START_ADDR);
  display_image_width(display, SCREEN_WIDTH);
  display_window_start_xy(display, 0, 0);
  display_canvas_image_start_address(display, PAGE1_START_ADDR);
  display_canvas_image_width(display, SCREEN_WIDTH);
  display_active_window_xy(display, 0, 0);
  display_active_window_wh(display, SCREEN_WIDTH, SCREEN_HEIGHT);
  return SUCCESS;
}
//**************************************************************//
//**************************************************************//
void lcdRegWrite(Display *display, uint8_t reg)
{
  _pinl(display->xnscs); //xnscs low
  //shift_out_fast(&spi_bus, (uint32_t)8, (uint32_t)RA8876_SPI_CMDWRITE);@todo implement spi interface
  //shift_out_fast(&spi_bus, (uint32_t)8, (uint32_t)reg);
  shift_out(display->spi_mosi, display->spi_clk, MSBFIRST, (uint32_t)8, (uint32_t)RA8876_SPI_CMDWRITE);
  shift_out(display->spi_mosi, display->spi_clk, MSBFIRST, (uint32_t)8, (uint32_t)reg);
  _pinh(display->xnscs); //xnscs high
}
//**************************************************************//
//**************************************************************//
void lcdDataWrite(Display *display, uint8_t data)
{
  _pinl(display->xnscs); //xnscs low
  //shift_out_fast(&spi_bus, (uint32_t)8, (uint32_t)RA8876_SPI_DATAWRITE);
  //shift_out_fast(&spi_bus, (uint32_t)8, (uint32_t)data); @todo implement spi interface
  shift_out(display->spi_mosi, display->spi_clk, MSBFIRST, 8, (uint8_t)RA8876_SPI_DATAWRITE);
  shift_out(display->spi_mosi, display->spi_clk, MSBFIRST, 8, (uint8_t)data);
  _pinh(display->xnscs); //xnscs high
}
//**************************************************************//
//**************************************************************//
uint8_t lcdDataRead(Display *display)
{
  _pinl(display->xnscs); //xnscs low
  //shift_out_fast(&spi_bus, (uint32_t)8, (uint32_t)RA8876_SPI_DATAREAD);@todo implement spi interface
  shift_out(display->spi_mosi, display->spi_clk, MSBFIRST, (uint32_t)8, (uint32_t)RA8876_SPI_DATAREAD);
  _pinh(display->spi_mosi); //mosi high
  //uint8_t data = shift_in_fast(&spi_bus, 8);
  uint8_t data = shift_in(display->spi_miso, display->spi_clk, LSBFIRST, (uint32_t)8);
  _pinh(display->xnscs); //xnscs high
  return data;
}
//**************************************************************//
//**************************************************************//
uint8_t lcdStatusRead(Display *display)
{
  _pinl(display->xnscs); //xnscs low
  //shift_out_fast(&spi_bus, (uint32_t)8, (uint32_t)RA8876_SPI_STATUSREAD);@todo implement spi interface
  shift_out(display->spi_mosi, display->spi_clk, MSBFIRST, (uint32_t)8, (uint32_t)RA8876_SPI_STATUSREAD);
  _pinh(display->spi_mosi); //mosi high
  //uint8_t data = shift_in_fast(&spi_bus, 8);
  uint8_t data = shift_in(display->spi_miso, display->spi_clk, LSBFIRST, (uint32_t)8);
  _pinh(display->xnscs); //xnscs high
  return data;
}
//**************************************************************//
//**************************************************************//
void lcdRegDataWrite(Display *display, uint8_t reg, uint8_t data)
{
  lcdRegWrite(display, reg);
  lcdDataWrite(display, data);
}
//**************************************************************//
//**************************************************************//
uint8_t lcdRegDataRead(Display *display, uint8_t reg)
{
  lcdRegWrite(display, reg);
  uint8_t data = lcdDataRead(display);
  return data;
}

//**************************************************************//
// support SPI interface to write 16bpp data after Regwrite 04h
//**************************************************************//
void lcdDataWrite16bbp(Display *display, uint16_t data)
{
  _pinl(display->xnscs); //xnscs low
  //shift_out_fast(&spi_bus, (uint32_t)8, (uint32_t)RA8876_SPI_DATAWRITE);@todo implement spi interface
  //shift_out_fast(&spi_bus, (uint32_t)8, (uint32_t)data);
  //shift_out_fast(&spi_bus, (uint32_t)8, (uint32_t)data >> 8);
  shift_out(display->spi_mosi, display->spi_clk, MSBFIRST, (uint32_t)8, (uint32_t)RA8876_SPI_DATAWRITE);
  shift_out(display->spi_mosi, display->spi_clk, MSBFIRST, (uint32_t)8, (uint32_t)data);
  shift_out(display->spi_mosi, display->spi_clk, MSBFIRST, (uint32_t)8, (uint32_t)data >> 8);
  _pinh(display->xnscs); //xnscs high
}

//**************************************************************//
//RA8876 register
//**************************************************************//
/*[Status Register] bit7  Host Memory Write FIFO full
0: Memory Write FIFO is not full.
1: Memory Write FIFO is full.
Only when Memory Write FIFO is not full, MPU may write another one pixel.*/
//**************************************************************//
void checkWriteFifoNotFull(Display *display)
{
  uint16_t i;
  for (i = 0; i < 10000; i++) //Please according to your usage to modify i value.
  {
    if ((lcdStatusRead(display) & 0x80) == 0)
    {
      break;
    }
  }
}

//**************************************************************//
/*[Status Register] bit6  Host Memory Write FIFO empty
0: Memory Write FIFO is not empty.
1: Memory Write FIFO is empty.
When Memory Write FIFO is empty, MPU may write 8bpp data 64
pixels, or 16bpp data 32 pixels, 24bpp data 16 pixels directly.*/
//**************************************************************//
void checkWriteFifoEmpty(Display *display)
{
  uint16_t i;
  for (i = 0; i < 10000; i++) //Please according to your usage to modify i value.
  {
    if ((lcdStatusRead(display) & 0x40) == 0x40)
    {
      break;
    }
  }
}

//**************************************************************//
/*[Status Register] bit5  Host Memory Read FIFO full
0: Memory Read FIFO is not full.
1: Memory Read FIFO is full.
When Memory Read FIFO is full, MPU may read 8bpp data 32
pixels, or 16bpp data 16 pixels, 24bpp data 8 pixels directly.*/
//**************************************************************//
void checkReadFifoNotFull(Display *display)
{
  uint16_t i;
  for (i = 0; i < 10000; i++) //Please according to your usage to modify i value.
  {
    if ((lcdStatusRead(display) & 0x20) == 0x00)
    {
      break;
    }
  }
}

//**************************************************************//
/*[Status Register] bit4   Host Memory Read FIFO empty
0: Memory Read FIFO is not empty.
1: Memory Read FIFO is empty.*/
//**************************************************************//
void checkReadFifoNotEmpty(Display *display)
{
  uint16_t i;
  for (i = 0; i < 10000; i++) // //Please according to your usage to modify i value.
  {
    if ((lcdStatusRead(display) & 0x10) == 0x00)
    {
      break;
    }
  }
}

//**************************************************************//
/*[Status Register] bit3   Core task is busy
Following task is running:
BTE, Geometry engine, Serial flash DMA, Text write or Graphic write
0: task is done or idle.   1: task is busy*/
//**************************************************************//
void check2dBusy(Display *display)
{
  uint32_t i;
  for (i = 0; i < 1000000; i++) //Please according to your usage to modify i value.
  {
    _waitus(1); //wait 1us
    if ((lcdStatusRead(display) & 0x08) == 0x00)
    {
      break;
    }
  }
}

//**************************************************************//
/*[Status Register] bit2   SDRAM ready for access
0: SDRAM is not ready for access   1: SDRAM is ready for access*/
//**************************************************************//
bool checkSdramReady(Display *display)
{
  uint32_t i;
  for (i = 0; i < 1000000; i++) //Please according to your usage to modify i value.
  {
    _waitms(1); //wait 1ms
    if ((lcdStatusRead(display) & 0x04) == 0x04)
    {
      return true;
    }
  }
  return false;
}

//**************************************************************//
/*[Status Register] bit1  Operation mode status
0: Normal operation state  1: Inhibit operation state
Inhibit operation state means internal reset event keep running or
initial display still running or chip enter power saving state.	*/
//**************************************************************//
bool checkIcReady(Display *display)
{
  uint32_t i;
  for (i = 0; i < 1000000; i++) //Please according to your usage to modify i value.
  {
    _waitus(1); //wait 1us
    if ((lcdStatusRead(display) & 0x02) == 0x00)
    {
      return true;
    }
  }
  return false;
}
//**************************************************************//
//**************************************************************//
//[05h] [06h] [07h] [08h] [09h] [0Ah]
//------------------------------------//----------------------------------*/
bool display_pll_initial(Display *display)
{
  /*(1) 10MHz <= OSC_FREQ <= 15MHz 
  (2) 10MHz <= (OSC_FREQ/PLLDIVM) <= 40MHz
  (3) 250MHz <= [OSC_FREQ/(PLLDIVM+1)]x(PLLDIVN+1) <= 600MHz
PLLDIVM:0
PLLDIVN:1~63
PLLDIVK:CPLL & MPLL = 1/2/4/8.SPLL = 1/2/4/8/16/32/64/128.
ex:
 OSC_FREQ = 10MHz
 Set X_DIVK=2
 Set X_DIVM=0
 => (X_DIVN+1)=(XPLLx4)/10*/

  // Set tft output pixel clock
  if (SCAN_FREQ >= 79) //&&(SCAN_FREQ<=100))
  {
    lcdRegDataWrite(display, 0x05, 0x04); //PLL Divided by 4
    lcdRegDataWrite(display, 0x06, (SCAN_FREQ * 4 / OSC_FREQ) - 1);
  }
  else if ((SCAN_FREQ >= 63) && (SCAN_FREQ <= 78))
  {
    lcdRegDataWrite(display, 0x05, 0x05); //PLL Divided by 4
    lcdRegDataWrite(display, 0x06, (SCAN_FREQ * 8 / OSC_FREQ) - 1);
  }
  else if ((SCAN_FREQ >= 40) && (SCAN_FREQ <= 62))
  {
    lcdRegDataWrite(display, 0x05, 0x06); //PLL Divided by 8
    lcdRegDataWrite(display, 0x06, (SCAN_FREQ * 8 / OSC_FREQ) - 1);
  }
  else if ((SCAN_FREQ >= 32) && (SCAN_FREQ <= 39))
  {
    lcdRegDataWrite(display, 0x05, 0x07); //PLL Divided by 8
    lcdRegDataWrite(display, 0x06, (SCAN_FREQ * 16 / OSC_FREQ) - 1);
  }
  else if ((SCAN_FREQ >= 16) && (SCAN_FREQ <= 31))
  {
    lcdRegDataWrite(display, 0x05, 0x16); //PLL Divided by 16
    lcdRegDataWrite(display, 0x06, (SCAN_FREQ * 16 / OSC_FREQ) - 1);
  }
  else if ((SCAN_FREQ >= 8) && (SCAN_FREQ <= 15))
  {
    lcdRegDataWrite(display, 0x05, 0x26); //PLL Divided by 32
    lcdRegDataWrite(display, 0x06, (SCAN_FREQ * 32 / OSC_FREQ) - 1);
  }
  else if ((SCAN_FREQ > 0) && (SCAN_FREQ <= 7))
  {
    lcdRegDataWrite(display, 0x05, 0x36); //PLL Divided by 64
    lcdRegDataWrite(display, 0x06, (SCAN_FREQ * 64 / OSC_FREQ) - 1);
  }

  // Set internal Buffer Ram clock
  if (DRAM_FREQ >= 158) //
  {
    lcdRegDataWrite(display, 0x07, 0x02); //PLL Divided by 4
    lcdRegDataWrite(display, 0x08, (DRAM_FREQ * 2 / OSC_FREQ) - 1);
  }
  else if ((DRAM_FREQ >= 125) && (DRAM_FREQ <= 157))
  {
    lcdRegDataWrite(display, 0x07, 0x03); //PLL Divided by 4
    lcdRegDataWrite(display, 0x08, (DRAM_FREQ * 4 / OSC_FREQ) - 1);
  }
  else if ((DRAM_FREQ >= 79) && (DRAM_FREQ <= 124))
  {
    lcdRegDataWrite(display, 0x07, 0x04); //PLL Divided by 4
    lcdRegDataWrite(display, 0x08, (DRAM_FREQ * 4 / OSC_FREQ) - 1);
  }
  else if ((DRAM_FREQ >= 63) && (DRAM_FREQ <= 78))
  {
    lcdRegDataWrite(display, 0x07, 0x05); //PLL Divided by 4
    lcdRegDataWrite(display, 0x08, (DRAM_FREQ * 8 / OSC_FREQ) - 1);
  }
  else if ((DRAM_FREQ >= 40) && (DRAM_FREQ <= 62))
  {
    lcdRegDataWrite(display, 0x07, 0x06); //PLL Divided by 8
    lcdRegDataWrite(display, 0x08, (DRAM_FREQ * 8 / OSC_FREQ) - 1);
  }
  else if ((DRAM_FREQ >= 32) && (DRAM_FREQ <= 39))
  {
    lcdRegDataWrite(display, 0x07, 0x07); //PLL Divided by 16
    lcdRegDataWrite(display, 0x08, (DRAM_FREQ * 16 / OSC_FREQ) - 1);
  }
  else if (DRAM_FREQ <= 31)
  {
    lcdRegDataWrite(display, 0x07, 0x06);                    //PLL Divided by 8
    lcdRegDataWrite(display, 0x08, (30 * 8 / OSC_FREQ) - 1); //set to 30MHz if out off range
  }

  // Set Core clock
  if (CORE_FREQ >= 158)
  {
    lcdRegDataWrite(display, 0x09, 0x02); //PLL Divided by 2
    lcdRegDataWrite(display, 0x0A, (CORE_FREQ * 2 / OSC_FREQ) - 1);
  }
  else if ((CORE_FREQ >= 125) && (CORE_FREQ <= 157))
  {
    lcdRegDataWrite(display, 0x09, 0x03); //PLL Divided by 4
    lcdRegDataWrite(display, 0x0A, (CORE_FREQ * 4 / OSC_FREQ) - 1);
  }
  else if ((CORE_FREQ >= 79) && (CORE_FREQ <= 124))
  {
    lcdRegDataWrite(display, 0x09, 0x04); //PLL Divided by 4
    lcdRegDataWrite(display, 0x0A, (CORE_FREQ * 4 / OSC_FREQ) - 1);
  }
  else if ((CORE_FREQ >= 63) && (CORE_FREQ <= 78))
  {
    lcdRegDataWrite(display, 0x09, 0x05); //PLL Divided by 8
    lcdRegDataWrite(display, 0x0A, (CORE_FREQ * 8 / OSC_FREQ) - 1);
  }
  else if ((CORE_FREQ >= 40) && (CORE_FREQ <= 62))
  {
    lcdRegDataWrite(display, 0x09, 0x06); //PLL Divided by 8
    lcdRegDataWrite(display, 0x0A, (CORE_FREQ * 8 / OSC_FREQ) - 1);
  }
  else if ((CORE_FREQ >= 32) && (CORE_FREQ <= 39))
  {
    lcdRegDataWrite(display, 0x09, 0x06); //PLL Divided by 8
    lcdRegDataWrite(display, 0x0A, (CORE_FREQ * 8 / OSC_FREQ) - 1);
  }
  else if (CORE_FREQ <= 31)
  {
    lcdRegDataWrite(display, 0x09, 0x06);                    //PLL Divided by 8
    lcdRegDataWrite(display, 0x0A, (30 * 8 / OSC_FREQ) - 1); //set to 30MHz if out off range
  }

  _waitms(1); //wait 1ms
  lcdRegWrite(display, 0x01);
  lcdDataWrite(display, 0x80);
  _waitms(2); //wait 2ms for pll stable
  if ((lcdDataRead(display) & 0x80) == 0x80)
    return true;
  else
    return false;
}

//**************************************************************//
bool display_sdram_initial(Display *display)
{
  uint8_t CAS_Latency;
  uint16_t Auto_Refresh;
#ifdef W9812G6JH
  CAS_Latency = 3;
  Auto_Refresh = (64 * DRAM_FREQ * 1000) / (4096);
  Auto_Refresh = Auto_Refresh - 2;
  lcdRegDataWrite(display, 0xe0, 0x29);
  lcdRegDataWrite(display, 0xe1, CAS_Latency); //CAS:2=0x02£¬CAS:3=0x03
  lcdRegDataWrite(display, 0xe2, Auto_Refresh);
  lcdRegDataWrite(display, 0xe3, Auto_Refresh >> 8);
  lcdRegDataWrite(display, 0xe4, 0x01);
#endif
  checkSdramReady(display);
  return true;
}
//**************************************************************//
//**************************************************************//
void display_on(Display *display, bool on)
{
  if (on)
    lcdRegDataWrite(display, RA8876_DPCR, XPCLK_INV << 7 | RA8876_DISPLAY_ON << 6 | RA8876_OUTPUT_RGB);
  else
    lcdRegDataWrite(display, RA8876_DPCR, XPCLK_INV << 7 | RA8876_DISPLAY_OFF << 6 | RA8876_OUTPUT_RGB);
  _waitms(20); //wait 20ms
}
//**************************************************************//
//**************************************************************//
void display_lcd_horizontal_width_vertical_height(Display *display, uint16_t width, uint16_t height)
{
  unsigned char temp;
  temp = (width / 8) - 1;
  lcdRegDataWrite(display, RA8876_HDWR, temp);
  temp = width % 8;
  lcdRegDataWrite(display, RA8876_HDWFTR, temp);
  temp = height - 1;
  lcdRegDataWrite(display, RA8876_VDHR0, temp);
  temp = (height - 1) >> 8;
  lcdRegDataWrite(display, RA8876_VDHR1, temp);
}
//**************************************************************//
//**************************************************************//
void display_lcd_horizontal_non_display(Display *display, uint16_t numbers)
{
  uint8_t temp;
  if (numbers < 8)
  {
    lcdRegDataWrite(display, RA8876_HNDR, 0x00);
    lcdRegDataWrite(display, RA8876_HNDFTR, numbers);
  }
  else
  {
    temp = (numbers / 8) - 1;
    lcdRegDataWrite(display, RA8876_HNDR, temp);
    temp = numbers % 8;
    lcdRegDataWrite(display, RA8876_HNDFTR, temp);
  }
}
//**************************************************************//
//**************************************************************//
void display_lcd_hsync_start_position(Display *display, uint16_t numbers)
{
  uint8_t temp;
  if (numbers < 8)
  {
    lcdRegDataWrite(display, RA8876_HSTR, 0x00);
  }
  else
  {
    temp = (numbers / 8) - 1;
    lcdRegDataWrite(display, RA8876_HSTR, temp);
  }
}
//**************************************************************//
//**************************************************************//
void display_lcd_hsync_pulse_width(Display *display, uint16_t numbers)
{
  uint8_t temp;
  if (numbers < 8)
  {
    lcdRegDataWrite(display, RA8876_HPWR, 0x00);
  }
  else
  {
    temp = (numbers / 8) - 1;
    lcdRegDataWrite(display, RA8876_HPWR, temp);
  }
}
//**************************************************************//
//**************************************************************//
void display_lcd_vertical_non_display(Display *display, uint16_t numbers)
{
  uint8_t temp;
  temp = numbers - 1;
  lcdRegDataWrite(display, RA8876_VNDR0, temp);
  lcdRegDataWrite(display, RA8876_VNDR1, temp >> 8);
}
//**************************************************************//
//**************************************************************//
void display_lcd_vsync_start_position(Display *display, uint16_t numbers)
{
  uint8_t temp;
  temp = numbers - 1;
  lcdRegDataWrite(display, RA8876_VSTR, temp);
}
//**************************************************************//
//**************************************************************//
void display_lcd_vsync_pulse_width(Display *display, uint16_t numbers)
{
  uint8_t temp;
  temp = numbers - 1;
  lcdRegDataWrite(display, RA8876_VPWR, temp);
}
//**************************************************************//
//**************************************************************//
void display_image_start_address(Display *display, uint32_t addr)
{
  lcdRegDataWrite(display, RA8876_MISA0, addr);       //20h
  lcdRegDataWrite(display, RA8876_MISA1, addr >> 8);  //21h
  lcdRegDataWrite(display, RA8876_MISA2, addr >> 16); //22h
  lcdRegDataWrite(display, RA8876_MISA3, addr >> 24); //23h
}
//**************************************************************//
//**************************************************************//
void display_image_width(Display *display, uint16_t width)
{
  lcdRegDataWrite(display, RA8876_MIW0, width);      //24h
  lcdRegDataWrite(display, RA8876_MIW1, width >> 8); //25h
}
//**************************************************************//
//**************************************************************//
void display_window_start_xy(Display *display, uint16_t x0, uint16_t y0)
{
  lcdRegDataWrite(display, RA8876_MWULX0, x0);      //26h
  lcdRegDataWrite(display, RA8876_MWULX1, x0 >> 8); //27h
  lcdRegDataWrite(display, RA8876_MWULY0, y0);      //28h
  lcdRegDataWrite(display, RA8876_MWULY1, y0 >> 8); //29h
}
//**************************************************************//
//**************************************************************//
void display_canvas_image_start_address(Display *display, uint32_t addr)
{
  lcdRegDataWrite(display, RA8876_CVSSA0, addr);       //50h
  lcdRegDataWrite(display, RA8876_CVSSA1, addr >> 8);  //51h
  lcdRegDataWrite(display, RA8876_CVSSA2, addr >> 16); //52h
  lcdRegDataWrite(display, RA8876_CVSSA3, addr >> 24); //53h
}
//**************************************************************//
//**************************************************************//
void display_canvas_image_width(Display *display, uint16_t width)
{
  lcdRegDataWrite(display, RA8876_CVS_IMWTH0, width);      //54h
  lcdRegDataWrite(display, RA8876_CVS_IMWTH1, width >> 8); //55h
}
//**************************************************************//
//**************************************************************//
void display_active_window_xy(Display *display, uint16_t x0, uint16_t y0)
{
  lcdRegDataWrite(display, RA8876_AWUL_X0, x0);      //56h
  lcdRegDataWrite(display, RA8876_AWUL_X1, x0 >> 8); //57h
  lcdRegDataWrite(display, RA8876_AWUL_Y0, y0);      //58h
  lcdRegDataWrite(display, RA8876_AWUL_Y1, y0 >> 8); //59h
}
//**************************************************************//
//**************************************************************//
void display_active_window_wh(Display *display, uint16_t width, uint16_t height)
{
  lcdRegDataWrite(display, RA8876_AW_WTH0, width);      //5ah
  lcdRegDataWrite(display, RA8876_AW_WTH1, width >> 8); //5bh
  lcdRegDataWrite(display, RA8876_AW_HT0, height);      //5ch
  lcdRegDataWrite(display, RA8876_AW_HT1, height >> 8); //5dh
}
//**************************************************************//
//**************************************************************//
void display_set_pixel_cursor(Display *display, uint16_t x, uint16_t y)
{
  lcdRegDataWrite(display, RA8876_CURH0, x);      //5fh
  lcdRegDataWrite(display, RA8876_CURH1, x >> 8); //60h
  lcdRegDataWrite(display, RA8876_CURV0, y);      //61h
  lcdRegDataWrite(display, RA8876_CURV1, y >> 8); //62h
}
//**************************************************************//
//**************************************************************//
void bte_Source0_MemoryStartAddr(Display *display, uint32_t addr)
{
  lcdRegDataWrite(display, RA8876_S0_STR0, addr);       //93h
  lcdRegDataWrite(display, RA8876_S0_STR1, addr >> 8);  //94h
  lcdRegDataWrite(display, RA8876_S0_STR2, addr >> 16); //95h
  lcdRegDataWrite(display, RA8876_S0_STR3, addr >> 24); ////96h
}
//**************************************************************//
//**************************************************************//
void bte_Source0_ImageWidth(Display *display, uint16_t width)
{
  lcdRegDataWrite(display, RA8876_S0_WTH0, width);      //97h
  lcdRegDataWrite(display, RA8876_S0_WTH1, width >> 8); //98h
}
//**************************************************************//
//**************************************************************//
void bte_Source0_WindowStartXY(Display *display, uint16_t x0, uint16_t y0)
{
  lcdRegDataWrite(display, RA8876_S0_X0, x0);      //99h
  lcdRegDataWrite(display, RA8876_S0_X1, x0 >> 8); //9ah
  lcdRegDataWrite(display, RA8876_S0_Y0, y0);      //9bh
  lcdRegDataWrite(display, RA8876_S0_Y1, y0 >> 8); //9ch
}
//**************************************************************//
//**************************************************************//
void bte_Source1_MemoryStartAddr(Display *display, uint32_t addr)
{
  lcdRegDataWrite(display, RA8876_S1_STR0, addr);       //9dh
  lcdRegDataWrite(display, RA8876_S1_STR1, addr >> 8);  //9eh
  lcdRegDataWrite(display, RA8876_S1_STR2, addr >> 16); //9fh
  lcdRegDataWrite(display, RA8876_S1_STR3, addr >> 24); //a0h
}
//**************************************************************//
//**************************************************************//
void bte_Source1_ImageWidth(Display *display, uint16_t width)
{
  lcdRegDataWrite(display, RA8876_S1_WTH0, width);      //a1h
  lcdRegDataWrite(display, RA8876_S1_WTH1, width >> 8); //a2h
}
//**************************************************************//
//**************************************************************//
void bte_Source1_WindowStartXY(Display *display, uint16_t x0, uint16_t y0)
{
  lcdRegDataWrite(display, RA8876_S1_X0, x0);      //a3h
  lcdRegDataWrite(display, RA8876_S1_X1, x0 >> 8); //a4h
  lcdRegDataWrite(display, RA8876_S1_Y0, y0);      //a5h
  lcdRegDataWrite(display, RA8876_S1_Y1, y0 >> 8); //a6h
}
//**************************************************************//
//**************************************************************//
void bte_DestinationMemoryStartAddr(Display *display, uint32_t addr)
{
  lcdRegDataWrite(display, RA8876_DT_STR0, addr);       //a7h
  lcdRegDataWrite(display, RA8876_DT_STR1, addr >> 8);  //a8h
  lcdRegDataWrite(display, RA8876_DT_STR2, addr >> 16); //a9h
  lcdRegDataWrite(display, RA8876_DT_STR3, addr >> 24); //aah
}
//**************************************************************//
//**************************************************************//
void bte_DestinationImageWidth(Display *display, uint16_t width)
{
  lcdRegDataWrite(display, RA8876_DT_WTH0, width);      //abh
  lcdRegDataWrite(display, RA8876_DT_WTH1, width >> 8); //ach
}
//**************************************************************//
//**************************************************************//
void bte_DestinationWindowStartXY(Display *display, uint16_t x0, uint16_t y0)
{
  lcdRegDataWrite(display, RA8876_DT_X0, x0);      //adh
  lcdRegDataWrite(display, RA8876_DT_X1, x0 >> 8); //aeh
  lcdRegDataWrite(display, RA8876_DT_Y0, y0);      //afh
  lcdRegDataWrite(display, RA8876_DT_Y1, y0 >> 8); //b0h
}
//**************************************************************//
//**************************************************************//
void bte_WindowSize(Display *display, uint16_t width, uint16_t height)
{
  lcdRegDataWrite(display, RA8876_BTE_WTH0, width);       //b1h
  lcdRegDataWrite(display, RA8876_BTE_WTH1, width >> 8);  //b2h
  lcdRegDataWrite(display, RA8876_BTE_HIG0, height);      //b3h
  lcdRegDataWrite(display, RA8876_BTE_HIG1, height >> 8); //b4h
}

/*BTE function*/
//**************************************************************//
//**************************************************************//
void display_bte_memory_copy(Display *display, uint32_t s0_addr, uint16_t s0_image_width, uint16_t s0_x, uint16_t s0_y, uint32_t des_addr, uint16_t des_image_width,
                             uint16_t des_x, uint16_t des_y, uint16_t copy_width, uint16_t copy_height)
{
  bte_Source0_MemoryStartAddr(display, s0_addr);
  bte_Source0_ImageWidth(display, s0_image_width);
  bte_Source0_WindowStartXY(display, s0_x, s0_y);

  bte_DestinationMemoryStartAddr(display, des_addr);
  bte_DestinationImageWidth(display, des_image_width);
  bte_DestinationWindowStartXY(display, des_x, des_y);

  bte_WindowSize(display, copy_width, copy_height);
  lcdRegDataWrite(display, RA8876_BTE_CTRL1, RA8876_BTE_ROP_CODE_12 << 4 | RA8876_BTE_MEMORY_COPY_WITH_ROP);                                             //91h
  lcdRegDataWrite(display, RA8876_BTE_COLR, RA8876_S0_COLOR_DEPTH_16BPP << 5 | RA8876_S1_COLOR_DEPTH_16BPP << 2 | RA8876_DESTINATION_COLOR_DEPTH_16BPP); //92h
  lcdRegDataWrite(display, RA8876_BTE_CTRL0, RA8876_BTE_ENABLE << 4);                                                                                    //90h
  check2dBusy(display);
}
//**************************************************************//
//**************************************************************//
void display_bte_memory_copy_with_rop(Display *display, uint32_t s0_addr, uint16_t s0_image_width, uint16_t s0_x, uint16_t s0_y, uint32_t s1_addr, uint16_t s1_image_width, uint16_t s1_x, uint16_t s1_y,
                                      uint32_t des_addr, uint16_t des_image_width, uint16_t des_x, uint16_t des_y, uint16_t copy_width, uint16_t copy_height, uint8_t rop_code)
{
  bte_Source0_MemoryStartAddr(display, s0_addr);
  bte_Source0_ImageWidth(display, s0_image_width);
  bte_Source0_WindowStartXY(display, s0_x, s0_y);
  bte_Source1_MemoryStartAddr(display, s1_addr);
  bte_Source1_ImageWidth(display, s1_image_width);
  bte_Source1_WindowStartXY(display, s1_x, s1_y);
  bte_DestinationMemoryStartAddr(display, des_addr);
  bte_DestinationImageWidth(display, des_image_width);
  bte_DestinationWindowStartXY(display, des_x, des_y);
  bte_WindowSize(display, copy_width, copy_height);
  lcdRegDataWrite(display, RA8876_BTE_CTRL1, rop_code << 4 | RA8876_BTE_MEMORY_COPY_WITH_ROP);                                                           //91h
  lcdRegDataWrite(display, RA8876_BTE_COLR, RA8876_S0_COLOR_DEPTH_16BPP << 5 | RA8876_S1_COLOR_DEPTH_16BPP << 2 | RA8876_DESTINATION_COLOR_DEPTH_16BPP); //92h
  lcdRegDataWrite(display, RA8876_BTE_CTRL0, RA8876_BTE_ENABLE << 4);                                                                                    //90h
  check2dBusy(display);
}
//**************************************************************//
//**************************************************************//
void display_bte_memory_copy_with_chroma_key(Display *display, uint32_t s0_addr, uint16_t s0_image_width, uint16_t s0_x, uint16_t s0_y,
                                             uint32_t des_addr, uint16_t des_image_width, uint16_t des_x, uint16_t des_y, uint16_t copy_width, uint16_t copy_height, uint16_t chromakey_color)
{
  bte_Source0_MemoryStartAddr(display, s0_addr);
  bte_Source0_ImageWidth(display, s0_image_width);
  bte_Source0_WindowStartXY(display, s0_x, s0_y);
  bte_DestinationMemoryStartAddr(display, des_addr);
  bte_DestinationImageWidth(display, des_image_width);
  bte_DestinationWindowStartXY(display, des_x, des_y);
  bte_WindowSize(display, copy_width, copy_height);
  display_back_ground_color_16bpp(display, chromakey_color);
  lcdRegDataWrite(display, RA8876_BTE_CTRL1, RA8876_BTE_MEMORY_COPY_WITH_CHROMA);                                                                        //91h
  lcdRegDataWrite(display, RA8876_BTE_COLR, RA8876_S0_COLOR_DEPTH_16BPP << 5 | RA8876_S1_COLOR_DEPTH_16BPP << 2 | RA8876_DESTINATION_COLOR_DEPTH_16BPP); //92h
  lcdRegDataWrite(display, RA8876_BTE_CTRL0, RA8876_BTE_ENABLE << 4);                                                                                    //90h
  check2dBusy(display);
}

//**************************************************************//
//write data after setting
//**************************************************************//
void display_bte_mpu_write_with_rop(Display *display, uint32_t s1_addr, uint16_t s1_image_width, uint16_t s1_x, uint16_t s1_y, uint32_t des_addr, uint16_t des_image_width,
                                    uint16_t des_x, uint16_t des_y, uint16_t width, uint16_t height, uint8_t rop_code)
{
  uint16_t i, j;
  bte_Source1_MemoryStartAddr(display, s1_addr);
  bte_Source1_ImageWidth(display, s1_image_width);
  bte_Source1_WindowStartXY(display, s1_x, s1_y);
  bte_DestinationMemoryStartAddr(display, des_addr);
  bte_DestinationImageWidth(display, des_image_width);
  bte_DestinationWindowStartXY(display, des_x, des_y);
  bte_WindowSize(display, width, height);
  lcdRegDataWrite(display, RA8876_BTE_CTRL1, rop_code << 4 | RA8876_BTE_MPU_WRITE_WITH_ROP);                                                             //91h
  lcdRegDataWrite(display, RA8876_BTE_COLR, RA8876_S0_COLOR_DEPTH_16BPP << 5 | RA8876_S1_COLOR_DEPTH_16BPP << 2 | RA8876_DESTINATION_COLOR_DEPTH_16BPP); //92h
  lcdRegDataWrite(display, RA8876_BTE_CTRL0, RA8876_BTE_ENABLE << 4);                                                                                    //90h
  display_ram_access_prepare(display);
}

//**************************************************************//
//write data after setting
//**************************************************************//
void display_bte_mpu_write_with_chroma_key(Display *display, uint32_t des_addr, uint16_t des_image_width, uint16_t des_x, uint16_t des_y, uint16_t width, uint16_t height, uint16_t chromakey_color)
{
  uint16_t i, j;
  bte_DestinationMemoryStartAddr(display, des_addr);
  bte_DestinationImageWidth(display, des_image_width);
  bte_DestinationWindowStartXY(display, des_x, des_y);
  bte_WindowSize(display, width, height);
  display_back_ground_color_16bpp(display, chromakey_color);
  lcdRegDataWrite(display, RA8876_BTE_CTRL1, RA8876_BTE_MPU_WRITE_WITH_CHROMA);                                                                          //91h
  lcdRegDataWrite(display, RA8876_BTE_COLR, RA8876_S0_COLOR_DEPTH_16BPP << 5 | RA8876_S1_COLOR_DEPTH_16BPP << 2 | RA8876_DESTINATION_COLOR_DEPTH_16BPP); //92h
  lcdRegDataWrite(display, RA8876_BTE_CTRL0, RA8876_BTE_ENABLE << 4);                                                                                    //90h
  display_ram_access_prepare(display);
}

//**************************************************************//
//write data after setting
//**************************************************************//
void display_bte_mpu_write_color_expansion(Display *display, uint32_t des_addr, uint16_t des_image_width, uint16_t des_x, uint16_t des_y, uint16_t width, uint16_t height, uint16_t foreground_color, uint16_t background_color)
{
  uint16_t i, j;
  bte_DestinationMemoryStartAddr(display, des_addr);
  bte_DestinationImageWidth(display, des_image_width);
  bte_DestinationWindowStartXY(display, des_x, des_y);
  bte_WindowSize(display, width, height);
  display_fore_ground_color_16bpp(display, foreground_color);
  display_back_ground_color_16bpp(display, background_color);
  lcdRegDataWrite(display, RA8876_BTE_CTRL1, RA8876_BTE_ROP_BUS_WIDTH8 << 4 | RA8876_BTE_MPU_WRITE_COLOR_EXPANSION);                                     //91h
  lcdRegDataWrite(display, RA8876_BTE_COLR, RA8876_S0_COLOR_DEPTH_16BPP << 5 | RA8876_S1_COLOR_DEPTH_16BPP << 2 | RA8876_DESTINATION_COLOR_DEPTH_16BPP); //92h
  lcdRegDataWrite(display, RA8876_BTE_CTRL0, RA8876_BTE_ENABLE << 4);                                                                                    //90h
  display_ram_access_prepare(display);
}

//**************************************************************//
/*background_color do not set the same as foreground_color*/
//write data after setting
//**************************************************************//
void display_bte_mpu_write_color_expansion_with_chroma_key(Display *display, uint32_t des_addr, uint16_t des_image_width, uint16_t des_x, uint16_t des_y, uint16_t width, uint16_t height, uint16_t foreground_color, uint16_t background_color)
{
  uint16_t i, j;
  bte_DestinationMemoryStartAddr(display, des_addr);
  bte_DestinationImageWidth(display, des_image_width);
  bte_DestinationWindowStartXY(display, des_x, des_y);
  bte_WindowSize(display, width, height);
  display_fore_ground_color_16bpp(display, foreground_color);
  display_back_ground_color_16bpp(display, background_color);
  lcdRegDataWrite(display, RA8876_BTE_CTRL1, RA8876_BTE_ROP_BUS_WIDTH8 << 4 | RA8876_BTE_MPU_WRITE_COLOR_EXPANSION_WITH_CHROMA);                         //91h
  lcdRegDataWrite(display, RA8876_BTE_COLR, RA8876_S0_COLOR_DEPTH_16BPP << 5 | RA8876_S1_COLOR_DEPTH_16BPP << 2 | RA8876_DESTINATION_COLOR_DEPTH_16BPP); //92h
  lcdRegDataWrite(display, RA8876_BTE_CTRL0, RA8876_BTE_ENABLE << 4);                                                                                    //90h
  display_ram_access_prepare(display);
}
//**************************************************************//
//**************************************************************//
void display_bte_pattern_fill(Display *display, uint8_t p8x8or16x16, uint32_t s0_addr, uint16_t s0_image_width, uint16_t s0_x, uint16_t s0_y,
                              uint32_t des_addr, uint16_t des_image_width, uint16_t des_x, uint16_t des_y, uint16_t width, uint16_t height)
{
  bte_Source0_MemoryStartAddr(display, s0_addr);
  bte_Source0_ImageWidth(display, s0_image_width);
  bte_Source0_WindowStartXY(display, s0_x, s0_y);
  bte_DestinationMemoryStartAddr(display, des_addr);
  bte_DestinationImageWidth(display, des_image_width);
  bte_DestinationWindowStartXY(display, des_x, des_y);
  bte_WindowSize(display, width, height);
  lcdRegDataWrite(display, RA8876_BTE_CTRL1, RA8876_BTE_ROP_CODE_12 << 4 | RA8876_BTE_PATTERN_FILL_WITH_ROP);                                            //91h
  lcdRegDataWrite(display, RA8876_BTE_COLR, RA8876_S0_COLOR_DEPTH_16BPP << 5 | RA8876_S1_COLOR_DEPTH_16BPP << 2 | RA8876_DESTINATION_COLOR_DEPTH_16BPP); //92h

  if (p8x8or16x16 == 0)
    lcdRegDataWrite(display, RA8876_BTE_CTRL0, RA8876_BTE_ENABLE << 4 | RA8876_PATTERN_FORMAT8X8); //90h
  else
    lcdRegDataWrite(display, RA8876_BTE_CTRL0, RA8876_BTE_ENABLE << 4 | RA8876_PATTERN_FORMAT16X16); //90h

  check2dBusy(display);
}
//**************************************************************//
//**************************************************************//
void display_bte_pattern_fill_with_chroma_key(Display *display, uint8_t p8x8or16x16, uint32_t s0_addr, uint16_t s0_image_width, uint16_t s0_x, uint16_t s0_y,
                                              uint32_t des_addr, uint16_t des_image_width, uint16_t des_x, uint16_t des_y, uint16_t width, uint16_t height, uint16_t chromakey_color)
{
  bte_Source0_MemoryStartAddr(display, s0_addr);
  bte_Source0_ImageWidth(display, s0_image_width);
  bte_Source0_WindowStartXY(display, s0_x, s0_y);
  bte_DestinationMemoryStartAddr(display, des_addr);
  bte_DestinationImageWidth(display, des_image_width);
  bte_DestinationWindowStartXY(display, des_x, des_y);
  bte_WindowSize(display, width, height);
  display_back_ground_color_16bpp(display, chromakey_color);
  lcdRegDataWrite(display, RA8876_BTE_CTRL1, RA8876_BTE_ROP_CODE_12 << 4 | RA8876_BTE_PATTERN_FILL_WITH_CHROMA);                                         //91h
  lcdRegDataWrite(display, RA8876_BTE_COLR, RA8876_S0_COLOR_DEPTH_16BPP << 5 | RA8876_S1_COLOR_DEPTH_16BPP << 2 | RA8876_DESTINATION_COLOR_DEPTH_16BPP); //92h
  if (p8x8or16x16 == 0)
    lcdRegDataWrite(display, RA8876_BTE_CTRL0, RA8876_BTE_ENABLE << 4 | RA8876_PATTERN_FORMAT8X8); //90h
  else
    lcdRegDataWrite(display, RA8876_BTE_CTRL0, RA8876_BTE_ENABLE << 4 | RA8876_PATTERN_FORMAT16X16); //90h
  check2dBusy(display);
}

//**************************************************************//
/*These 8 bits determine prescaler value for Timer 0 and 1.*/
/*Time base is ¡°Core_Freq / (Prescaler + 1)¡±*/
//**************************************************************//
void pwm_Prescaler(Display *display, uint8_t prescaler)
{
  lcdRegDataWrite(display, RA8876_PSCLR, prescaler); //84h
}
//**************************************************************//
//**************************************************************//
void pwm_ClockMuxReg(Display *display, uint8_t pwm1_clk_div, uint8_t pwm0_clk_div, uint8_t xpwm1_ctrl, uint8_t xpwm0_ctrl)
{
  lcdRegDataWrite(display, RA8876_PMUXR, pwm1_clk_div << 6 | pwm0_clk_div << 4 | xpwm1_ctrl << 2 | xpwm0_ctrl); //85h
}
//**************************************************************//
//**************************************************************//
void pwm_Configuration(Display *display, uint8_t pwm1_inverter, uint8_t pwm1_auto_reload, uint8_t pwm1_start, uint8_t pwm0_dead_zone, uint8_t pwm0_inverter, uint8_t pwm0_auto_reload, uint8_t pwm0_start)
{
  lcdRegDataWrite(display, RA8876_PCFGR, pwm1_inverter << 6 | pwm1_auto_reload << 5 | pwm1_start << 4 | pwm0_dead_zone << 3 | pwm0_inverter << 2 | pwm0_auto_reload << 1 | pwm0_start); //86h
}
//**************************************************************//
//**************************************************************//
void pwm0_Duty(Display *display, uint16_t duty)
{
  lcdRegDataWrite(display, RA8876_TCMPB0L, duty);      //88h
  lcdRegDataWrite(display, RA8876_TCMPB0H, duty >> 8); //89h
}
//**************************************************************//
//**************************************************************//
void pwm0_ClocksPerPeriod(Display *display, uint16_t clocks_per_period)
{
  lcdRegDataWrite(display, RA8876_TCNTB0L, clocks_per_period);      //8ah
  lcdRegDataWrite(display, RA8876_TCNTB0H, clocks_per_period >> 8); //8bh
}
//**************************************************************//
//**************************************************************//
void pwm1_Duty(Display *display, uint16_t duty)
{
  lcdRegDataWrite(display, RA8876_TCMPB1L, duty);      //8ch
  lcdRegDataWrite(display, RA8876_TCMPB1H, duty >> 8); //8dh
}
//**************************************************************//
//**************************************************************//
void pwm1_ClocksPerPeriod(Display *display, uint16_t clocks_per_period)
{
  lcdRegDataWrite(display, RA8876_TCNTB1L, clocks_per_period);      //8eh
  lcdRegDataWrite(display, RA8876_TCNTB1F, clocks_per_period >> 8); //8fh
}
//**************************************************************//
//**************************************************************//
void display_ram_access_prepare(Display *display)
{
  lcdRegWrite(display, RA8876_MRWDP); //04h
}
//**************************************************************//
//**************************************************************//
void display_fore_ground_color_16bpp(Display *display, uint16_t color)
{
  lcdRegDataWrite(display, RA8876_FGCR, color >> 8); //d2h
  lcdRegDataWrite(display, RA8876_FGCG, color >> 3); //d3h
  lcdRegDataWrite(display, RA8876_FGCB, color << 3); //d4h
}
//**************************************************************//
//**************************************************************//
void display_back_ground_color_16bpp(Display *display, uint16_t color)
{
  lcdRegDataWrite(display, RA8876_BGCR, color >> 8); //d5h
  lcdRegDataWrite(display, RA8876_BGCG, color >> 3); //d6h
  lcdRegDataWrite(display, RA8876_BGCB, color << 3); //d7h
}

/*graphic function*/
//**************************************************************//
/* If use ra8876 graphic mode function turns on */
//**************************************************************//
void display_graphic_mode(Display *display, bool on)
{
  if (on)
    lcdRegDataWrite(display, RA8876_ICR, RA8877_LVDS_FORMAT << 3 | RA8876_GRAPHIC_MODE << 2 | RA8876_MEMORY_SELECT_IMAGE); //03h  //switch to graphic mode
  else
    lcdRegDataWrite(display, RA8876_ICR, RA8877_LVDS_FORMAT << 3 | RA8876_TEXT_MODE << 2 | RA8876_MEMORY_SELECT_IMAGE); //03h  //switch back to text mode
}
//**************************************************************//
/*support serial IF to write 16bpp pixel*/
//**************************************************************//
void display_put_pixel_16bpp(Display *display, uint16_t x, uint16_t y, uint16_t color)
{
  display_set_pixel_cursor(display, x, y);
  display_ram_access_prepare(display);
  //checkWriteFifoNotFull();//if high speed mcu and without Xnwait check
  //lcdDataWrite(color);
  //lcdDataWrite(color>>8);
  lcdDataWrite16bbp(display, color);
  //checkWriteFifoEmpty();//if high speed mcu and without Xnwait check
}
//**************************************************************//
/*support ra8876 serial IF to write 16bpp(RGB565) picture data for user operation */
//**************************************************************//
void display_put_picture_16bpp(Display *display, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
  uint16_t i, j;
  display_active_window_xy(display, x, y);
  display_active_window_wh(display, width, height);
  display_set_pixel_cursor(display, x, y);
  display_ram_access_prepare(display);
}
//**************************************************************//
/*support serial IF to write 16bpp(RGB565) picture data byte format from data pointer*/
//**************************************************************//
void display_put_picture_16bpp_data(Display *display, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const unsigned char *data)
{
  uint16_t i, j;

  display_active_window_xy(display, x, y);
  display_active_window_wh(display, width, height);
  display_set_pixel_cursor(display, x, y);
  display_ram_access_prepare(display);
  for (j = 0; j < height; j++)
  {
    for (i = 0; i < width; i++)
    {
      //checkWriteFifoNotFull();//if high speed mcu and without Xnwait check
      lcdDataWrite(display, *data);
      data++;
      //checkWriteFifoNotFull();//if high speed mcu and without Xnwait check
      lcdDataWrite(display, *data);
      data++;
    }
  }
  checkWriteFifoEmpty(display); //if high speed mcu and without Xnwait check
  display_active_window_xy(display, 0, 0);
  display_active_window_wh(display, SCREEN_WIDTH, SCREEN_HEIGHT);
}

/*text function*/
//**************************************************************//
/* If use ra8876 text mode function turns on */
//**************************************************************//
void display_text_mode(Display *display, bool on)
{
  if (on)
    lcdRegDataWrite(display, RA8876_ICR, RA8877_LVDS_FORMAT << 3 | RA8876_TEXT_MODE << 2 | RA8876_MEMORY_SELECT_IMAGE); //03h  //switch to text mode
  else
    lcdRegDataWrite(display, RA8876_ICR, RA8877_LVDS_FORMAT << 3 | RA8876_GRAPHIC_MODE << 2 | RA8876_MEMORY_SELECT_IMAGE); //03h  //switch back to graphic mode
}
//**************************************************************//
//**************************************************************//
void display_text_color(Display *display, uint16_t foreground_color, uint16_t background_color)
{
  display_fore_ground_color_16bpp(display, foreground_color);
  display_back_ground_color_16bpp(display, background_color);
}
//**************************************************************//
//**************************************************************//
void display_set_text_cursor(Display *display, uint16_t x, uint16_t y)
{
  lcdRegDataWrite(display, RA8876_F_CURX0, x);      //63h
  lcdRegDataWrite(display, RA8876_F_CURX1, x >> 8); //64h
  lcdRegDataWrite(display, RA8876_F_CURY0, y);      //65h
  lcdRegDataWrite(display, RA8876_F_CURY1, y >> 8); //66h
}
//**************************************************************//
/* source_select = 0 : internal CGROM,  source_select = 1: external CGROM, source_select = 2: user-define*/
/* size_select = 0 : 8*16/16*16, size_select = 1 : 12*24/24*24, size_select = 2 : 16*32/32*32  */
/* iso_select = 0 : iso8859-1, iso_select = 1 : iso8859-2, iso_select = 2 : iso8859-4, iso_select = 3 : iso8859-5*/
//**************************************************************//
void display_set_text_parameter1(Display *display, uint8_t source_select, uint8_t size_select, uint8_t iso_select) //cch
{
  lcdRegDataWrite(display, RA8876_CCR0, source_select << 6 | size_select << 4 | iso_select); //cch
}
//**************************************************************//
/*align = 0 : full alignment disable, align = 1 : full alignment enable*/
/*chroma_key = 0 : text with chroma key disable, chroma_key = 1 : text with chroma key enable*/
/* width_enlarge and height_enlarge can be set 0~3, (00b: X1) (01b : X2)  (10b : X3)  (11b : X4)*/
//**************************************************************//
void display_set_text_parameter2(Display *display, uint8_t align, uint8_t chroma_key, uint8_t width_enlarge, uint8_t height_enlarge)
{
  lcdRegDataWrite(display, RA8876_CCR1, align << 7 | chroma_key << 6 | width_enlarge << 2 | height_enlarge); //cdh
}
//**************************************************************//
//**************************************************************//
void display_genitop_character_rom_parameter(Display *display, uint8_t scs_select, uint8_t clk_div, uint8_t rom_select, uint8_t character_select, uint8_t gt_width)
{
  if (scs_select == 0)
    lcdRegDataWrite(display, RA8876_SFL_CTRL, RA8876_SERIAL_FLASH_SELECT0 << 7 | RA8876_SERIAL_FLASH_FONT_MODE << 6 | RA8876_SERIAL_FLASH_ADDR_24BIT << 5 | RA8876_FOLLOW_RA8876_MODE << 4 | RA8876_SPI_FAST_READ_8DUMMY); //b7h
  if (scs_select == 1)
    lcdRegDataWrite(display, RA8876_SFL_CTRL, RA8876_SERIAL_FLASH_SELECT1 << 7 | RA8876_SERIAL_FLASH_FONT_MODE << 6 | RA8876_SERIAL_FLASH_ADDR_24BIT << 5 | RA8876_FOLLOW_RA8876_MODE << 4 | RA8876_SPI_FAST_READ_8DUMMY); //b7h

  lcdRegDataWrite(display, RA8876_SPI_DIVSOR, clk_div); //bbh

  lcdRegDataWrite(display, RA8876_GTFNT_SEL, rom_select << 5);                 //ceh
  lcdRegDataWrite(display, RA8876_GTFNT_CR, character_select << 3 | gt_width); //cfh
}
//**************************************************************//
//support ra8876 internal font and external string font code write from data pointer
//**************************************************************//
void display_draw_string(Display *display, const uint16_t x0, const uint16_t y0, const char *str)
{
  display_text_mode(display, true);
  display_set_text_cursor(display, x0, y0);
  display_ram_access_prepare(display);
  while (*str != '\0')
  {
    checkWriteFifoNotFull(display);
    lcdDataWrite(display, *str);
    ++str;
  }
  check2dBusy(display);
  display_text_mode(display, false);
}

/*draw function*/
//**************************************************************//
//**************************************************************//
void display_draw_line(Display *display, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
  display_fore_ground_color_16bpp(display, color);
  lcdRegDataWrite(display, RA8876_DLHSR0, x0);             //68h
  lcdRegDataWrite(display, RA8876_DLHSR1, x0 >> 8);        //69h
  lcdRegDataWrite(display, RA8876_DLVSR0, y0);             //6ah
  lcdRegDataWrite(display, RA8876_DLVSR1, y0 >> 8);        //6bh
  lcdRegDataWrite(display, RA8876_DLHER0, x1);             //6ch
  lcdRegDataWrite(display, RA8876_DLHER1, x1 >> 8);        //6dh
  lcdRegDataWrite(display, RA8876_DLVER0, y1);             //6eh
  lcdRegDataWrite(display, RA8876_DLVER1, y1 >> 8);        //6fh
  lcdRegDataWrite(display, RA8876_DCR0, RA8876_DRAW_LINE); //67h,0x80
  check2dBusy(display);
}
//**************************************************************//
//**************************************************************//
void display_draw_square(Display *display, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
  display_fore_ground_color_16bpp(display, color);
  lcdRegDataWrite(display, RA8876_DLHSR0, x0);               //68h
  lcdRegDataWrite(display, RA8876_DLHSR1, x0 >> 8);          //69h
  lcdRegDataWrite(display, RA8876_DLVSR0, y0);               //6ah
  lcdRegDataWrite(display, RA8876_DLVSR1, y0 >> 8);          //6bh
  lcdRegDataWrite(display, RA8876_DLHER0, x1);               //6ch
  lcdRegDataWrite(display, RA8876_DLHER1, x1 >> 8);          //6dh
  lcdRegDataWrite(display, RA8876_DLVER0, y1);               //6eh
  lcdRegDataWrite(display, RA8876_DLVER1, y1 >> 8);          //6fh
  lcdRegDataWrite(display, RA8876_DCR1, RA8876_DRAW_SQUARE); //76h,0xa0
  check2dBusy(display);
}
//**************************************************************//
//**************************************************************//
void display_draw_square_fill(Display *display, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
  display_fore_ground_color_16bpp(display, color);
  lcdRegDataWrite(display, RA8876_DLHSR0, x0);                    //68h
  lcdRegDataWrite(display, RA8876_DLHSR1, x0 >> 8);               //69h
  lcdRegDataWrite(display, RA8876_DLVSR0, y0);                    //6ah
  lcdRegDataWrite(display, RA8876_DLVSR1, y0 >> 8);               //6bh
  lcdRegDataWrite(display, RA8876_DLHER0, x1);                    //6ch
  lcdRegDataWrite(display, RA8876_DLHER1, x1 >> 8);               //6dh
  lcdRegDataWrite(display, RA8876_DLVER0, y1);                    //6eh
  lcdRegDataWrite(display, RA8876_DLVER1, y1 >> 8);               //6fh
  lcdRegDataWrite(display, RA8876_DCR1, RA8876_DRAW_SQUARE_FILL); //76h,0xa0
  check2dBusy(display);
}
//**************************************************************//
//**************************************************************//
void display_draw_circle_square(Display *display, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t xr, uint16_t yr, uint16_t color)
{
  display_fore_ground_color_16bpp(display, color);
  lcdRegDataWrite(display, RA8876_DLHSR0, x0);                      //68h
  lcdRegDataWrite(display, RA8876_DLHSR1, x0 >> 8);                 //69h
  lcdRegDataWrite(display, RA8876_DLVSR0, y0);                      //6ah
  lcdRegDataWrite(display, RA8876_DLVSR1, y0 >> 8);                 //6bh
  lcdRegDataWrite(display, RA8876_DLHER0, x1);                      //6ch
  lcdRegDataWrite(display, RA8876_DLHER1, x1 >> 8);                 //6dh
  lcdRegDataWrite(display, RA8876_DLVER0, y1);                      //6eh
  lcdRegDataWrite(display, RA8876_DLVER1, y1 >> 8);                 //6fh
  lcdRegDataWrite(display, RA8876_ELL_A0, xr);                      //77h
  lcdRegDataWrite(display, RA8876_ELL_A1, xr >> 8);                 //79h
  lcdRegDataWrite(display, RA8876_ELL_B0, yr);                      //7ah
  lcdRegDataWrite(display, RA8876_ELL_B1, yr >> 8);                 //7bh
  lcdRegDataWrite(display, RA8876_DCR1, RA8876_DRAW_CIRCLE_SQUARE); //76h,0xb0
  check2dBusy(display);
}
//**************************************************************//
//**************************************************************//
void display_draw_circle_square_fill(Display *display, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t xr, uint16_t yr, uint16_t color)
{
  display_fore_ground_color_16bpp(display, color);
  lcdRegDataWrite(display, RA8876_DLHSR0, x0);                           //68h
  lcdRegDataWrite(display, RA8876_DLHSR1, x0 >> 8);                      //69h
  lcdRegDataWrite(display, RA8876_DLVSR0, y0);                           //6ah
  lcdRegDataWrite(display, RA8876_DLVSR1, y0 >> 8);                      //6bh
  lcdRegDataWrite(display, RA8876_DLHER0, x1);                           //6ch
  lcdRegDataWrite(display, RA8876_DLHER1, x1 >> 8);                      //6dh
  lcdRegDataWrite(display, RA8876_DLVER0, y1);                           //6eh
  lcdRegDataWrite(display, RA8876_DLVER1, y1 >> 8);                      //6fh
  lcdRegDataWrite(display, RA8876_ELL_A0, xr);                           //77h
  lcdRegDataWrite(display, RA8876_ELL_A1, xr >> 8);                      //78h
  lcdRegDataWrite(display, RA8876_ELL_B0, yr);                           //79h
  lcdRegDataWrite(display, RA8876_ELL_B1, yr >> 8);                      //7ah
  lcdRegDataWrite(display, RA8876_DCR1, RA8876_DRAW_CIRCLE_SQUARE_FILL); //76h,0xf0
  check2dBusy(display);
}
//**************************************************************//
//**************************************************************//
void display_draw_triangle(Display *display, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  display_fore_ground_color_16bpp(display, color);
  lcdRegDataWrite(display, RA8876_DLHSR0, x0);                 //68h
  lcdRegDataWrite(display, RA8876_DLHSR1, x0 >> 8);            //69h
  lcdRegDataWrite(display, RA8876_DLVSR0, y0);                 //6ah
  lcdRegDataWrite(display, RA8876_DLVSR1, y0 >> 8);            //6bh
  lcdRegDataWrite(display, RA8876_DLHER0, x1);                 //6ch
  lcdRegDataWrite(display, RA8876_DLHER1, x1 >> 8);            //6dh
  lcdRegDataWrite(display, RA8876_DLVER0, y1);                 //6eh
  lcdRegDataWrite(display, RA8876_DLVER1, y1 >> 8);            //6fh
  lcdRegDataWrite(display, RA8876_DTPH0, x2);                  //70h
  lcdRegDataWrite(display, RA8876_DTPH1, x2 >> 8);             //71h
  lcdRegDataWrite(display, RA8876_DTPV0, y2);                  //72h
  lcdRegDataWrite(display, RA8876_DTPV1, y2 >> 8);             //73h
  lcdRegDataWrite(display, RA8876_DCR0, RA8876_DRAW_TRIANGLE); //67h,0x82
  check2dBusy(display);
}
//**************************************************************//
//**************************************************************//
void display_draw_triangle_fill(Display *display, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  display_fore_ground_color_16bpp(display, color);
  lcdRegDataWrite(display, RA8876_DLHSR0, x0);                      //68h
  lcdRegDataWrite(display, RA8876_DLHSR1, x0 >> 8);                 //69h
  lcdRegDataWrite(display, RA8876_DLVSR0, y0);                      //6ah
  lcdRegDataWrite(display, RA8876_DLVSR1, y0 >> 8);                 //6bh
  lcdRegDataWrite(display, RA8876_DLHER0, x1);                      //6ch
  lcdRegDataWrite(display, RA8876_DLHER1, x1 >> 8);                 //6dh
  lcdRegDataWrite(display, RA8876_DLVER0, y1);                      //6eh
  lcdRegDataWrite(display, RA8876_DLVER1, y1 >> 8);                 //6fh
  lcdRegDataWrite(display, RA8876_DTPH0, x2);                       //70h
  lcdRegDataWrite(display, RA8876_DTPH1, x2 >> 8);                  //71h
  lcdRegDataWrite(display, RA8876_DTPV0, y2);                       //72h
  lcdRegDataWrite(display, RA8876_DTPV1, y2 >> 8);                  //73h
  lcdRegDataWrite(display, RA8876_DCR0, RA8876_DRAW_TRIANGLE_FILL); //67h,0xa2
  check2dBusy(display);
}
//**************************************************************//
//**************************************************************//
void display_draw_circle(Display *display, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
  display_fore_ground_color_16bpp(display, color);
  lcdRegDataWrite(display, RA8876_DEHR0, x0);                //7bh
  lcdRegDataWrite(display, RA8876_DEHR1, x0 >> 8);           //7ch
  lcdRegDataWrite(display, RA8876_DEVR0, y0);                //7dh
  lcdRegDataWrite(display, RA8876_DEVR1, y0 >> 8);           //7eh
  lcdRegDataWrite(display, RA8876_ELL_A0, r);                //77h
  lcdRegDataWrite(display, RA8876_ELL_A1, r >> 8);           //78h
  lcdRegDataWrite(display, RA8876_ELL_B0, r);                //79h
  lcdRegDataWrite(display, RA8876_ELL_B1, r >> 8);           //7ah
  lcdRegDataWrite(display, RA8876_DCR1, RA8876_DRAW_CIRCLE); //76h,0x80
  check2dBusy(display);
}
//**************************************************************//
//**************************************************************//
void display_draw_circle_fill(Display *display, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
  display_fore_ground_color_16bpp(display, color);
  lcdRegDataWrite(display, RA8876_DEHR0, x0);                     //7bh
  lcdRegDataWrite(display, RA8876_DEHR1, x0 >> 8);                //7ch
  lcdRegDataWrite(display, RA8876_DEVR0, y0);                     //7dh
  lcdRegDataWrite(display, RA8876_DEVR1, y0 >> 8);                //7eh
  lcdRegDataWrite(display, RA8876_ELL_A0, r);                     //77h
  lcdRegDataWrite(display, RA8876_ELL_A1, r >> 8);                //78h
  lcdRegDataWrite(display, RA8876_ELL_B0, r);                     //79h
  lcdRegDataWrite(display, RA8876_ELL_B1, r >> 8);                //7ah
  lcdRegDataWrite(display, RA8876_DCR1, RA8876_DRAW_CIRCLE_FILL); //76h,0xc0
  check2dBusy(display);
}
//**************************************************************//
//**************************************************************//
void display_draw_ellipse(Display *display, uint16_t x0, uint16_t y0, uint16_t xr, uint16_t yr, uint16_t color)
{
  display_fore_ground_color_16bpp(display, color);
  lcdRegDataWrite(display, RA8876_DEHR0, x0);                 //7bh
  lcdRegDataWrite(display, RA8876_DEHR1, x0 >> 8);            //7ch
  lcdRegDataWrite(display, RA8876_DEVR0, y0);                 //7dh
  lcdRegDataWrite(display, RA8876_DEVR1, y0 >> 8);            //7eh
  lcdRegDataWrite(display, RA8876_ELL_A0, xr);                //77h
  lcdRegDataWrite(display, RA8876_ELL_A1, xr >> 8);           //78h
  lcdRegDataWrite(display, RA8876_ELL_B0, yr);                //79h
  lcdRegDataWrite(display, RA8876_ELL_B1, yr >> 8);           //7ah
  lcdRegDataWrite(display, RA8876_DCR1, RA8876_DRAW_ELLIPSE); //76h,0x80
  check2dBusy(display);
}

void display_write_gt9271_touch_register(Display *display, uint16_t regAddr, uint8_t *val, uint16_t cnt)
{
  uint16_t i = 0;
  display->i2cBus.start();
  display->i2cBus.write(display->i2c_addr_write);
  display->i2cBus.write(regAddr >> 8);
  display->i2cBus.write(regAddr);
  for (i = 0; i < cnt; i++, val++)
  {
    display->i2cBus.write(*val);
  }

  display->i2cBus.stop();
}

uint8_t display_gt9271_send_cfg(Display *display, uint8_t *buf, uint16_t cfg_len)
{
  display_write_gt9271_touch_register(display, 0x8047, buf, cfg_len);
  _waitms(10); //wait 10ms
}

uint8_t display_read_gt9271_touch_addr(Display *display, uint16_t regAddr, uint8_t *pBuf, uint8_t len)
{
  uint8_t i;
  display->i2cBus.start();

  display->i2cBus.write(display->i2c_addr_write);
  display->i2cBus.write(regAddr >> 8);
  display->i2cBus.write(regAddr);
  display->i2cBus.start();
  display->i2cBus.write(display->i2c_addr_read);
  for (i = 0; i < len; i++)
  {
    if (i == (len - 1))
    {
      pBuf[i] = display->i2cBus.read(1);
      break;
    }
    pBuf[i] = display->i2cBus.read(0);
  }
  display->i2cBus.stop();
  return i;
}

uint8_t readGT9271TouchLocation(Display *display, TouchLocation *pLoc, uint8_t num)
{
  uint8_t retVal = 0;
  uint8_t i = 0;
  uint8_t k = 0;
  uint8_t ss[1];
  do
  {

    if (!pLoc)
      break; // must have a buffer
    if (!num)
      break; // must be able to take at least one
    ss[0] = 0;
    display_read_gt9271_touch_addr(display, 0x814e, ss, 1);
    uint8_t status = ss[0];
    if ((status & 0x0f) == 0)
      break; // no points detected
    uint8_t hitPoints = status & 0x0f;

    uint8_t tbuf[32];
    uint8_t tbuf1[32];
    uint8_t tbuf2[16];
    display_read_gt9271_touch_addr(display, 0x8150, tbuf, 32);
    display_read_gt9271_touch_addr(display, 0x8150 + 32, tbuf1, 32);
    display_read_gt9271_touch_addr(display, 0x8150 + 64, tbuf2, 16);

    if (hitPoints <= 4)
    {
      for (k = 0, i = 0;
           (i < 4 * 8) && (k < num); k++, i += 8)
      {
        pLoc[k].x = tbuf[i + 1] << 8 | tbuf[i + 0];
        pLoc[k].y = tbuf[i + 3] << 8 | tbuf[i + 2];
      }
    }
    if (hitPoints > 4)
    {
      for (k = 0, i = 0;
           (i < 4 * 8) && (k < num); k++, i += 8)
      {
        pLoc[k].x = tbuf[i + 1] << 8 | tbuf[i + 0];
        pLoc[k].y = tbuf[i + 3] << 8 | tbuf[i + 2];
      }

      for (k = 4, i = 0;
           (i < 4 * 8) && (k < num); k++, i += 8)
      {
        pLoc[k].x = tbuf1[i + 1] << 8 | tbuf1[i + 0];
        pLoc[k].y = tbuf1[i + 3] << 8 | tbuf1[i + 2];
      }
    }

    if (hitPoints > 8)
    {
      for (k = 0, i = 0;
           (i < 4 * 8) && (k < num); k++, i += 8)
      {
        pLoc[k].x = tbuf[i + 1] << 8 | tbuf[i + 0];
        pLoc[k].y = tbuf[i + 3] << 8 | tbuf[i + 2];
      }

      for (k = 4, i = 0;
           (i < 4 * 8) && (k < num); k++, i += 8)
      {
        pLoc[k].x = tbuf1[i + 1] << 8 | tbuf1[i + 0];
        pLoc[k].y = tbuf1[i + 3] << 8 | tbuf1[i + 2];
      }

      for (k = 8, i = 0;
           (i < 2 * 8) && (k < num); k++, i += 8)
      {
        pLoc[k].x = tbuf2[i + 1] << 8 | tbuf2[i + 0];
        pLoc[k].y = tbuf2[i + 3] << 8 | tbuf2[i + 2];
      }
    }

    retVal = hitPoints;

  } while (0);

  ss[0] = 0;
  display_write_gt9271_touch_register(display, 0x814e, ss, 1);

  return retVal;
}

int display_update_buttons(Display *display, Button *buttons, const int amount)
{
  TouchLocation loc[1];
  int amountPressed = 0;
  if (readGT9271TouchLocation(display, loc, 1) > 0)
  {
    printf("found touch\n");
    //printf("found click: %d,%d   %d %d\n", loc[0].x, loc[0].y, (loc[0].x > buttons[i].xmin) && (loc[0].x < buttons[i].xmax), (loc[0].y > buttons[i].ymin) && (loc[0].y < buttons[i].ymax));
    for (int i = 0; i < amount; i++)
    {
      buttons[i].pressed = false;
      //print("found click: %d,%d   %d %d\n", loc[0].x, loc[0].y, (loc[0].x > buttons[i].xmin) && (loc[0].x < buttons[i].xmax), (loc[0].y > buttons[i].ymin) && (loc[0].y < buttons[i].ymax));
      //print("button range: %d-%d,%d-%d", buttons[i].xmin, buttons[i].xmax, buttons[i].ymin, buttons[i].ymax);
      if (((SCREEN_WIDTH - loc[0].x) > buttons[i].xmin) && ((SCREEN_WIDTH - loc[0].x) < buttons[i].xmax))
      {
        if (((SCREEN_HEIGHT - loc[0].y) > buttons[i].ymin) && ((SCREEN_HEIGHT - loc[0].y) < buttons[i].ymax))
        {
          printf("Button pressed:%d\n", buttons[i].name);
          if ((_getms() - buttons[i].lastPress) > 200)
          {
            printf("pressed\n");
            buttons[i]
                .lastPress = _getms();
            buttons[i].pressed = true;
            amountPressed++;
          }
        }
      }
    }
  }
  return amountPressed;
}

/*
To load image with proper settings: use inkscape to export image with anialiasing turned off, then open in gimp where 
it is exported to jpg using max quality settings (100) and use preview to check for any back "smudging". Finally
use the image took from RA8876 to convert that jpg to bin. 8 characters max with extension of three characters.
*/
void display_load_image(Display *display, Image image)
{
  if (image.page == 1)
  {
    display_canvas_image_start_address(display, PAGE1_START_ADDR);
  }
  else if (image.page == 2)
  {
    display_canvas_image_start_address(display, PAGE2_START_ADDR);
  }
  else if (image.page == 3)
  {
    display_canvas_image_start_address(display, PAGE3_START_ADDR);
  }
  FILE *fp = fopen(image.name, "r");
  if (fp == NULL)
  {
    printf("Error opening file(%s): %d\n", image.name, _geterror());
    display_canvas_image_start_address(display, PAGE1_START_ADDR);
    display_active_window_xy(display, 0, 0);
    display_active_window_wh(display, SCREEN_WIDTH, SCREEN_HEIGHT);
    return;
  }

  display_put_picture_16bpp(display, image.x0, image.y0, image.width, image.height);
  int data;
  int limit = image.width * image.height * 2;
  int count = 0;
  while ((data = fgetc(fp)) != EOF)
  {
    lcdDataWrite(display, data);
    if (count > limit)
    {
      printf("something went wrong...\n");
      break;
    }
    count++;
  }
  fclose(fp);
  display_canvas_image_start_address(display, PAGE1_START_ADDR);
  display_active_window_xy(display, 0, 0);
  display_active_window_wh(display, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void display_bte_memory_copy_image(Display *display, Image image, int xpos, int ypos)
{
  int pageAddr = 0;
  switch (image.page)
  {
  case 1:
    pageAddr = PAGE1_START_ADDR;
    break;
  case 2:
    pageAddr = PAGE2_START_ADDR;
    break;
  case 3:
    pageAddr = PAGE3_START_ADDR;
    break;
  default:
    break;
  }

  if (image.backgroundColor != NULL)
  {
    display_bte_memory_copy_with_chroma_key(display, pageAddr, SCREEN_WIDTH, image.x0, image.y0, PAGE1_START_ADDR, SCREEN_WIDTH, xpos, ypos, image.width, image.height, image.backgroundColor);
  }
  else
  {
    display_bte_memory_copy(display, pageAddr, SCREEN_WIDTH, image.x0, image.y0, PAGE1_START_ADDR, SCREEN_WIDTH, xpos, ypos, image.width, image.height);
  }
}
//display.display_bte_memory_copy(PAGE2_START_ADDR, SCREEN_WIDTH, 0, 0, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, 100, 50);
