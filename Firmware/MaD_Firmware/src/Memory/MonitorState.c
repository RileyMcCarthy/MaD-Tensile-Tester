#include "Memory/MonitorState.h"
#include "Utility/Debug.h"
#include <stddef.h>

static MonitorData monitor_data;
static MonitorSDCard monitor_sd_card; 
static int _monitor_sd_lock = -1;
// can use boolean because only one thread is locking
// bytes are written atomically
static bool _monitor_data_lock = false;

int init_monitor_state()
{
    monitor_sd_card.sd_card_state = SD_CARD_IDLE;
    monitor_sd_card.read_data_index = -1;
    monitor_sd_card.read_data_count = 0;
    monitor_sd_card.is_writing = false;

    monitor_data.log = 0;
    
    if (_monitor_sd_lock == -1)
    {
        _monitor_sd_lock = _locknew();
    }

    _monitor_data_lock = false;

    return _monitor_sd_lock;
}

bool lock_monitor_data(MonitorData ** data)
{
    if (_monitor_data_lock)
    {
        *data = NULL;
        return false;
    }
    _monitor_data_lock = true;

    *data = &monitor_data;

    return true;
}

bool lock_monitor_data_ms(MonitorData ** data, int ms)
{
    int start = _getms();
    while (!lock_monitor_data(data))
    {
        if (_getms() - start > ms)
        {
            DEBUG_ERROR("Failed to lock monitor state, waited %dms\n", ms);
            return false;
        }
    }
    return true;
}

bool unlock_monitor_data()
{
    if (!_monitor_data_lock )
    {
        return false;
    }

    _monitor_data_lock = false;
    return true;
}

bool lock_sd_card(MonitorSDCard ** data)
{
    if (_monitor_sd_lock == -1)
    {
        DEBUG_ERROR("%s","Monitor state lock not initialized\n");
        return false;
    }
    if (_locktry(_monitor_sd_lock) == 0)
    {
        *data = NULL;
        return false;
    }

    *data = &monitor_sd_card;
    return true;
}

bool lock_sd_card_ms(MonitorSDCard ** data, int ms)
{
    int start = _getms();
    while (!lock_sd_card(data))
    {
        if (_getms() - start > ms)
        {
            DEBUG_ERROR("Failed to lock monitor state for sd card, waited %dms\n", ms);
            return false;
        }
    }
    return true;
}

SDCardState * unlock_and_monitor_sd_card()
{
    MonitorSDCard * sdcard;
    if (!lock_sd_card(&sdcard))
    {
        DEBUG_ERROR("%s","Resource allocated, this should not happen!\n");
        return false;
    }

    _lockrel(_monitor_sd_lock);
    return &(monitor_sd_card.sd_card_state);
}

bool unlock_sd_card()
{
    MonitorSDCard * sdcard;
    if (!lock_sd_card_ms(&sdcard,10))
    {
        DEBUG_ERROR("%s","Resource allocated, this should not happen!\n");
        return false;
    }

    sdcard->sd_card_state = SD_CARD_IDLE;
    _lockrel(_monitor_sd_lock);
    return true;
}

