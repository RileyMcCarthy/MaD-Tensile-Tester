#include "Monitor.h"
#include "IOBoard.h"
#include "ForceGauge.h"
#include "Encoder.h"
#include <stdbool.h>
#include "simpletools.h"
#include "SDCard.h"

#define MONITOR_MEMORY_SIZE 1000
static long monitor_stack[MONITOR_MEMORY_SIZE];

extern long motion_position_steps;
static ForceGauge forceGauge;
static Encoder encoder;

bool monitorLogData;

extern hasNewData;

/*responsible for reading/writing data to buffer/test output*/
static void monitor_cog(Monitor *monitor)
{
  monitor->data.log = 0;
  bool isLogging = false;
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
    bool update = false;

    if (forceGauge.responding)
    {
      if (forceGauge.counter != monitor->data.log)
      {
        monitor->data.log = forceGauge.counter; // Increment when new data is added to buffer, used for checking if data is new.
        update = true;
      }
      monitor->data.forceRaw = forceGauge.forceRaw;
    }
    else
    {
      monitor->data.forceRaw = 0;
      //printf("Force Gauge disconnected, attempting to reconnect\n");
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

    monitor->data.encoderRaw = encoder.value();
    monitor->data.timems = _getms();
    monitor->data.timeus = _getus();
    monitor->data.forcemN = raw_to_force(monitor->data.forceRaw, monitor->configuration);
    monitor->data.encoderum = steps_to_um(monitor->data.encoderRaw, monitor->configuration);
    // these are convinience variables for the user, can be removed later
    monitor->data.force = raw_to_force(monitor->data.forceRaw, monitor->configuration) / 1000.0; // Convert Force to N
    monitor->data.position = steps_to_mm(monitor->data.encoderRaw, monitor->configuration);      // Convert steps to mm
    monitor->data.setpoint = motion_position_steps;
    if (update && monitorLogData)
    {
      if (isLogging == false)
      {
        isLogging = sdcard_open("test.txt", "w");
        printf("Logging started\n");
      }
      else
      {
        printf("writing data\n");
        while(!sdcard_write(&(monitor->data), sizeof(MonitorData)));
      }
    }
    else if (!monitorLogData && isLogging)
    {
      sdcard_close();
      isLogging = false;
      printf("Logging stopped\n");
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
