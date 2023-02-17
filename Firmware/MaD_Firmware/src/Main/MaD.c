#include "MaD.h"
#include "JSON.h"
#include <stdio.h>
#include <stdint.h>
#include "ControlSystem.h"
#include "Error.h"
#include "SDCard.h"

static Monitor monitor;
static ControlSystem control;

static MachineProfile machineProfile;
static MachineState machineState;

static void load_machine_profile()
{
  if (sdcard_open("config.txt","r"))
  {
    sdcard_read(&machineProfile, sizeof(MachineProfile));
    printf("Loaded machine profile\n");
  }
  else
  {
    printf("Failed to open config.txt\n");
  }
  sdcard_close();
}

/**
 * @brief Starts the display, motion control, and all MaD board related tasks. Should never exit
 *
 */
void mad_begin()
{

  machine_state_init(&machineState);

  if (sdcard_begin())
  {
    // printf("Motion started\n");
  }
  else
  {
    // printf("Motion failed to start\n");
    return;
  }
  load_machine_profile();

 // while(!sdcard_open("test.txt", "r"))
 //   printf("File exists\n");
 // 
 // MonitorData buffer;
 // printf("Reading file\n");
 // while (sdcard_read(&buffer, sizeof(MonitorData)))
 // {
 //   printf("%d, %d, %d\n", buffer.timems, -1*buffer.encoderRaw, buffer.setpoint);
 // }
 // printf("Closing filetest\n");
 // sdcard_close();
 // while(1);

  //_waitms(100);
  //sdcard_open("config.txt", "w");
  //printf("Opened file\n");
  //sdcard_write("Testing writing data\n", 20);
  //while(!sdcard_is_busy());
  //
  //sdcard_close();
  //while(sdcard_is_open());
  //
  //printf("Opening file\n");
  //sdcard_open("config.txt", "r");
  //while(!sdcard_is_open());
  //
  //char buffer[200];
  ////printf("Reading file\n");
  //if (sdcard_read(&buffer, 200))
  //{
  //  printf("%s", buffer);
  //}
  //else
  //{
  //  printf("Failed to read file\n");
  //}
  //sdcard_close();
  //while(1);
  if (motion_begin())
  {
    // printf("Motion started\n");
  }
  else
  {
    // printf("Motion failed to start\n");
    return;
  }

  if (monitor_begin(&monitor, &machineState, &(machineProfile.configuration), 10))
  {
    // printf("Monitor started\n");
  }
  else
  {
    // printf("Monitor failed to start\n");
    return;
  }

  if (control_begin(&control, &machineProfile, &machineState, &(monitor.data)))
  {
    // printf("Control started\n");
  }
  else
  {
    // printf("Control failed to start\n");
    return;
  }
  //_waitms(300);
  //// load motion profile
  //int dir = 1;
  //int speed = 1000;
  //int count = 0;
  //motion_test_clear();
  //while(count < 100)
  //{
  //  int steps = dir*1000;
  //  printf("steps: %d\n", steps);
  //  motion_test_add_move(steps, speed, count == 99);
  //  //_waitms(1000/speed);
  //  if (dir == 1)
  //  {
  //    dir = -1;
  //  }
  //  else
  //  {
  //    dir = 1;
  //  }
  //  speed += 100;
  //  count++;
  //}
//
  //FILE *f = fopen("/sd/test.txt", "w");
  //if (f == NULL)
  //{
  //    printf("Error opening file!\n");
  //    exit(1);
  //}
//
  state_machine_set(&machineState, PARAM_SELF_CHARGE_PUMP, true);

  // printf("Starting serial communication\n");
  beginCommunication(&machineProfile, &machineState, &monitor, &control);
  
}
