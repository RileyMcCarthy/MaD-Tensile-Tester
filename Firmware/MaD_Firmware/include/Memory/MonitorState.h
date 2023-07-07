#include "MachineProfile.h"

typedef enum SDCardState
{
  SD_CARD_SUCCESS,
  SD_CARD_IDLE, // waiting for next command, write or read is comleted!
  SD_CARD_READ_MACHINE_PROFILE, // read machine profile from SD card, updated global struct (locked)
  SD_CARD_READ_DATA,
  SD_CARD_READ_DATA_SIZE,
  SD_CARD_WRITE_DATA,
  SD_CARD_WRITE_MACHINE_PROFILE,
  SD_CARD_FILE_EXISTS,
  SD_CARD_ERROR
} SDCardState;

typedef struct MonitorData
{
    int forceRaw;        // Raw force value
    int encoderRaw;      // Raw encoder value
    int forcemN;         // Force in mN
    int encoderum;       // Encoder in um
    double force;        // Force value in N
    double position;     // Position value in mm
    int setpoint;        // Setpoint in um
    unsigned int timems; // time in ms
    unsigned int timeus; // time in us
    int log;
} MonitorData;

typedef struct MonitorSDCard
{
  SDCardState sd_card_state;
  MachineProfile sd_card_profile; // Storage for machine profile read from SD card
  MonitorData sd_card_data[255]; // Storage for sd card data read from SD card
  char sd_card_file_name[255]; // Storage for file name read from SD card
  int read_data_index;
  int read_data_count;

  bool is_writing;
} MonitorSDCard;

int init_monitor_data();
bool lock_monitor_data(MonitorData ** data);
bool lock_monitor_data_ms(MonitorData ** data, int ms);
bool unlock_monitor_data();
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
bool lock_sd_card(MonitorSDCard ** data);
bool lock_sd_card_ms(MonitorSDCard ** data, int ms);
SDCardState * unlock_and_monitor_sd_card();
bool lock_sd_card();
