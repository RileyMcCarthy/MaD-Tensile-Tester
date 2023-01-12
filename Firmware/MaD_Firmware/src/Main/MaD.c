#include "MaD.h"
#include "JSON.h"
#include <stdio.h>
#include <stdint.h>
#include "ControlSystem.h"
#include "Error.h"
#include "DataSlave.h"

static Monitor monitor;
static ControlSystem control;

static MachineProfile machineProfile;
static MachineState machineState;

/**
 * @brief Starts the display, motion control, and all MaD board related tasks. Should never exit
 *
 */
void mad_begin()
{
  int delay = 1;
  // printf("Starting MaD\n");
  //_pinl(PIN_SERVO_ENA); // does nothing... yet
  //_pinl(PIN_SERVO_DIR); // pinh is cw, pinl is ccw
  //while(1)
  //{
  //  _pinl(PIN_SERVO_PUL);
  //  _waitms(delay);
  //  _pinh(PIN_SERVO_PUL);
  //  _waitms(delay);
  //}

  machine_state_init(&machineState);

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

  state_machine_set(&machineState, PARAM_SELF_CHARGE_PUMP, true);

  // printf("Starting serial communication\n");
  beginCommunication(&machineProfile, &machineState, &monitor, &control);
}
