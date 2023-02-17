#include "Communication.h"
#include <stdlib.h>
#include "StateMachine.h"
#include "Error.h"
#include "Motion.h"
#include <stdio.h>
#include "SDCard.h"
/* Command structure
 * w<7 cmd bits> <N> <N data>... <CRC>
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

static bool send(int cmd, char *buf, uint16_t size)
{
    DEBUG_INFO("Sending data of size: %d\n", size);

    char *bufCopy = NULL;
    if (size != 0)
    {
        bufCopy = (char *)__builtin_alloca(size);
        memcpy(bufCopy, buf, size);
        if (bufCopy == NULL)
        {
            DEBUG_WARNING("Failed to send: buffer is empty\n");
            return false;
        }
    }
    fds.tx(0x55);
    fds.tx(cmd);
    fds.tx(size);
    fds.tx(size>>8);
    for (unsigned int i = 0; i < size; i++)
    {
        fds.tx(bufCopy[i]);
    }
    unsigned crc = crc8(bufCopy, size);
    fds.tx(crc);
    return true;
}

static bool wait_for_queue;
static int recieveCMD()
{
    while (1)
    {
        while (fds.rxcheck() != 0x55)
        {
            if (wait_for_queue && !motion_test_is_full())
            {
                wait_for_queue = false;
                return CMD_AWK;
            }
        }
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
    wait_for_queue = false;
    fds.start(57, 56, 0, 1000000);
    while (1)
    {
        DEBUG_INFO("Waiting for command\n");
        int cmd = recieveCMD();
        DEBUG_INFO("cmd:%d,write:%d\n", cmd & ~CMD_WRITE, (cmd & CMD_WRITE) == CMD_WRITE);
        if ((cmd & CMD_WRITE) != CMD_WRITE)
        {
            DEBUG_INFO("Sending data\n");
            switch (cmd)
            {
            case CMD_PING:
            {
                DEBUG_INFO("pinging device back\n");
                uint8_t res = MAD_VERSION;
                send(CMD_PING, &res, 1);
                break;
            }
            case CMD_AWK:
            {
                DEBUG_INFO("Sending delayed awk\n");
                uint8_t awk = 0;
                send(CMD_AWK, &awk, 1);
                break;
            }
            case CMD_DATA:
            {
                DEBUG_INFO("%d,%d\n", monitor->data.forcemN, monitor->data.log);
                MonitorDataPacket packet;
                packet.forcemN = monitor->data.forcemN;
                packet.encoderum = monitor->data.encoderum;
                packet.setpointum = monitor->data.setpoint;
                packet.timeus = monitor->data.timeus;
                packet.log = monitor->data.log;
                // printf("%d\n", packet.timeus);
                send(CMD_DATA, &packet, sizeof(MonitorDataPacket));

                break;
            }
            case CMD_STATE:
            {
                DEBUG_INFO("Sending machine state\n");
                send(CMD_STATE, machineState, sizeof(MachineState));
                break;
            }
            case CMD_MPROFILE:
            {
                DEBUG_INFO("Sending machine profile\n");
                send(CMD_MPROFILE, machineProfile, sizeof(MachineProfile));
                break;
            }
            case CMD_MCONFIG:
            {
                DEBUG_INFO("Sending machine configuration\n");
                send(CMD_MCONFIG, &(machineProfile->configuration), sizeof(MachineConfiguration));
                break;
            }
            case CMD_MPERFORMANCE:
            {
                DEBUG_INFO("Sending machine performance\n");
                send(CMD_MPERFORMANCE, &(machineProfile->performance), sizeof(MachinePerformance));
                break;
            }
            case CMD_MOTIONMODE:
            {
                DEBUG_INFO("Sending motion mode\n");
                send(CMD_MOTIONMODE, &(machineState->motionParameters.mode), sizeof(MotionMode));
                break;
            }
            case CMD_MOTIONFUNCTION:
            {
                DEBUG_INFO("Sending motion function and data\n");
                send(CMD_MOTIONFUNCTION, &(machineState->_function), sizeof(int));
                send(CMD_MOTIONFUNCTION, &(machineState->_functionData), sizeof(int));
                break;
            }
            case CMD_MOTIONSTATUS:
            {
                DEBUG_INFO("Sending motion status\n");
                send(CMD_MOTIONSTATUS, &(machineState->motionParameters.status), sizeof(int));
                break;
            }
            case CMD_TESTDATA:
            {
                DEBUG_INFO("Sending test data\n");
                uint32_t index;
                if (receive(&index, sizeof(uint32_t)))
                {
                    if (!sdcard_open("test.txt", "r"))
                    {
                        DEBUG_WARNING("Failed to open test data file\n");
                        break;
                    }
                    MonitorData buffer[10];
                    if (sdcard_read_index(buffer, sizeof(MonitorData)*1, index))
                    {
                        printf("got data\n");
                        send(CMD_TESTDATA, buffer, sizeof(MonitorData)*1);
                    }
                    else
                    {
                        DEBUG_WARNING("End of test data file\n");
                        uint8_t end = 1;
                        send(CMD_TESTDATA, &end, sizeof(uint8_t));
                    }
                    sdcard_close(); // close sd card
                }
                else
                {
                    DEBUG_WARNING("failed to receive test data's index\n");
                }
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
            DEBUG_INFO("Recieving Data\n");
            switch (cmd & ~CMD_WRITE)
            {
            case CMD_MPROFILE:
            {
                DEBUG_INFO("Getting machine profile: %d\n", sizeof(MachineProfile));
                MachineProfile temp;
                if (receive(&temp, sizeof(MachineProfile)))
                {           
                    *machineProfile = temp;
                    while (!sdcard_open("config.txt", "w"))
                    printf("waiting to write machine profiule\n"); // wait for sdcard to be ready
                    sdcard_write(machineProfile, sizeof(MachineProfile));
                    sdcard_close();
                }
                else
                {
                    DEBUG_WARNING("failed to receive machine profile\n");
                }
                break;
            }
            case CMD_MCONFIG:
            {
                DEBUG_INFO("Getting machine configuration\n");
                MachineConfiguration configuration;
                if (receive(&configuration, sizeof(MachineConfiguration)))
                {
                    machineProfile->configuration = configuration;
                    DEBUG_INFO("test var:%f\n", machineProfile->configuration.maxMotorTorque);
                }
                else
                {
                    DEBUG_WARNING("failed to receive machine configuration\n");
                }
                break;
            }
            case CMD_MPERFORMANCE:
            {
                DEBUG_INFO("Getting machine performance\n");
                MachinePerformance performance;
                if (receive(&performance, sizeof(MachinePerformance)))
                {
                    machineProfile->performance = performance;
                    DEBUG_INFO("test var:%f\n", machineProfile->performance.maxVelocity);
                }
                else
                {
                    DEBUG_WARNING("failed to receive machine performance\n");
                }
                break;
            }
            case CMD_MOTIONMODE:
            {
                //@TODO remove motion status hardcode
                state_machine_set(machineState, PARAM_MOTION_STATUS, MOTIONSTATUS_ENABLED);
                DEBUG_INFO("Getting motion mode\n");
                MotionMode mode;
                if (receive(&mode, sizeof(int)))
                {
                    state_machine_set(machineState, PARAM_MOTION_MODE, mode);
                    DEBUG_INFO("test var:%d\n", machineState->motionParameters.mode);
                }
                else
                {
                    DEBUG_WARNING("failed to receive motion mode\n");
                }
                break;
            }
            case CMD_MOTIONFUNCTION:
            {
                DEBUG_INFO("Getting motion function,data\n");
                int function;
                int data;
                if (receive(&function, sizeof(int)) && receive(&data, sizeof(int)))
                {
                    machineState->_function = function;
                    machineState->_functionData = data;
                    DEBUG_INFO("Function,data:%d,%d\n", machineState->_function, machineState->_functionData);
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
            case CMD_COMMAND:
            {
                DEBUG_INFO("Getting test command\n");
                uint8_t buf = 0;
                MotionCommand command;
                if (receive((char*)&command, sizeof(MotionCommand)))
                {
                    if (motion_test_add_move(&command))
                    {
                        buf = 0;
                        send(CMD_AWK, &buf, sizeof(uint8_t)); // send ack, 0 is success, 1 is fail, 2 is busy
                    }else
                    {
                        buf = 2;
                        //wait_for_queue = true;
                        send(CMD_AWK, &buf, sizeof(uint8_t)); // send ack, 0 is success, 1 is fail, 2 is busy
                    }
                }
                else
                {
                    buf = 1;
                    send(CMD_AWK, &buf, sizeof(uint8_t)); // send ack, 0 is success, 1 is fail, 2 is busy
                    DEBUG_WARNING("failed to receive command\n");
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