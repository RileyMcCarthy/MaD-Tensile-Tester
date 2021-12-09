#include "Monitor.h"
static long monitor_stack[64];

/*responsible for reading/writing data to buffer/test output*/
static void monitor_cog(Monitor *monitor)
{
    int sampleCount = 0;
    int lastms = 0;
    int delayms = 1000000 / monitor->sampleRate;
    int delay = 0;
    while (1)
    {
        /*Delay to run at sampleRate*/
        delay = delayms + (_getus() - lastms);
        if (delay > 0)
            _waitus(delay);
        lastms = _getus();

        int force = force_gauge_get_force(forceGauge); // Get Force
        int position = dyn4->encoder.value();          // Get Position
        int time = _getms();                           // Get Time

        // Update Buffer
        // circular buffer is faster implementation, this is simple for now
        monitor->writingData = true;
        for (int i = 1; i < MONITOR_BUFFER_SIZE; i++) // shift buffer right
        {
            monitor->dataBuffer[i] = monitor->dataBuffer[i - 1];
        }
        buffer[0].force = force;
        buffer[0].position = position;
        buffer[0].timems = time;
        monitor->writingData = false;
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
