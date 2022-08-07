// automatically generated by spin2cpp v5.9.14 on Sun Aug 07 14:30:13 2022
// command line: C:\spin2cpp\spin2cpp.exe --p2 -o MaD_Firmware/Librarys/I2C/jm_i2c Mad_Firmware/Librarys/I2C/jm_i2c.spin2 

#ifndef jm_i2c_Class_Defined__
#define jm_i2c_Class_Defined__

#include <stdint.h>

class jm_i2c {
public:
/*  fixed io pins  */
/*  I  */
// programming / debug
  static const int RX1 = 63;
/*  O  */
  static const int TX1 = 62;
/*  O  */
// serial flash
  static const int SF_CS = 61;
/*  O  */
  static const int SF_SCK = 60;
/*  O  */
  static const int SF_SDO = 59;
/*  I  */
  static const int SF_SDI = 58;
  static const int PU_NONE = 0;
  static const int PU_1K5 = 1;
  static const int PU_3K3 = 2;
// pull-up options
  static const int PU_15K = 3;
  static const int ACK = 0;
// ACK==0, NAK==1''                                ' I2C acknowledge options
  static const int NAK = 1;
  static void 	null(void);
  void 	setup(int32_t scl, int32_t sda, int32_t khz, int32_t pullup);
  int32_t 	present(int32_t slaveid);
  void 	wait(int32_t slaveid);
  void 	start(void);
  int32_t 	write(int32_t i2cbyte);
  int32_t 	wr_block(int32_t p_block, int32_t count);
  int32_t 	read(int32_t ackbit);
  void 	rd_block(int32_t p_block, int32_t count, int32_t ackbit);
  void 	stop(void);
private:
// i2c bus pins
  volatile int32_t 	sclpin;
  volatile int32_t 	sdapin;
// system ticks in 1/4 period
  volatile int32_t 	clktix;
};

#endif