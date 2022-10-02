#include "MaD.h"
#include "JSON.h"
#include <stdio.h>
#include <stdint.h>
#include "ControlSystem.h"
#include "W25QXX.h"
#include "Error.h"

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
