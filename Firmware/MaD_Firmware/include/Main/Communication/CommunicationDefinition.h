#ifndef COMMDEF_H
#define COMMDEF_H

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

#define MAD_VERSION 1.0

#endif
