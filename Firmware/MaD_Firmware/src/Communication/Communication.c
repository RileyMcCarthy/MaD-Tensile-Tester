#include "Communication.h"
#include <stdlib.h>
#include "StateMachine.h"
#include "Error.h"
/* Command structure
 * wxxxxxxx <data>.. <CRC>
 * ________ ________ ________
 * <data> = raw data send/recieved depending on w bit
 * <CRC> = check
 */

typedef struct __using("jm_fullduplexserial.spin2") FDS;
static FDS fds;

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
            DEBUG_WARNING("invalid data recieved\n");
            return false;
        }
    }
    return true;
}

static bool send(char *buf, unsigned int size)
{
    DEBUG_WARNING("Sending data of size: %d\n", size);
    char *bufCopy = (char *)__builtin_alloca(size);
    memcpy(bufCopy, buf, size);
    if (bufCopy == NULL)
    {
        DEBUG_WARNING("Failed to send: buffer is empty\n");
        return false;
    }
    for (unsigned int i = 0; i < size; i++)
    {
        fds.tx(bufCopy[i]);
    }
    unsigned crc = crc8(bufCopy, size);
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

static int flashAddress = 0;

void beginCommunication(MachineProfile *machineProfile, MachineState *machineState, Monitor *monitor, ControlSystem *control)
{
    // Begin main loop
    fds.start(57, 56, 0, 256000); // tx, rx, 46, 47, 50, 49
    while (1)
    {
        DEBUG_WARNING("Waiting for command\n");
        int cmd = recieveCMD();
        DEBUG_WARNING("cmd:%d,write:%d\n", cmd & ~CMD_WRITE, (cmd & CMD_WRITE) == CMD_WRITE);
        if ((cmd & CMD_WRITE) != CMD_WRITE)
        {
            DEBUG_WARNING("Sending data\n");
            switch (cmd)
            {
            case CMD_PING:
            {
                DEBUG_WARNING("pinging device back\n");
                uint8_t res = MAD_VERSION;
                send(&res, 1);
                break;
            }
            case CMD_DATA:
            {
                DEBUG_WARNING("Sending monitor data:%f\n", monitor->data.force);
                send(&(monitor->data), sizeof(MonitorData));
                break;
            }
            case CMD_STATE:
            {
                DEBUG_WARNING("Sending machine state\n");
                send(machineState, sizeof(MachineState));
                break;
            }
            case CMD_MPROFILE:
            {
                DEBUG_WARNING("Sending machine profile\n");
                send(machineProfile, sizeof(MachineProfile));
                break;
            }
            case CMD_MCONFIG:
            {
                DEBUG_WARNING("Sending machine configuration\n");
                send(&(machineProfile->configuration), sizeof(MachineConfiguration));
                break;
            }
            case CMD_MPERFORMANCE:
            {
                DEBUG_WARNING("Sending machine performance\n");
                send(&(machineProfile->performance), sizeof(MachinePerformance));
                break;
            }
            case CMD_MOTIONPROFILE:
            {
                DEBUG_WARNING("Sending motion profile\n");
                send(&(control->motionProfile), sizeof(MotionProfile));
                break;
            }
            case CMD_MOTIONMODE:
            {
                DEBUG_WARNING("Sending motion mode\n");
                send(&(machineState->motionParameters.mode), sizeof(MotionMode));
                break;
            }
            case CMD_MOTIONFUNCTION:
            {
                DEBUG_WARNING("Sending motion function and data\n");
                send(&(machineState->_function), sizeof(int));
                send(&(machineState->_functionData), sizeof(int));
                break;
            }
            case CMD_MOTIONSTATUS:
            {
                DEBUG_WARNING("Sending motion status\n");
                send(&(machineState->motionParameters.status), sizeof(int));
                break;
            }
            default:
            {
                DEBUG_WARNING("Command not found\n");
                break;
            }
            }
        }
        else
        {
            DEBUG_WARNING("Recieving Data\n");
            switch (cmd & ~CMD_WRITE)
            {
            case CMD_MPROFILE:
            {
                DEBUG_WARNING("Getting machine profile: %d\n", sizeof(MachineProfile));
                MachineProfile temp;
                if (receive(&temp, sizeof(MachineProfile)))
                {
                    *machineProfile = temp;
                    DEBUG_WARNING("test var:%d\n", machineProfile->number);
                }
                else
                {
                    DEBUG_WARNING("failed to receive machine profile\n");
                }
                break;
            }
            case CMD_MCONFIG:
            {
                DEBUG_WARNING("Getting machine configuration\n");
                MachineConfiguration configuration;
                if (receive(&configuration, sizeof(MachineConfiguration)))
                {
                    machineProfile->configuration = configuration;
                    DEBUG_WARNING("test var:%f\n", machineProfile->configuration.maxMotorTorque);
                }
                else
                {
                    DEBUG_WARNING("failed to receive machine configuration\n");
                }
                break;
            }
            case CMD_MPERFORMANCE:
            {
                DEBUG_WARNING("Getting machine performance\n");
                MachinePerformance performance;
                if (receive(&performance, sizeof(MachinePerformance)))
                {
                    machineProfile->performance = performance;
                    DEBUG_WARNING("test var:%f\n", machineProfile->performance.maxVelocity);
                }
                else
                {
                    DEBUG_WARNING("failed to receive machine performance\n");
                }
                break;
            }
            case CMD_MOTIONPROFILE:
            {
                DEBUG_WARNING("Getting motion profile\n");
                MotionProfile profile;
                if (receive(&profile, sizeof(MotionProfile)))
                {
                    control->motionProfile = profile;
                    DEBUG_WARNING("test var:%s\n", control->motionProfile.name);
                }
                else
                {
                    DEBUG_WARNING("failed to receive motion profile\n");
                }
                break;
            }
            case CMD_MOTIONMODE:
            {
                //@TODO remove motion status hardcode
                state_machine_set(machineState, PARAM_MOTION_STATUS, MOTIONSTATUS_ENABLED);
                DEBUG_WARNING("Getting motion mode\n");
                MotionMode mode;
                if (receive(&mode, sizeof(int)))
                {
                    state_machine_set(machineState, PARAM_MOTION_MODE, mode);
                    DEBUG_WARNING("test var:%d\n", machineState->motionParameters.mode);
                }
                else
                {
                    DEBUG_WARNING("failed to receive motion mode\n");
                }
                break;
            }
            case CMD_MOTIONFUNCTION:
            {
                DEBUG_WARNING("Getting motion function,data\n");
                int function;
                int data;
                if (receive(&function, sizeof(int)) && receive(&data, sizeof(int)))
                {
                    machineState->_function = function;
                    machineState->_functionData = data;
                    DEBUG_WARNING("Function,data:%d,%d\n", machineState->_function, machineState->_functionData);
                }
                else
                {
                    DEBUG_WARNING("failed to receive function and data\n");
                }
                break;
            }
            case CMD_MOTIONSTATUS:
            {
                DEBUG_WARNING("Getting motion status\n");
                MotionStatus status;
                if (receive(&status, sizeof(int)))
                {
                    state_machine_set(machineState, PARAM_MOTION_STATUS, status);
                    DEBUG_WARNING("Motion Status:%d\n", machineState->motionParameters.status);
                }
                else
                {
                    DEBUG_WARNING("failed to receive motion status\n");
                }
                break;
            }
            default:
            {
                DEBUG_WARNING("unknown command\n");
                break;
            }
            }
        }
    }
}