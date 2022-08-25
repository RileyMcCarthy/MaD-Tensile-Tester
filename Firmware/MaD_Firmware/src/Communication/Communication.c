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
#define CMD_MOTIONSTATUS 9   // send/recieve motion status
#define CMD_FLASHDATA 10     // Send flash data

// @TODO RETURN CHECKSUM FOR VALIDATION IT WAS RECIEVED CORRECTLY
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
/*static bool send(char *buf, unsigned int size)
{
    int attempts = 0;
    unsigned crc;
    unsigned crcRead;
    do
    {
        if (buf == NULL)
        {
            printf("Failed to send: buffer is empty\n");
            return false;
        }
        for (unsigned int i = 0; i < size; i++)
        {
            fds.tx(buf[i]);
        }
        crc = crc8(buf, size);
        fds.tx(crc);
        attempts++;
        crcRead = fds.rxtime(100);
        if (crcRead == 0)
        {
            return false;
        }
    } while (crc != crcRead || attempts > 5);
    return true;*/
static bool send(char *buf, unsigned int size)
{
    if (buf == NULL)
    {
        printf("Failed to send: buffer is empty\n");
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

static int recieveCMD()
{
    while (1)
    {
        while (fds.rx() != 0x55)
            ;
        int cmd = fds.rxtime(10);
        if (cmd != -1)
        {
            return cmd;
        }
    }
}

void beginCommunication(MachineProfile *machineProfile, MachineState *machineState, Monitor *monitor, ControlSystem *control)
{
    // Begin main loop
    fds.start(50, 49, 0, 115200);
    while (1)
    {
        printf("Waiting for command\n");
        int cmd = recieveCMD();
        char *ptr;
        printf("cmd:%d,write:%d\n", cmd & ~CMD_WRITE, (cmd & CMD_WRITE) == CMD_WRITE);
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
                printf("Sending monitor data:%f\n", monitor->data.force);
                send(&(monitor->data), sizeof(MonitorData));
                break;
            }
            case CMD_STATE:
            {
                printf("Sending machine state\n");
                send(machineState, sizeof(MachineState));
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
                send(&(machineState->motionParameters.mode), sizeof(MotionMode));
                break;
            }
            case CMD_MOTIONFUNCTION:
            {
                printf("Sending motion function and data\n");
                send(&(machineState->function), sizeof(int));
                send(&(machineState->functionData), sizeof(int));
                break;
            }
            case CMD_FLASHDATA:
            {
                printf("Sending flash data\n");
                MonitorData *data = monitor_read_data(0); // Read flash from address 0
                while (1)
                {
                    send(data, sizeof(MonitorData));
                    printf("Time:%d\n", data->timems);
                    if (data->timems < 0)
                    {
                        break;
                    }
                    monitor_read_data(-1); // Read next address in flash
                }
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
                printf("Getting machine profile: %d\n", sizeof(MachineProfile));
                MachineProfile temp;
                if (receive(&temp, sizeof(MachineProfile)))
                {
                    *machineProfile = temp;
                    printf("test var:%d\n", machineProfile->number);
                }
                else
                {
                    printf("failed to receive machine profile\n");
                }
                break;
            }
            case CMD_MCONFIG:
            {
                printf("Getting machine configuration\n");
                MachineConfiguration configuration;
                if (receive(&configuration, sizeof(MachineConfiguration)))
                {
                    machineProfile->configuration = configuration;
                    printf("test var:%f\n", machineProfile->configuration.maxMotorTorque);
                }
                else
                {
                    printf("failed to receive machine configuration\n");
                }
                break;
            }
            case CMD_MPERFORMANCE:
            {
                printf("Getting machine performance\n");
                MachinePerformance performance;
                if (receive(&performance, sizeof(MachinePerformance)))
                {
                    machineProfile->performance = performance;
                    printf("test var:%f\n", machineProfile->performance.maxVelocity);
                }
                else
                {
                    printf("failed to receive machine performance\n");
                }
                break;
            }
            case CMD_MOTIONPROFILE:
            {
                printf("Getting motion profile\n");
                MotionProfile profile;
                if (receive(&profile, sizeof(MotionProfile)))
                {
                    control->motionProfile = profile;
                    printf("test var:%s\n", control->motionProfile.name);
                }
                else
                {
                    printf("failed to receive motion profile\n");
                }
                break;
            }
            case CMD_MOTIONMODE:
            {
                //@TODO remove motion status hardcode
                state_machine_set(machineState, PARAM_MOTION_STATUS, STATUS_ENABLED);
                printf("Getting motion mode\n");
                MotionMode mode;
                if (receive(&mode, sizeof(int)))
                {
                    state_machine_set(machineState, PARAM_MOTION_MODE, mode);
                    printf("test var:%d\n", machineState->motionParameters.mode);
                }
                else
                {
                    printf("failed to receive motion mode\n");
                }
                break;
            }
            case CMD_MOTIONFUNCTION:
            {
                printf("Getting motion function,data\n");
                int function;
                int data;
                if (receive(&function, sizeof(int)) && receive(&data, sizeof(int)))
                {
                    machineState->function = function;
                    machineState->functionData = data;
                    printf("Function,data:%d,%d\n", machineState->function, machineState->functionData);
                }
                else
                {
                    printf("failed to receive function and data\n");
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