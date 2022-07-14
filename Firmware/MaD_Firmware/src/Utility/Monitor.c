#include "Monitor.h"
#include "IOBoard.h"
#include "ForceGauge.h"
#include "Encoder.h"
#include <stdbool.h>

#define MONITOR_MEMORY_SIZE 4096
static long monitor_stack[MONITOR_MEMORY_SIZE];

bool monitorWriteData;

static ForceGauge forceGauge;
static Encoder encoder;

/*responsible for reading/writing data to buffer/test output*/
static void monitor_cog(Monitor *monitor)
{
    _waitms(1000);
    mount("/da", _vfs_open_sdcardx(40, 42, 41, 39)); // Mount data card using default pins
    FILE *file = fopen("/da/raw1.txt", "w");
        if (file != NULL) {
            printf("file opened\n");
        }else {
            printf("file failed to open\n");
        }
        fputs("time (ms),force (mN),position (mm),forceRaw,encoderRaw\n",file);
        monitor->data.timems=1000;
        monitor->data.forceRaw=0;
        monitor->data.encoderRaw=50;
        for (int i=0;i<1000000;i++)
        {
            putc('\n',file); 
            //fprintf(file,"%d,%d,%d\n", monitor->data.timems, monitor->data.forceRaw, monitor->data.encoderRaw);
        }
        fclose(file);
        printf("done writing\n");
        while(1);
    // Connect Force Gauge
    if (force_gauge_begin(&forceGauge, FORCE_GAUGE_RX, FORCE_GAUGE_TX) == SUCCESS)
    {
        state_machine_set(monitor->machineState, PARAM_MACHINE_FORCE_GAUGE_COM, true);
    }else
    {
        state_machine_set(monitor->machineState, PARAM_MACHINE_FORCE_GAUGE_COM, false);
    }

    // Set up encoder
    encoder.start(DYN4_ENCODER_A, DYN4_ENCODER_B, -1, false, 0, -100000, 100000);

    int delayCycles = _clkfreq / monitor->sampleRate;
    printf("Monitor Cog Started at %dHz with delay of:%d\n", monitor->sampleRate,delayCycles);
    //FILE *file = NULL;

    monitorWriteData = false;
    while (1)
    {
        file = fopen("/da/raw1.txt", "w");
        fprintf(file, "time (ms),force (mN),position (mm),forceRaw,encoderRaw\n");
        monitor->data.timems=1000;
        monitor->data.forceRaw=0;
        monitor->data.encoderRaw=50;
        if (file != NULL) {
            printf("file opened\n");
        }else {
            printf("file failed to open\n");
        }
        for (int i=0;i<1000;i++)
        {
            fprintf(file,"%d,%d,%d\n", monitor->data.timems, monitor->data.forceRaw, monitor->data.encoderRaw);
        }
        fclose(file);
        printf("done writing\n");
        /*Delay to run at sampleRate, replace with _waitcnt*/
        uint32_t waitcycles = _cnt() + delayCycles;

        Error err;
        int forceRawTemp = force_gauge_get_raw(&forceGauge, &err); // Get Force
        if (err == SUCCESS)
        {
            monitor->data.forceRaw = forceRawTemp;
        }
        else // Force gauge isnt responding attempt to reconnect
        {
            printf("Force Gauge disconnected, attempting to reconnect\n");
            force_gauge_stop(&forceGauge);
            if (force_gauge_begin(&forceGauge, FORCE_GAUGE_RX, FORCE_GAUGE_TX) == SUCCESS)
            {
                state_machine_set(monitor->machineState, PARAM_MACHINE_FORCE_GAUGE_COM, true);
            }else
            {
                state_machine_set(monitor->machineState, PARAM_MACHINE_FORCE_GAUGE_COM, false);
            }
        }

        monitor->data.encoderRaw = encoder.value();

        //monitor->data.positionum = (1000 * steps_to_mm(monitor->encoder->value(), &(monitor->profile->configuration))); // Get Position in um
        monitor->data.timems = _getms();
        if (monitorWriteData)
        {
            if (file==NULL)
            {
                file = fopen("/da/raw1.txt", "w");
                printf("opening file\n");
                fprintf(file, "time (ms),force (mN),position (mm),forceRaw,encoderRaw\n");
            }
            else
            {
                fprintf(file,"%d,%d,%d\n", monitor->data.timems, monitor->data.forceRaw, monitor->data.encoderRaw);
            }
        }
        else
        {
            if (file != NULL)
            {
                printf("closing file\n");
                fclose(file);
                file=NULL;
            }
        }
        if (waitcycles < (uint32_t)_cnt())
            ;//printf("Error: Monitor COG is running too slow:%lu,%lu\n", ((uint32_t)_cnt() - waitcycles),delayCycles);
        else
            waitcnt(waitcycles);
    }
}

bool monitor_begin(Monitor *monitor, MachineState *machineState, int sampleRate)
{
    monitor->sampleRate = sampleRate;
    monitor->machineState = machineState;
    monitor->cogid = _cogstart_C(monitor_cog, monitor, &monitor_stack[0], sizeof(long) * MONITOR_MEMORY_SIZE);
    if (monitor->cogid != -1)
    {
        return true;
    }
    return false;
}

void monitor_set_position(int position)
{
  //  monitorNewPosition = position;
  //  monitorHasNewPosition = true;
}
