#include "Monitor.h"
#include "IOBoard.h"
static long monitor_stack[64 * 4];
static bool monitorHasNewPosition = false;
static int monitorNewPosition = 0;

/*responsible for reading/writing data to buffer/test output*/
static void monitor_cog(Monitor *monitor)
{
    int sampleCount = 0;
    int lastms = 0;
    int delayms = 1000000 / monitor->sampleRate;
    int delay = 0;
    Encoder encoder;
    encoder.start(DYN4_ENCODER_A, DYN4_ENCODER_B, -1, false, 0, -100000, 100000);
    while (1)
    {
        /*Delay to run at sampleRate*/
        delay = delayms - (_getus() - lastms);
        if (delay > 0)
            _waitus(delay);
        lastms = _getus();
        int forceRawTemp = force_gauge_get_raw(monitor->forceGauge); // Get Force
        if (forceRawTemp != NULL)
        {
            monitor->data.forceRaw = forceRawTemp;
            monitor->data.force = force_gauge_raw_to_force(monitor->forceGauge, forceRawTemp);
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
        monitor->data.position = encoder.value(); // Get Position
        monitor->data.timems = _getms();          // Get Time

        // Update Buffer
        // circular buffer is faster implementation, this is simple for now
    }
}

bool monitor_begin(Monitor *monitor, DYN4 *dyn4, ForceGauge *forceGauge, int sampleRate)
{
    monitor->dyn4 = dyn4;
    monitor->forceGauge = forceGauge;
    monitor->sampleRate = sampleRate;
    monitor->cogid = _cogstart_C(monitor_cog, monitor, &monitor_stack[0], sizeof(long) * 64);
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