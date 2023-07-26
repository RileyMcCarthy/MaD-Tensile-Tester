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
void mad_begin()
{
  //_pinh(28);
  //while(1);
  _stdio_debug_lock = _locknew();
  if (_stdio_debug_lock == -1)
  {
    printf("WARNING NO LOCKS AVAILABLE!!!, EXITING PROGRAM\n");
    return;
  }
  machine_state_init(); // Initialize the machine state default values, memory, and lock

  init_machine_profile(); // Initialize the machine profile memory and lock

  if (motion_begin())
  {
    DEBUG_INFO("%s","Motion started\n");
  }
  else
  {
    DEBUG_INFO("%s","Motion failed to start\n");
    return;
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

  state_machine_set(PARAM_SELF_CHARGE_PUMP, true);

  DEBUG_INFO("%s","Starting serial communication\n");
  start_communication();
  monitor_begin(10);
}
