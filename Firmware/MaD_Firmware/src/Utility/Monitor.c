#include "Monitor.h"
#include "IOBoard.h"
#define MONITOR_MEMORY_SIZE 64 * 64
static long monitor_stack[MONITOR_MEMORY_SIZE];
static bool monitorHasNewPosition = false;
static int monitorNewPosition = 0;
bool monitorWriteData;

/*responsible for reading/writing data to buffer/test output*/
static void
monitor_cog(Monitor *monitor)
{
    int sampleCount = 0;
    int lastus = 0;
    int delayus = 1000000 / monitor->sampleRate;
    int delay = 0;
    Encoder encoder;
    encoder.start(DYN4_ENCODER_A, DYN4_ENCODER_B, -1, false, 0, -100000, 100000);
    monitorWriteData = false;
    FILE *file = NULL;
    mount("/da", _vfs_open_sdcardx(40, 42, 41, 39)); // Mount data card using default pins
    int startTimems = 0;
    while (1)
    {
        /*Delay to run at sampleRate*/
        delay = delayus - (_getus() - lastus);
        if (delay > 0)
            _waitus(delay);
        lastus = _getus();
        int forceRawTemp = force_gauge_get_raw(monitor->forceGauge); // Get Force
        if (forceRawTemp != NULL)
        {
            monitor->data.forceRaw = forceRawTemp;
            monitor->data.force = force_gauge_raw_to_force(monitor->profile->configuration->forceGaugeZeroFactor, monitor->profile->configuration->forceGaugeScaleFactor, forceRawTemp);
        }
        else // Force gauge isnt responding attempt to reconnect
        {
            force_gauge_begin(monitor->forceGauge, FORCE_GAUGE_RX, FORCE_GAUGE_TX, monitor->forceGauge->interpolationSlope, monitor->forceGauge->interpolationZero);
            encoder.set(0);
        }
        if (monitorHasNewPosition)
        {
            encoder.set(monitorNewPosition);
            monitorHasNewPosition = false;
        }
        monitor->data.positionum = (1000 * steps_to_mm(encoder.value(), monitor->profile->configuration)); // Get Position in um
        monitor->data.timems = _getms();
        if (monitorWriteData)
        {
            if (file == NULL)
            {
                startTimems = _getms();
                file = fopen("/da/raw1.txt", "w");
                if (file == NULL)
                {
                    printf("Failed to open file for writing\n");
                }
                else
                {
                    printf("Opened file for writing\n");
                    fprintf(file, "time (ms),forceRaw,force (mN), encoderRaw,position (um)\n");
                }
            }
            // printf("%d,%d,%d,%d,%d\n", monitor->data.timems, monitor->data.forceRaw, monitor->data.force, encoder.value(), monitor->data.positionum);
            fprintf(file, "%d,%d,%d,%d,%d\n", monitor->data.timems - startTimems, monitor->data.forceRaw, monitor->data.force, monitor->data.encoderRaw, monitor->data.positionum);
        }
        else
        {
            if (file != NULL)
            {
                fclose(file);
                file = NULL;
            }
        }

        //  Update Buffer
        //  circular buffer is faster implementation, this is simple for now
    }
}

bool monitor_begin(Monitor *monitor, DYN4 *dyn4, ForceGauge *forceGauge, MachineProfile *profile, int sampleRate)
{
    monitor->dyn4 = dyn4;
    monitor->forceGauge = forceGauge;
    monitor->profile = profile;
    monitor->sampleRate = sampleRate;
    monitor->cogid = _cogstart_C(monitor_cog, monitor, &monitor_stack[0], sizeof(long) * MONITOR_MEMORY_SIZE);
    if (monitor->cogid != -1)
    {
        return true;
    }
    return false;
}

Monitor *monitor_create()
{
    return (Monitor *)malloc(sizeof(Monitor));
}

void monitor_destroy(Monitor *monitor)
{
    free(monitor);
}

void monitor_set_position(int position)
{
    monitorNewPosition = position;
    monitorHasNewPosition = true;
}
