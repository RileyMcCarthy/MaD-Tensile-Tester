#include "Memory/CogStatus.h"
#include "Utility/Debug.h"

#define COG_STATUS_TIMOUT 1000

static int _cog_motion_status = -1;
static int _cog_control_status = -1;
static int _cog_communication_status = -1;
static int _cog_monitor_status = -1;

//each cog should write currentms to memory and we can check if it is still running
// blink led for the cog that dies if it dies


void set_motion_status(int statusms)
{
    _cog_motion_status = statusms;
}

void set_control_status(int statusms)
{
    _cog_control_status = statusms;
}

void set_communication_status(int statusms)
{
    _cog_communication_status = statusms;
}

void set_monitor_status(int statusms)
{
    _cog_monitor_status = statusms;
}

static _last_debug_sent = 0;

bool get_machine_status()
{

    bool motion_status = (_getms()-_cog_motion_status) < COG_STATUS_TIMOUT;
    bool control_status = (_getms()-_cog_control_status) < COG_STATUS_TIMOUT;
    bool communication_status = (_getms()-_cog_communication_status) < COG_STATUS_TIMOUT;
    bool monitor_status = (_getms()-_cog_monitor_status) < COG_STATUS_TIMOUT;
    
    bool machine_status = motion_status && control_status && communication_status && monitor_status;
    if (!machine_status && (_getms() - _last_debug_sent) > 5000)
    {
        _last_debug_sent = _getms();
        DEBUG_ERROR("Machine Status Error: Motion: %d, Control: %d, Communication: %d, Monitor: %d\n", motion_status, control_status, communication_status, monitor_status);
    }
    return machine_status;
}



