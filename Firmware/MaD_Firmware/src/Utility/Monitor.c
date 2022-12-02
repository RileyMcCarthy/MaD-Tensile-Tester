#include "Monitor.h"
#include "IOBoard.h"
#include "ForceGauge.h"
#include "Encoder.h"
#include <stdbool.h>
#include "simpletools.h"

#define MONITOR_MEMORY_SIZE 1000
static long monitor_stack[MONITOR_MEMORY_SIZE];

static ForceGauge forceGauge;
static Encoder encoder;

static bool sync_setpoint;

bool monitorLogData;

extern hasNewData;

void monitor_sync_setpoint()
{
  sync_setpoint = true;
}

/*responsible for reading/writing data to buffer/test output*/
static void monitor_cog(Monitor *monitor)
{
  sync_setpoint = true;
  monitor->data.log = 0;

  // Connect Force Gauge
  if (force_gauge_begin(&forceGauge, FORCE_GAUGE_RX, FORCE_GAUGE_TX) == SUCCESS)
  {
    state_machine_set(monitor->machineState, PARAM_MACHINE_FORCE_GAUGE_COM, true);
  }
  else
  {
    state_machine_set(monitor->machineState, PARAM_MACHINE_FORCE_GAUGE_COM, false);
  }
  // Set up encoder
  encoder.start(DYN4_ENCODER_A, DYN4_ENCODER_B, -1, false, 0, -100000, 100000);
  while (1)
  {
    MonitorData temp;
    bool update = false;
    if (sync_setpoint)
    {
      encoder.Set(monitor->data.setpoint);
      sync_setpoint = false;
    }

    if (forceGauge.responding)
    {
      if (forceGauge.counter != monitor->data.log)
      {
        temp.log = forceGauge.counter; // Increment when new data is added to buffer, used for checking if data is new.
        update = true;
      }
      temp.forceRaw = forceGauge.forceRaw;
    }
    else
    {
      temp.forceRaw = 0;
      printf("Force Gauge disconnected, attempting to reconnect\n");
      force_gauge_stop(&forceGauge);
      if (force_gauge_begin(&forceGauge, FORCE_GAUGE_RX, FORCE_GAUGE_TX) == SUCCESS)
      {
        // printf("Force Gauge reconnected\n");
        state_machine_set(monitor->machineState, PARAM_MACHINE_FORCE_GAUGE_COM, true);
      }
      else
      {
        state_machine_set(monitor->machineState, PARAM_MACHINE_FORCE_GAUGE_COM, false);
      }
    }

    temp.encoderRaw = encoder.value();
    temp.timems = _getms();
    temp.timeus = _getus();
    temp.forcemN = raw_to_force(temp.forceRaw, monitor->configuration);
    temp.encoderum = steps_to_um(temp.encoderRaw, monitor->configuration);
    // these are convinience variables for the user, can be removed later
    temp.force = raw_to_force(temp.forceRaw, monitor->configuration) / 1000.0; // Convert Force to N
    temp.position = steps_to_mm(temp.encoderRaw, monitor->configuration);      // Convert steps to mm

    monitor->data = temp;
    if (update)
    {
      hasNewData = true;
    }
  }
}

bool monitor_begin(Monitor *monitor, MachineState *machineState, MachineConfiguration *configuration, int sampleRate)
{
  monitor->configuration = configuration;
  monitor->sampleRate = sampleRate;
  monitor->machineState = machineState;
  monitor->cogid = _cogstart_C(monitor_cog, monitor, &monitor_stack[0], sizeof(long) * MONITOR_MEMORY_SIZE);
  if (monitor->cogid != -1)
  {
    return true;
  }
  return false;
}
