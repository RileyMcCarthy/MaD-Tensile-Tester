#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdbool.h>
#include <stdint.h>
#include "Utility/Monitor.h"
#include "Utility/ControlSystem.h"

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
#define MAD_VERSION 1


#define NOTIFICATION_ERROR "ERROR"
#define NOTIFICATION_WARNING "WARNING"
#define NOTIFICATION_INFO "INFO"
#define NOTIFICATION_SUCCESS "SUCCESS"

#define MAX_SIZE_NOTIFICATION_BUFFER 20
#define MAX_SIZE_NOTIFICATION_TYPE 15
#define MAX_SIZE_NOTIFICATION_MESSAGE 100
typedef struct Notification {
    char type[MAX_SIZE_NOTIFICATION_TYPE];
    char message[MAX_SIZE_NOTIFICATION_MESSAGE];
} Notification;


void beginCommunication(void *arg);

#endif
