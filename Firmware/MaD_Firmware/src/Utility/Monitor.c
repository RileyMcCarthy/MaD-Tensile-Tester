#include "Monitor.h"
static long monitor_stack[64];

/*responsible for reading/writing data to buffer/test output*/
static void monitor_cog(Monitor *monitor)
{
    int sampleCount = 0;
    int lastms = 0;
    int delayms = 1000000 / monitor->sampleRate;
    int delay = 0;
    int forceRaw;
    while (1)
    {
        /*Delay to run at sampleRate*/
        delay = delayms - (_getus() - lastms);
        // printf("delay: %d\n", delay);
        if (delay > 0)
            _waitus(delay);
        lastms = _getus();
        // printf("monitor running\n");
        int forceRawTemp = force_gauge_get_raw(monitor->forceGauge); // Get Force
        if (forceRawTemp != NULL)
        {
            monitor->data.forceRaw = forceRawTemp;
            monitor->data.force = force_gauge_raw_to_force(monitor->forceGauge, forceRaw);
        }
        monitor->data.position = monitor->dyn4->encoder.value(); // Get Position
        monitor->data.timems = _getms();                         // Get Time

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
