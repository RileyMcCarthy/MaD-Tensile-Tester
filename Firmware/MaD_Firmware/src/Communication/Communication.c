#include "Main/Communication/Communication.h"
#include "Utility/StateMachine.h"
#include "Utility/JsonEncoder.h"
#include "Utility/JsonDecoder.h"
#include "Utility/Debug.h"
#include "Utility/Motion.h"
#include "Utility/Monitor.h"
#include <stdlib.h>
#include "StaticQueue.h"
#include <propeller.h>
#include "Memory/MachineProfile.h"
#include "Main/MaD.h"
#include "Main/Communication/CRC.h"
/* Command structure
 * w<7 cmd bits> <N> <N data>... <CRC>
 * ________ ________ ________
 * <data> = raw data send/recieved depending on w bit
 * <CRC> = check
 */

#define COMMUNICATION_MEMORY_SIZE 3000
static long comm_stack[COMMUNICATION_MEMORY_SIZE];
typedef struct __using("lib/Protocol/jm_fullduplexserial.spin2") FDS;

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
#define CMD_MOVE 10     // start/send sending motion data
#define CMD_AWK 11        // send/recieve AWK
#define CMD_TESTDATA 12   // send/recieve test data
#define CMD_TESTDATA_COUNT 13 // send/recieve test data count
#define CMD_MANUAL 14 // send/recieve manual control data
#define CMD_SET_GAUGE 15

#define MAD_VERSION 1
static bool command_to_string(char *buf, int size, uint8_t cmd)
{
    switch(cmd)
    {
        case CMD_PING:
            snprintf(buf, size, "CMD_PING");
            break;
        case CMD_DATA:
            snprintf(buf, size, "CMD_DATA");
            break;
        case CMD_STATE:
            snprintf(buf, size, "CMD_STATE");
            break;
        case CMD_MPROFILE:
            snprintf(buf, size, "CMD_MPROFILE");
            break;
        case CMD_MCONFIG:
            snprintf(buf, size, "CMD_MCONFIG");
            break;
        case CMD_MPERFORMANCE:
            snprintf(buf, size, "CMD_MPERFORMANCE");
            break;
        case CMD_MOTIONPROFILE:
            snprintf(buf, size, "CMD_MOTIONPROFILE");
            break;
        case CMD_MOTIONMODE:
            snprintf(buf, size, "CMD_MOTIONMODE");
            break;
        case CMD_MOTIONFUNCTION:
            snprintf(buf, size, "CMD_MOTIONFUNCTION");
            break;
        case CMD_MOTIONSTATUS:
            snprintf(buf, size, "CMD_MOTIONSTATUS");
            break;
        case CMD_MOVE:
            snprintf(buf, size, "CMD_MOVE");
            break;
        case CMD_AWK:
            snprintf(buf, size, "CMD_AWK");
            break;
        case CMD_TESTDATA:
            snprintf(buf, size, "CMD_TESTDATA");
            break;
        case CMD_TESTDATA_COUNT:
            snprintf(buf, size, "CMD_TESTDATA_COUNT");
            break; 
        default:
            snprintf(buf, size, "UNKNOWN");
            return false;
    }
    return true;
}

// @TODO RETURN CHECKSUM FOR VALIDATION IT WAS RECIEVED CORRECTLY
static uint16_t receive(char *buf, int max_size)
{
    if (buf == NULL)
    {
        return 0;
    }

    // Read data size
    uint16_t size = fds.rxtime(10);
    if (size == -1)
    {
        DEBUG_WARNING("%s", "invalid data recieved\n");
        return 0;
    }

    size |= fds.rxtime(10) << 8;
    if (size == -1)
    {
        DEBUG_WARNING("%s","invalid data recieved\n");
        return 0;
    }

    DEBUG_INFO("Recieved data of size: %d\n", size);

    if (size > max_size-1)
    {
        DEBUG_WARNING("invalid data recieved, data is larger then buffer: buf=%d, max=%d\n", size, max_size);
        return 0;
    }
    // Read data
    for (unsigned int i = 0; i < size; i++)
    {
        buf[i] = fds.rxtime(10);
        if (buf[i] == -1)
        {
            DEBUG_WARNING("%s","invalid data recieved\n");
            return 0;
        }
    }

    // Read CRC
    uint8_t crc = fds.rxtime(10);
    if (crc == -1)
    {
        DEBUG_WARNING("%s","invalid data recieved\n");
        return 0;
    }

    // Check CRC
    if (crc != crc8(buf, size))
    {
        DEBUG_WARNING("%s","invalid data recieved\n");
        return 0;
    }

    return size;
}

// copying shouldnt need to happen, lock data before sending it...

static bool send(int cmd, char *buf, uint16_t size)
{
    DEBUG_INFO("Sending data of size: %d\n", size);

    fds.tx(0x55);
    fds.tx(cmd);
    fds.tx(size);
    fds.tx(size >> 8);

    for (int i = 0; i < size; i++)
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
        int res;
        //DEBUG_INFO("%s","about to wait\n");
        while ((res = fds.rxtime(1000)) != 0x55)
        {
            
        }
        int cmd = fds.rxtime(10);
        if (cmd != -1)
        {
            //DEBUG_INFO("GOT CMD: %d\n", cmd);
            return cmd;
        }
    }
}

static void load_machine_profile()
{
    DEBUG_INFO("%s","Loading machine profile\n");
    if (!sd_profile_exists())
    {
        // Load default profile, one does not exist
        DEBUG_WARNING("%s","No machine profile found, loading default\n");
        MachineProfile temp_profile;
        memset(&temp_profile, 0, sizeof(MachineProfile));
        strcpy(temp_profile.name,"DEFAULT");
        if (!write_sd_profile(&temp_profile))
        {
            DEBUG_ERROR("%s","Failed to write machine profile!\n");
        }
    }
    
    DEBUG_INFO("%s","Reading sd profile from sd\n");
    MachineProfile machine_profile_temp;
    if (!read_sd_profile(&machine_profile_temp))
    {
        DEBUG_ERROR("%s","Failed to read machine profile from SD card, default should have been loaded!\n");
    }

    DEBUG_INFO("%s","Loading machine profile from SD card\n");
    
    // Load profile from SD card
    MachineProfile *machine_profile;
    if (!lock_machine_profile_ms(&machine_profile, 100))
    {
        DEBUG_ERROR("%s","Failed to lock machine profile, default should have been loaded!\n");
        return;
    }
    memcpy(machine_profile, &machine_profile_temp, sizeof(MachineProfile));
    unlock_machine_profile();

    set_machine_profile_loaded(true);    
    DEBUG_INFO("%s","Machine profile loaded\n");
    return;
    
}

#define MAX_BUFFER_SIZE 1000
static char recieved_json[MAX_BUFFER_SIZE];
static void command_respond(uint8_t cmd)
{
    char cmd_str[30];
    //command_to_string(cmd_str, 30, cmd); THIS WOULD BREAK IT< IDK WHY WTF?
    DEBUG_INFO("Responding to command: %d\n",cmd);
    switch (cmd)
    {
    case CMD_PING:
    {
        DEBUG_INFO("%s","Sending ping with firmware version\n");
        char *res = "{\"version\": \"1.0.0\"}";
        send(CMD_PING, res, strlen(res)+1);
        break;
    }
    case CMD_AWK:
    {
        DEBUG_INFO("%s","Sending delayed awk\n");
        uint8_t awk = 0;
        char buf[100];
        snprintf(buf, 100, "{\"awk\": %d}",awk);
        send(CMD_AWK, buf, strlen(buf));
        break;
    }
    case CMD_DATA:
    {
        MonitorData monitor_data;
        get_monitor_data(&monitor_data, 10);
        DEBUG_INFO("Sending Data (%d)\n", monitor_data.log);
        char buf[300];
        snprintf(buf, 300, "{\"Force\":%d,\"Position\":%d,\"Setpoint\":%d,\"Time\":%d,\"Log\":%d, \"Raw\":%d}",
            monitor_data.forcemN, monitor_data.encoderum, monitor_data.setpoint, monitor_data.timeus, monitor_data.log,monitor_data.forceRaw);
        send(CMD_DATA, buf, strlen(buf)); // dont send null ptr

        break;
    }
    case CMD_STATE:
    {
        DEBUG_INFO("%s","Sending machine state\n");
        MachineState machine_state;
        get_machine_state(&machine_state);
        char *buf = machine_state_to_json(&machine_state);
        if (buf == NULL)
        {
            DEBUG_ERROR("%s","Failed to convert machine state to json\n");
            return;
        }
        send(CMD_STATE, buf, strlen(buf));
        unlock_json_buffer();
        _waitms(10);
        break;
    }
    case CMD_MPROFILE:
    {
        DEBUG_INFO("%s","Sending machine profile\n");

        // Access machine profile in memory
        MachineProfile *machine_profile;
        if (!lock_machine_profile_ms(&machine_profile, 10))
        {
            return;
        }
    
        MachineProfile machine_profile_copy;
        memcpy(&machine_profile_copy, machine_profile, sizeof(MachineProfile));
        
        unlock_machine_profile();

        // Convert to json
        char *buf = machine_profile_to_json(&machine_profile_copy);
        if (buf == NULL)
        {
            DEBUG_ERROR("%s","Failed to convert machine profile to json\n");
            return;
        }
        DEBUG_INFO("Sending machine profile: %s\n", buf);
        send(CMD_MPROFILE, buf, strlen(buf));
        unlock_json_buffer();
        break;
    }
    case CMD_TESTDATA:
    {
        DEBUG_INFO("%s","Sending test data\n");
        uint32_t index;
        uint8_t count;

        TestDataRequest req;
        json_to_test_data_request(&req, recieved_json);

        MonitorData buffer[255];
        if (read_sd_card_data(buffer, req.index, req.count) != 0)
        {
            char *buf = test_data_to_json(buffer, req.count, req.index);
            if (buf == NULL)
            {
                DEBUG_ERROR("%s","Failed to convert test data to json\n");
                return;
            }
            send(CMD_TESTDATA, buf, strlen(buf));
            unlock_json_buffer();
        }
        break;
    }
    case CMD_TESTDATA_COUNT:
    {
        // send the number of test data points
        DEBUG_INFO("%s","Sending test data count\n");
        int count = read_data_size();
        char buf[100];
        snprintf(buf, 100, "{\"test_count\":%d}", count);
        send(CMD_TESTDATA_COUNT, buf, strlen(buf));
        break;
    }
    case CMD_SET_GAUGE:
    {
        DEBUG_WARNING("%s","Setting gauge\n");
        set_gauge_length();
        break;
    }
    default:
    {
        DEBUG_WARNING("%s","Write Command not found\n");
        break;
    }
    }
}

void command_recieve(uint8_t cmd)
{
    DEBUG_INFO("%s","Recieving Data from command\n");
    if (!receive(recieved_json, MAX_BUFFER_SIZE))
    {
        DEBUG_WARNING("%s","failed to receive command data\n");
        return;
    }
    DEBUG_INFO("Recieved: %s\n", recieved_json);
    switch (cmd)
    {
    case CMD_MPROFILE:
    {
        DEBUG_INFO("%s","Getting machine profile\n");
        MachineProfile temp;
        if (!json_to_machine_profile(&temp, recieved_json))
        {
            DEBUG_WARNING("%s","failed to parse machine profile\n");
            break;
        }

        MachineProfile *machine_profile;
        if (!lock_machine_profile_ms(&machine_profile, 10))
        {
            return;
        }

        memcpy(machine_profile, &temp, sizeof(MachineProfile));
        write_sd_profile(machine_profile);

        unlock_machine_profile();
        DEBUG_INFO("%s","Machine profile updated\n");
        break;
    }
    case CMD_MOTIONMODE:
    {
        //@TODO remove motion status hardcode
        state_machine_set(PARAM_MOTION_STATUS, MOTIONSTATUS_ENABLED);
        DEBUG_INFO("%s","Getting motion mode\n");

        MotionMode mode;
        if (!json_to_motion_mode(&mode, recieved_json))
        {
            DEBUG_WARNING("%s","failed to parse motion mode\n");
            break;
        }

        DEBUG_INFO("Setting motion mode: %d\n", mode);
        state_machine_set(PARAM_MOTION_MODE, mode);
        break;
    }
    case CMD_MOTIONSTATUS:
    {
        DEBUG_INFO("%s","Getting motion status\n");
        MotionStatus status;

        if (!json_to_motion_status(&status, recieved_json))
        {
            DEBUG_WARNING("%s","failed to parse motion status\n");
            break;
        }

        state_machine_set(PARAM_MOTION_STATUS, status);

        break;
    }
    case CMD_MOVE:
    {
        DEBUG_WARNING("%s","Getting test command\n");
        char awk[30];
        strncpy(awk, "OK", 30);

        Move move;
        if (json_to_move(&move, recieved_json))
        {
            if (motion_test_add_move(&move))
            {
                DEBUG_WARNING("%s","move added\n");
                strncpy(awk, "OK", 30);
            }
            else
            {
                strncpy(awk, "BUSY", 30);
                DEBUG_WARNING("%s","failed to add move\n");
            }
        }
        else
        {
            strncpy(awk, "FAIL", 30);
            DEBUG_ERROR("%s","failed to parse test command\n");
            break;
        }
        
        char buf[100];
        snprintf(buf, 100, "{\"awk\": \"%s\"}", awk);

        send(CMD_AWK, buf, strlen(buf)); // send ack, 0 is success, 1 is fail, 2 is busy
        break;
    }
    case CMD_MANUAL:
    {
        DEBUG_INFO("%s","Getting manual command\n");

        Move move;
        if (json_to_move(&move, recieved_json))
        {
            if (motion_add_move(&move))
            {
                DEBUG_INFO("Adding move G:%d X%d F%d\n", move.g, move.x, move.f);
            }
            else
            {
                DEBUG_WARNING("%s","failed to add move\n");
                break;
            }
        }
        else
        {
            DEBUG_ERROR("%s","failed to parse manual command\n");
            break;
        }
        break;
    }
    default:
    {
        DEBUG_WARNING("%s","unknown command\n");
        break;
    }
    }
}

static void beginCommunication(void *arg)
{
    load_machine_profile();
    // Begin main loop
    fds.start(RPI_RX, RPI_TX, 0, 115200);
    while (1)
    {
        DEBUG_INFO("%s","Waiting for command\n");
        int cmd = recieveCMD();
        DEBUG_INFO("cmd:%d,write:%d\n", (cmd & ~CMD_WRITE), ((cmd & CMD_WRITE) == CMD_WRITE));
        if ((cmd & CMD_WRITE) != CMD_WRITE)
        {
            command_respond(cmd);
        }
        else
        {
            command_recieve(cmd & ~CMD_WRITE);
        }
    }
}

bool start_communication()
{
    // Start communication cog
    int id = _cogstart_C(beginCommunication, NULL, &comm_stack[0], sizeof(long) * COMMUNICATION_MEMORY_SIZE);
    return id != -1;
}
