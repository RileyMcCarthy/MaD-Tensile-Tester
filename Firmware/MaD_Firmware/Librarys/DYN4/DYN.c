/*
  DYN232M_Arduino_SourceCode.ino - DMM Technology Corp. DYN2 and DYN4 AC Servo drive Arduino UART RS232 communication code.

  Version 1.0 - Release May 2018

  Distributed By: DMM Technology Corp.
                  www.dmm-tech.com
*/

/**/
#include "DYN.h"
#include <stdio.h>

#ifndef SIMULATION
static struct __using("jm_fullduplexserial.spin2") serial;
#else
#include "SimSerial.h"
static SimSerial serial;
#endif

#define Go_Absolute_Pos 0x01
#define Go_Relative_Pos 0x03
#define Is_AbsPos32 0x1b
#define General_Read 0x0e
#define Is_TrqCurrent 0x1E
#define Read_MainGain 0x18
#define Set_MainGain 0x10
#define Set_SpeedGain 0x11
#define Set_IntGain 0x12
#define Set_HighSpeed 0x14
#define Set_HighAccel 0x15
#define Set_Pos_OnRange 0x16
#define Is_MainGain 0x10
#define Is_SpeedGain 0x11
#define Is_IntGain 0x12
#define Is_TrqCons 0x13
#define Is_HighSpeed 0x14
#define Is_HighAccel 0x15
#define Is_Driver_ID 0x16
#define Is_Pos_OnRange 0x17
#define Is_Status 0x19
#define Is_Config 0x1a
// Add additional function code according to user requirement
/*Variables*/
static char InputBuffer[256];                          // Input buffer from RS232,
static char OutputBuffer[256];                         // Output buffer to RS232,
static unsigned char InBfTopPointer, InBfBtmPointer;   // input buffer pointers
static unsigned char OutBfTopPointer, OutBfBtmPointer; // output buffer pointers
static unsigned char Read_Package_Buffer[8], Read_Num, Read_Package_Length, Global_Func;
static unsigned char MotorPosition32Ready_Flag, MotorTorqueCurrentReady_Flag, MainGainRead_Flag;
static unsigned char Driver_MainGain, Driver_SpeedGain, Driver_IntGain, Driver_TrqCons, Driver_HighSpeed, Driver_HighAccel, Driver_ReadID, Driver_Status, Driver_Config, Driver_OnRange;
static long Motor_Pos32, MotorTorqueCurrent;
/*Prototypes*/
static void ReadPackage(void);
static void Get_Function(void);
static long Cal_SignValue(unsigned char One_Package[8]);
static long Cal_Value(unsigned char One_Package[8]);
static void Send_Package(char ID, long Displacement);
static void Make_CRC_Send(unsigned char Plength, unsigned char B[8]);
/**/

/*static void test_dyn4()
{
  printf("Testing dyn\n");

  // move_rel32(DYN1_ADDR, 500); _waitms(5000);

  ReadMotorPosition32(DYN1_ADDR);
  while (!GetMotorPosition32())
    ;
  printf("Axis 1 Encoder Position: %ld\n", Motor_Pos32);

  move_rel32(DYN1_ADDR, 500);
  _waitms(5000);

  ReadMotorPosition32(DYN1_ADDR);
  while (!GetMotorPosition32())
    ;
  printf("Axis 1 Encoder Position: %ld\n", Motor_Pos32);

  move_abs32(DYN1_ADDR, 0);
  _waitms(5000);

  ReadMotorPosition32(DYN1_ADDR);
  while (!GetMotorPosition32())
    ;
  printf("Axis 1 Encoder Position: %ld\n", Motor_Pos32);
}*/

void dyn_init()
{

  serial.start(DYN1_RX, DYN1_TX, 2, 38400);
  // test_dyn4();
}

/////////////////////////// SAMPLE COMMAND FUNCTIONS  ////////////////////////////////////

void move_rel32(char ID, long pos)
{
  char Axis_Num = ID;
  Global_Func = (char)Go_Relative_Pos;
  Send_Package(Axis_Num, pos);
}

void ReadMotorTorqueCurrent(char ID)
{
  Global_Func = General_Read;
  Send_Package(ID, Is_TrqCurrent);
  MotorTorqueCurrentReady_Flag = 0xff;
  while (MotorTorqueCurrentReady_Flag != 0x00)
  {
    ReadPackage();
  }
}

// Must send move command before read position command
void ReadMotorPosition32(char ID)
{
  Global_Func = (char)General_Read;
  Send_Package(ID, Is_AbsPos32);
  MotorPosition32Ready_Flag = 0xff;
}

bool GetMotorPosition32()
{
  ReadPackage();
  return MotorPosition32Ready_Flag == 0x00;
}

void move_abs32(char MotorID, long Pos32)
{
  char Axis_Num = MotorID;
  Global_Func = (char)Go_Absolute_Pos;
  Send_Package(Axis_Num, Pos32);
}

void Turn_const_speed(char ID, long spd)
{
  char Axis_Num = ID;
  Global_Func = 0x0a;
  Send_Package(Axis_Num, spd);
}

////////////////////// DYN232M SERIAL PROTOCOL PACKAGE FUNCTIONS  ///////////////////////////////

static void ReadPackage(void)
{
  unsigned char c, cif;

  while (serial.available() > 0)
  {
    InputBuffer[InBfTopPointer] = serial.rxtime(100); // Load InputBuffer with received packets
    InBfTopPointer++;
  }
  while (InBfBtmPointer != InBfTopPointer)
  {
    c = InputBuffer[InBfBtmPointer];
    InBfBtmPointer++;
    cif = c & 0x80;
    if (cif == 0)
    {
      Read_Num = 0;
      Read_Package_Length = 0;
    }
    if (cif == 0 || Read_Num > 0)
    {
      Read_Package_Buffer[Read_Num] = c;
      Read_Num++;
      if (Read_Num == 2)
      {
        cif = c >> 5;
        cif = cif & 0x03;
        Read_Package_Length = 4 + cif;
        c = 0;
      }
      if (Read_Num == Read_Package_Length)
      {
        Get_Function();
        Read_Num = 0;
        Read_Package_Length = 0;
      }
    }
  }
}

static void Get_Function(void)
{
  char ID, ReceivedFunction_Code, CRC_Check;
  ID = Read_Package_Buffer[0] & 0x7f;
  ReceivedFunction_Code = Read_Package_Buffer[1] & 0x1f;
  CRC_Check = 0;

  for (int i = 0; i < Read_Package_Length - 1; i++)
  {
    CRC_Check += Read_Package_Buffer[i];
  }

  CRC_Check ^= Read_Package_Buffer[Read_Package_Length - 1];
  CRC_Check &= 0x7f;
  if (CRC_Check != 0)
  {
  }
  else
  {
    switch (ReceivedFunction_Code)
    {
    case Is_AbsPos32:
      Motor_Pos32 = Cal_SignValue(Read_Package_Buffer);
      MotorPosition32Ready_Flag = 0x00;
      break;
    case Is_TrqCurrent:
      MotorTorqueCurrent = Cal_SignValue(Read_Package_Buffer);
      break;
    case Is_Status:
      Driver_Status = (char)Cal_SignValue(Read_Package_Buffer);
      // Driver_Status=drive status byte data
      break;
    case Is_Config:
      Driver_Config = (char)Cal_Value(Read_Package_Buffer);
      // Driver_Config = drive configuration setting
      break;
    case Is_MainGain:
      Driver_MainGain = (char)Cal_SignValue(Read_Package_Buffer);
      Driver_MainGain = Driver_MainGain & 0x7f;
      break;
    case Is_SpeedGain:
      Driver_SpeedGain = (char)Cal_SignValue(Read_Package_Buffer);
      Driver_SpeedGain = Driver_SpeedGain & 0x7f;
      break;
    case Is_IntGain:
      Driver_IntGain = (char)Cal_SignValue(Read_Package_Buffer);
      Driver_IntGain = Driver_IntGain & 0x7f;
      break;
    case Is_TrqCons:
      Driver_TrqCons = (char)Cal_SignValue(Read_Package_Buffer);
      Driver_TrqCons = Driver_TrqCons & 0x7f;
      break;
    case Is_HighSpeed:
      Driver_HighSpeed = (char)Cal_SignValue(Read_Package_Buffer);
      Driver_HighSpeed = Driver_HighSpeed & 0x7f;
      break;
    case Is_HighAccel:
      Driver_HighAccel = (char)Cal_SignValue(Read_Package_Buffer);
      Driver_HighAccel = Driver_HighAccel & 0x7f;
      break;
    case Is_Driver_ID:
      Driver_ReadID = ID;
      break;
    case Is_Pos_OnRange:
      Driver_OnRange = (char)Cal_SignValue(Read_Package_Buffer);
      Driver_OnRange = Driver_OnRange & 0x7f;
      break;
    }
  }
}

static long Cal_SignValue(unsigned char One_Package[8]) // Get data with sign - long
{
  unsigned char Package_Length, OneChar, i;
  long Lcmd;
  OneChar = One_Package[1];
  OneChar = OneChar >> 5;
  OneChar = OneChar & 0x03;
  Package_Length = 4 + OneChar;
  OneChar = One_Package[2]; /*First byte 0x7f, bit 6 reprents sign */
  OneChar = OneChar << 1;
  Lcmd = (long)OneChar; /* Sign extended to 32bits */
  Lcmd = Lcmd >> 1;
  for (i = 3; i < Package_Length - 1; i++)
  {
    OneChar = One_Package[i];
    OneChar &= 0x7f;
    Lcmd = Lcmd << 7;
    Lcmd += OneChar;
  }
  return (Lcmd); /* Lcmd : -2^27 ~ 2^27 - 1 */
}

static long Cal_Value(unsigned char One_Package[8])
{
  unsigned char Package_Length, OneChar, i;
  long Lcmd;
  OneChar = One_Package[1];
  OneChar = OneChar >> 5;
  OneChar = OneChar & 0x03;
  Package_Length = 4 + OneChar;

  OneChar = One_Package[2]; /*First byte 0x7f, bit 6 reprents sign      */
  OneChar &= 0x7f;
  Lcmd = (long)OneChar; /*Sign extended to 32bits           */
  for (i = 3; i < Package_Length - 1; i++)
  {
    OneChar = One_Package[i];
    OneChar &= 0x7f;
    Lcmd = Lcmd << 7;
    Lcmd += OneChar;
  }
  return (Lcmd); /*Lcmd : -2^27 ~ 2^27 - 1           */
}

static void Send_Package(char ID, long Displacement)
{
  unsigned char B[8], Package_Length, Function_Code;
  long TempLong;
  B[1] = B[2] = B[3] = B[4] = B[5] = (unsigned char)0x80;
  B[0] = ID & 0x7f;
  Function_Code = Global_Func & 0x1f;
  TempLong = Displacement & 0x0fffffff; // Max 28bits
  B[5] += (unsigned char)TempLong & 0x0000007f;
  TempLong = TempLong >> 7;
  B[4] += (unsigned char)TempLong & 0x0000007f;
  TempLong = TempLong >> 7;
  B[3] += (unsigned char)TempLong & 0x0000007f;
  TempLong = TempLong >> 7;
  B[2] += (unsigned char)TempLong & 0x0000007f;
  Package_Length = 7;
  TempLong = Displacement;
  TempLong = TempLong >> 20;
  if ((TempLong == 0x00000000) || (TempLong == 0xffffffff))
  { // Three byte data
    B[2] = B[3];
    B[3] = B[4];
    B[4] = B[5];
    Package_Length = 6;
  }
  TempLong = Displacement;
  TempLong = TempLong >> 13;
  if ((TempLong == 0x00000000) || (TempLong == 0xffffffff))
  { // Two byte data
    B[2] = B[3];
    B[3] = B[4];
    Package_Length = 5;
  }
  TempLong = Displacement;
  TempLong = TempLong >> 6;
  if ((TempLong == 0x00000000) || (TempLong == 0xffffffff))
  { // One byte data
    B[2] = B[3];
    Package_Length = 4;
  }
  B[1] += (Package_Length - 4) * 32 + Function_Code;
  Make_CRC_Send(Package_Length, B);
}

static void Make_CRC_Send(unsigned char Plength, unsigned char B[8])
{
  unsigned char Error_Check = 0;
  char RS232_HardwareShiftRegister;

  for (int i = 0; i < Plength - 1; i++)
  {
    OutputBuffer[OutBfTopPointer] = B[i];
    OutBfTopPointer++;
    Error_Check += B[i];
  }
  Error_Check = Error_Check | 0x80;
  OutputBuffer[OutBfTopPointer] = Error_Check;
  OutBfTopPointer++;
  while (OutBfBtmPointer != OutBfTopPointer)
  {
    RS232_HardwareShiftRegister = OutputBuffer[OutBfBtmPointer];
    // Serial.print("RS232_HardwareShiftRegister: ");
    // Serial.println(RS232_HardwareShiftRegister, DEC);
    serial.tx(RS232_HardwareShiftRegister);
    OutBfBtmPointer++; // Change to next byte in OutputBuffer to send
  }
}