#include "Communication.h"
#include <stdlib.h>
#include "StateMachine.h"
/* Command structure
 * wxxxxxxx <data>.. <CRC>
 * ________ ________ ________
 * <data> = raw data send/recieved depending on w bit
 * <CRC> = check
 */

typedef struct __using("jm_fullduplexserial.spin2") FDS;
static FDS fds;
#define CMD_WRITE 128
#define CMD_PING 0           // test communication
#define CMD_DATA 1           // send monitor data
#define CMD_STATE 2          // send machine state
#define CMD_MPROFILE 3       // send/recieve machine profile
#define CMD_MCONFIG 4        // send/recieve machine configuration
#define CMD_MPERFORMANCE 5   // send/receive machine performance
#define CMD_MOTIONPROFILE 6  // send/recieve motion profile
#define CMD_MOTIONMODE 7     // send/recieve motion mode
#define CMD_MOTIONFUNCTION 8 // send/recieve motion function and data
#define CMD_RUNTEST 9        // run motion profile
#define CMD_STOPTEST 10      // stop motion profile

static bool receive(char *buf, unsigned int size)
{
    if (buf == NULL)
    {
        return false;
    }
    for (unsigned int i = 0; i < size; i++)
    {
        buf[i] = fds.rxtime(10);
        if (buf[i] == -1)
        {
            printf("invalid data recieved\n");
            return false;
        }
    }
    return true;
}

static bool send(char *buf, unsigned int size)
{
    if (buf == NULL)
    {
        return false;
    }
    for (unsigned int i = 0; i < size; i++)
    {
        fds.tx(buf[i]);
    }
    unsigned crc = crc8(buf, size);
    fds.tx(crc);
    return true;
}

void beginCommunication(MachineProfile *machineProfile, MachineState *machineState, Monitor *monitor, ControlSystem *control)
{
    // Begin main loop
    fds.start(50, 49, 0, 115200);
    while (1)
    {
        printf("Waiting for command\n");
        int cmd = fds.rx();
        char *ptr;
        printf("cmd:%d,write:%d\n", cmd, (cmd & CMD_WRITE) == CMD_WRITE);
        if ((cmd & CMD_WRITE) != CMD_WRITE)
        {
            printf("Sending data\n");
            switch (cmd)
            {
            case CMD_PING:
            {
                printf("pinging device back\n");
                uint8_t res = 1;
                send(&res, 1);
                break;
            }
            case CMD_DATA:
            {
                printf("Sending monitor data\n");
                send(&(monitor->data), sizeof(MonitorData));
                break;
            }
            case CMD_MPROFILE:
            {
                printf("Sending machine profile\n");
                send(machineProfile, sizeof(MachineProfile));
                break;
            }
            case CMD_MCONFIG:
            {
                printf("Sending machine configuration\n");
                send(&(machineProfile->configuration), sizeof(MachineConfiguration));
                break;
            }
            case CMD_MPERFORMANCE:
            {
                printf("Sending machine performance\n");
                send(&(machineProfile->performance), sizeof(MachinePerformance));
                break;
            }
            case CMD_MOTIONPROFILE:
            {
                printf("Sending motion profile\n");
                send(&(control->motionProfile), sizeof(MotionProfile));
                break;
            }
            case CMD_MOTIONMODE:
            {
                printf("Sending motion mode\n");
                send(&(machineState.motionParameters.mode), sizeof(int));
                break;
            }
            case CMD_RUNTEST:
            {
                printf("Running motion profile\n");
                send(&(control->motionProfile), sizeof(MotionProfile));

                break;
            }
            default:
            {
                printf("Command not found\n");
                break;
            }
            }
        }
        else
        {
            printf("Recieving Data\n");
            switch (cmd & ~CMD_WRITE)
            {
            case CMD_MPROFILE:
            {
                printf("Getting motion profile\n");
                MachineProfile temp;
                if (receive(&temp, sizeof(MachineProfile)))
                {
                    *machineProfile = temp;
                    printf("test var:%f\n", machineProfile->number);
                }
                else
                {
                    printf("failed to receive motion profile\n");
                }
                break;
            }
            default:
            {
                printf("unknown command\n");
                break;
            }
            }
        }
    }
}