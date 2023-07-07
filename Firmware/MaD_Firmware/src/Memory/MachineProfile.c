#include "Memory/MachineProfile.h"
#include "Utility/Debug.h"
#include <stddef.h>

static MachineProfile machine_profile;
static int _machine_profile_lock = -1;

int init_machine_profile()
{
    if (_machine_profile_lock == -1)
    {
        _machine_profile_lock = _locknew();
    }

    if (_machine_profile_lock == -1)
    {
        DEBUG_ERROR("%s","Failed to create machine profile lock\n");
        return -1;
    }

    return _machine_profile_lock;
}

bool lock_machine_profile(MachineProfile ** profile)
{
    if (_machine_profile_lock == -1)
    {
        DEBUG_ERROR("Machine profile lock not initialized: %d\n", _machine_profile_lock);
        return false;
    }
    if (_locktry(_machine_profile_lock) == 0)
    {
        *profile = NULL;
        return false;
    }

    *profile = &machine_profile;

    return true;
}

bool lock_machine_profile_ms(MachineProfile ** profile, int ms)
{
    int start = _getms();
    while (!lock_machine_profile(profile))
    {
        if (_getms() - start > ms)
        {
            DEBUG_ERROR("%s","Failed to lock machine profile, waited %dms\n", ms);
            return false;
        }
    }

    return true;
}

bool unlock_machine_profile()
{
    if (_machine_profile_lock == -1)
    {
        return false;
    }

    _lockrel(_machine_profile_lock);
    return true;
}


