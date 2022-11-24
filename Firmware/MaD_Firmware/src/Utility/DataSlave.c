#include "DataSlave.h"
#include "CRC.h"
#define DATA_MEMORY_SIZE 1000
static long data_stack[DATA_MEMORY_SIZE];

#define _DATA_CS 41
#define _DATA_RDY 39 // 42
#define _DATA_DO 42  // 39
#define _DATA_CLK 40

static bool hasData;
static MonitorData *data;
static DataPacket newPacket;

void data_update()
{
    newPacket.data = *data;
    newPacket.crc = crc8(&(newPacket.data), sizeof(MonitorData));
    hasData = true;
}

static void data_cog(MonitorData *arg)
{
    data = arg;
    _pinl(_DATA_DO);
    _pinh(_DATA_RDY);
    DataPacket packet;
    int size = sizeof(DataPacket);
    uint8_t *bytes;
    while (1)
    {
    START:
        hasData = false;
        while (_pinr(_DATA_CS) == 0) // wait for cs high
            ;
        while (_pinr(_DATA_CS) != 0) // wait for cs low
        {
            if (hasData)
            {
                packet = newPacket;
                _pinh(_DATA_RDY);
                hasData = false;
            }
            _pinl(_DATA_RDY);
        }
        uint8_t *bytes = (uint8_t *)&packet;
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                while (_pinr(_DATA_CLK) != 0) // wait for clk low
                {
                    if (_pinr(_DATA_CS) == 1) // cs high exit loop
                    {
                        printf("Sent %d/%d bytes\n", i * 8 * j, size * 8);
                        goto START;
                    }
                }
                _pinw(_DATA_DO, (bytes[i] >> (7 - j)) & 0x01);
                while (_pinr(_DATA_CLK) != 1) // wait for clk high
                {
                    if (_pinr(_DATA_CS) == 1) // cs high exit loop
                    {
                        if (i * 8 + j != size * 8 - 1)
                        {
                            printf("Sent %d/%d bytes\n", i * 8 + j, size * 8);
                        }
                        goto START;
                    }
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