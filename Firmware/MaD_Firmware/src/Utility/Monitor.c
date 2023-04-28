#include "Utility/Monitor.h"
#include "Utility/IOBoard.h"
#include "ForceGauge.h"
#include "Encoder.h"
#include <stdbool.h>
#include <simpletools.h>
#include <stdio.h>
#include <JSON.h>
#include <propeller.h>

#define MONITOR_MEMORY_SIZE 3000
static long monitor_stack[MONITOR_MEMORY_SIZE];

extern long motion_position_steps;
static ForceGauge forceGauge;
static Encoder encoder;

bool monitorLogData;

// SD Card Variables
MachineProfile sd_card_profile;
int read_profile_status = 0;
MonitorData sd_card_data[255];
int read_data_index = -1;
int read_data_count = 0;

static bool get_force(ForceGauge *forceGauge, MachineState *state, int lastLog)
{
  if (forceGauge->responding)
  {
    return forceGauge->counter != lastLog;
  }
  //printf("Force Gauge disconnected, attempting to reconnect\n");
  force_gauge_stop(forceGauge);
  if (force_gauge_begin(forceGauge, FORCE_GAUGE_RX, FORCE_GAUGE_TX))
  {
    //printf("Force Gauge reconnected\n");
    state_machine_set(state, PARAM_MACHINE_FORCE_GAUGE_COM, true);
  }
  else
  {
    //printf("Force Gauge failed to reconnect\n");
    state_machine_set(state, PARAM_MACHINE_FORCE_GAUGE_COM, false);
  }
  return false;
}

static void read_sd()
{
  if (read_data_index != -1)
    {
      FILE *file = fopen("/sd/test.txt", "r");
      if (file != NULL)
      {
        fseek(file, read_data_index * sizeof(MonitorData), SEEK_SET);
        int n = fread(sd_card_data, sizeof(MonitorData), read_data_count, file);
        printf("Read test data: %d\n",sd_card_data[0].log);
        fclose(file);
        read_data_count = n/sizeof(MonitorData);
        if (read_data_count == 0)
        {
          //printf("incorrect number of bytes read: %d\n", n);
        }
        read_data_index = -1; // signal that read is complete
      }
      else 
      {
        //printf("Failed to open data for reading\n");
      }
    }
    else if (read_data_count == -1)
    {
      FILE *file = fopen("/sd/test.txt", "r");
      if (file != NULL)
      {
        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        read_data_count = size / sizeof(MonitorData);
        fclose(file);
      }
      else
      {
        read_data_count = 0;
      }
    }
    else if (read_profile_status == 1)
    {
      FILE *file_config_read = fopen("/sd/config.txt", "r");
      if (file_config_read != NULL)
      {
        int n = fread(&sd_card_profile, sizeof(MachineProfile), 1, file_config_read);
        fclose(file_config_read);
        if (n != sizeof(MachineProfile))
        {
          //printf("incorrect number of bytes read: %d\n", n);
          read_profile_status = -1;
        }
        else
        {
          read_profile_status = 0;
        }
      }
      else
      {
       // printf("Failed to open file config.txt for reading\n");
        read_profile_status = -1;
      }
      printf("read_profile_status: %s\n", sd_card_profile.name);
    }
    else if (read_profile_status == 2)
    {
      FILE *file = fopen("/sd/config.txt", "w");
      if (file != NULL)
      {
        int n = fwrite(&sd_card_profile, sizeof(MachineProfile), 1, file);
        if (n != sizeof(MachineProfile))
        {
          //printf("incorrect number of bytes written: %d\n", n);
          read_profile_status = -1;
        }
        else
        {
          read_profile_status = 0;
        }
        fclose(file);
      }
      else
      {
       //printf("Failed to open file\n");
        read_profile_status = -1;
      }
    }
}

/*responsible for reading/writing data to buffer/test output*/
static void monitor_cog(Monitor *monitor)
{
  FILE *testFile = NULL;
  monitor->data.log = 0;
  
  // Set up encoder
  encoder.start(DYN4_ENCODER_A, DYN4_ENCODER_B, -1, false, 0, -100000, 100000);
  long lastTime = _getms();
  while (1)
  {
    long start = _getus();
    bool update = false;

    if (get_force(&forceGauge, monitor->machineState, monitor->data.log))
    {
      //printf("Force Gauge: %d\n", forceGauge.forceRaw);
      monitor->data.log = forceGauge.counter; // Increment when new data is added to buffer, used for checking if data is new.
      monitor->data.forceRaw = forceGauge.forceRaw;
      update = true;
    }
    else
    {
      monitor->data.forceRaw = 0; // Force gauge not responding, set to 0
    }
    
    long forceus = _getus() - start;
    monitor->data.encoderRaw = encoder.value();
    monitor->data.timems = _getms();
    monitor->data.timeus = _getus();
    monitor->data.forcemN = raw_to_force(monitor->data.forceRaw, monitor->configuration);
    monitor->data.encoderum = steps_to_um(monitor->data.encoderRaw, monitor->configuration);
    // these are convinience variables for the user, can be removed later
    monitor->data.force = raw_to_force(monitor->data.forceRaw, monitor->configuration) / 1000.0; // Convert Force to N
    //printf("Force: %f\n", monitor->data.force);
    monitor->data.position = steps_to_mm(monitor->data.encoderRaw, monitor->configuration);      // Convert steps to mm
    monitor->data.setpoint = motion_position_steps;

    long encoderus = _getus() - start - forceus;

    if (monitorLogData)
    {
      if (testFile == NULL)
      {
        testFile = fopen("/sd/test.txt", "w");
        //printf("Logging started\n");
      }
      if (update && testFile != NULL)
      {
        //printf("Writing: %d\n", monitor->data.encoderum);
        fwrite(&(monitor->data), sizeof(MonitorData), 1, testFile);
      }
    }
    else if (testFile != NULL)
    {
      fclose(testFile);
      testFile = NULL;
      //("Logging stopped\n");
    }
    else
    {
      read_sd();
    }
    _waitms(1);
  }
}

bool read_sd_profile(MachineProfile * profile)
{
  long startms = _getms();
  if (monitorLogData)
  {
    return false; // Don't read from SD card if logging
  }
  read_profile_status = 1; // Signal read
  while (read_profile_status != 0) // Wait for cog to finish reading
  {
    if (read_profile_status == -1)
    {
      // Something went wrong
      //printf("Failed to read profile\n");
      return false;
    }
    if (_getms() - startms > 5000)
    {
      // Timeout
     // printf("Timeout reading profile\n");
      return false;
    }
  }
  memcpy(profile, &sd_card_profile, sizeof(MachineProfile));
  return true;
}

bool write_sd_profile(MachineProfile * profile)
{
  long startms = _getms();
  if (monitorLogData)
  {
    return false; // Don't write to SD card if logging
  }
  memcpy(&sd_card_profile, profile, sizeof(MachineProfile));
  read_profile_status = 2; // Signal write
  while(read_profile_status != 0) // Wait for cog to finish writing
  {
    if (read_profile_status == -1)
    {
      // Something went wrong
      return false;
    }
    if (_getms() - startms > 1000)
    {
      // Timeout
      //printf("Timeout writing profile\n");
      return false;
    }
  }
  return true;
}

int read_sd_card_data(MonitorData *data, int index, int count)
{
  long startms = _getms();
  if (monitorLogData)
  {
    //printf("unable to read data while logging\n");
    return 0; // Don't read from SD card if logging
  }
  read_data_count = count;
  read_data_index = index;
  while (read_data_index != -1) // Wait for cog to finish reading
  {
    if (read_data_index == -2)
    {
      //printf("read_sd_card_data failed\n");
      // End of file
      return 0;
    }
    if (_getms() - startms > 1000)
    {
      // Timeout
      //printf("Timeout reading data\n");
      return false;
    }
  }
  memcpy(data, sd_card_data, sizeof(MonitorData)*read_data_count);
  //printf("read_sd_card_data: %d\n", read_data_count);
  return read_data_count;
}

int read_data_size()
{
  long startms = _getms();
  if (monitorLogData)
  {
    return 0; // Don't read from SD card if logging
  }
  read_data_count = -1;
  while (read_data_count == -1) // Wait for cog to finish reading
  {
    if (_getms() - startms > 1000)
    {
      // Timeout
      //printf("Timeout reading data size\n");
      return 0;
    }
  }
  return read_data_count;
}

bool monitor_begin(Monitor *monitor, MachineState *machineState, MachineConfiguration *configuration, int sampleRate)
{
  monitor->configuration = configuration;
  monitor->sampleRate = sampleRate;
  monitor->machineState = machineState;
  monitor_cog(monitor);
  return
  monitor->cogid = _cogstart_C(monitor_cog, monitor, &monitor_stack[0], sizeof(long) * MONITOR_MEMORY_SIZE);
  if (monitor->cogid != -1)
  {
    return true;
  }
  return false;
}
