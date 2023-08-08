#include <stdint.h>
#include "Main/MaD.h"
#include "JSON.h"
#include "Utility/ControlSystem.h"
#include "Utility/Debug.h"

int _stdio_debug_lock;

/**
 * @brief Starts the display, motion control, and all MaD board related tasks. Should never exit
 *
 */

void test_sd_card()
{
  mount("/sd", _vfs_open_sdcard());
  _pinl(56);
  int lastblink = 0;
  for (int j=0;j<100;j++)
  {
    FILE *fp = fopen("/sd/test.txt", "w");
    if (fp == NULL)
    {
      printf("Failed to open file for writing\n");
      return;
    }
    printf("Writing to file\n");
    for (int i=0;i<10000;i++)
    {
      int len = fprintf(fp, "Hello World %d\n", i);
      if (len < 0)
      {
        printf("Failed to write to file: %d\n",i);
        return;
      }
      printf("Wrote to file %d\n", i);
      _waitms(1);
      if ((_getms() - lastblink) > 1000)
      {
        _pinnot(56);
        lastblink = _getms();
      }
    }
    fclose(fp);
    printf("Finished writing to file\n");
  }
}

#include "i2cNavKey.h"
static NavKey navkey1;
void test_navkey()
{
    printf("Starting navkey test\n");
    navkey_begin(&navkey1, NAVKEY_SCL, NAVKEY_SDA, NAVKEY_I2C_ADDR, INT_DATA | WRAP_DISABLE | DIRE_RIGHT | IPUP_ENABLE);
    navkey_write_counter(&navkey1, 0);              /* Reset the counter value */
    navkey_write_max(&navkey1, 100000);             /* Set the maximum threshold*/
    navkey_write_min(&navkey1, -100000);            /* Set the minimum threshold */
    navkey_write_step(&navkey1, 1);                 /* Set the step to 1*/
    navkey_write_double_push_period(&navkey1, 300); /*Set a period for the double push of 300ms */
    navkey_write_counter(&navkey1, 0);              // reset counter to position
    printf("Navkey test started\n");
    navkey1.status.UPR = 0;
    while(1)
    {
      navkey_update_status(&navkey1);
      NavKeyStatus *status = &(navkey1.status);
      printf("NAVKEY=UPR:%d,UPP:%d,DNR:%d,DNP:%d,RTR:%d,RTP:%d,LTR:%d,LTP:%d,CTRR:%d,CTRP:%d,CTRDP:%d,RINC:%d,RDEC:%d,RMAX:%d,RMIN:%d\n",
                                    status->UPR, status->UPP, status->DNR, status->DNP, status->RTR, status->RTP, status->LTR, status->LTP, status->CTRR, status->CTRP, status->CTRDP, status->RINC, status->RDEC, status->RMAX, status->RMIN);
      _waitms(500);
    }
}
#include <smartpins.h>
void mad_begin()
{
 /* _pinh(28);
  uint32_t clkticks_per_step = (60.0/(double)200000)*_clockfreq();
  if (clkticks_per_step >= 65535)
    printf("too slow for hardware :()");
  printf("Clock Freq: %lu\n", clkticks_per_step);
  uint32_t pulseTiming = ((clkticks_per_step >> 1) << 16) | clkticks_per_step; // Frequency of Pulse in ms
  printf("Pulse Timing: %lu\n", pulseTiming);
  _pinstart(PIN_SERVO_PUL, P_PULSE | P_OE, pulseTiming, 0);
  _wypin( PIN_SERVO_PUL, 10000 );
  while(_pinr(PIN_SERVO_PUL) == 0);
  printf("Done pin!\n");
  while(1)
  {
  }*/
  //test_navkey();
 // test_sd_card();
  //while(1);
  notification_init();
  machine_state_init(); // Initialize the machine state default values, memory, and lock
  init_machine_profile(); // Initialize the machine profile memory and lock
  _stdio_debug_lock = _locknew();
  if (_stdio_debug_lock == -1)
  {
    printf("WARNING NO LOCKS AVAILABLE!!!, EXITING PROGRAM\n");
    _reboot();
  }

  if (motion_begin())
  {
    DEBUG_INFO("%s","Motion started\n");
  }
  else
  {
    DEBUG_ERROR("%s","Motion failed to start\n");
    _reboot();
  }

  if (control_begin())
  {
    DEBUG_INFO("%s","Control started\n");
  }
  else
  {
    DEBUG_INFO("%s","Control failed to start\n");
    return;
  }

  DEBUG_INFO("%s","Starting serial communication\n");
  start_communication();
  DEBUG_NOTIFY("%s","MaD Firmware Started\n");
  state_machine_set(PARAM_SELF_CHARGE_PUMP, true);
  monitor_begin(10);
}
