#include "DataSlave.h"

#define DATA_MEMORY_SIZE 1000
static long data_stack[DATA_MEMORY_SIZE];

#define _DATA_CS 41
#define _DATA_RDY 39 // 42
#define _DATA_DO 42  // 39
#define _DATA_CLK 40

typedef union DataPacket_s
{
    MonitorData data;
    uint8_t byte[sizeof(MonitorData)];
} DataPacket;

static bool hasData;

void data_update()
{
    hasData = true;
}

static void data_cog(MonitorData *data)
{
    _pinl(_DATA_DO);
    _pinh(_DATA_RDY);
    DataPacket packet;
    while (1)
    {
    START:
        hasData = false;
        while (_pinr(_DATA_CS) == 0) // wait for cs high
        {
            _pinw(_DATA_RDY, !hasData);
        }
        while (_pinr(_DATA_CS) != 0) // wait for cs low
        {
            _pinw(_DATA_RDY, !hasData);
        }
        packet.data = *data;
        for (int i = 0; i < sizeof(MonitorData) * 8; i++)
        {
            int index = i / 8;
            int shift = i % 8;
            while (_pinr(_DATA_CLK) != 0) // wait for clk low
            {
                if (_pinr(_DATA_CS) == 1) // cs high exit loop
                {
                    printf("exiting early\n");
                    goto START;
                }
            }
            _pinw(_DATA_DO, (packet.byte[index] >> shift) & 0x01);
            while (_pinr(_DATA_CLK) != 1) // wait for clk high
            {
                if (_pinr(_DATA_CS) == 1) // cs high exit loop
                {
                    printf("exiting early2\n");
                    goto START;
                }
            }
        }
    }
}

bool data_begin(MonitorData *monitorData)
{
    hasData = false;
    int cogid = _cogstart_C(data_cog, monitorData, &data_stack[0], sizeof(long) * DATA_MEMORY_SIZE);
    if (cogid != -1)
    {
        return true;
    }
    return false;
}