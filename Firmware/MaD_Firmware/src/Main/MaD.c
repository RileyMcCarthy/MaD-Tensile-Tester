#include <stdint.h>
#include "Main/MaD.h"
#include "JSON.h"
#include "Utility/ControlSystem.h"
#include "Utility/Error.h"

static Monitor monitor;
static ControlSystem control;

static MachineProfile machineProfile;
static MachineState machineState;

static void load_machine_profile()
{
  MachineProfile * profile;
  printf("Reading config.txt\n");
  if (!read_sd_profile(&machineProfile))
  {
    printf("Failed to read config.txt\n");
    return;
  }
  printf("Read config.txt:%s\n", machineProfile.name);
}


/**
 * @brief Starts the display, motion control, and all MaD board related tasks. Should never exit
 *
 */
void mad_begin()
{
  machine_state_init(&machineState);
  if (motion_begin())
  {
    // printf("Motion started\n");
  }
  else
  {
    printf("Motion failed to start\n");
    return;
  }

  /*if (monitor_begin(&monitor, &machineState, &(machineProfile.configuration), 10))
  {
    // printf("Monitor started\n");
  }
  else
  {
    printf("Monitor failed to start\n");
    return;
  }
  _waitms(1000);
  while(1)
  {
    load_machine_profile();
  }
  load_machine_profile();*/

  if (control_begin(&control, &machineProfile, &machineState, &(monitor.data)))
  {
    // printf("Control started\n");
  }
  else
  {
    printf("Control failed to start\n");
    return;
  }

  state_machine_set(&machineState, PARAM_SELF_CHARGE_PUMP, true);

  // printf("Starting serial communication\n");
  start_communication(&machineProfile, &machineState, &monitor, &control);
  monitor_begin(&monitor, &machineState, &(machineProfile.configuration), 10);
}
