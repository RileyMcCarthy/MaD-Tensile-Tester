#define P2_TARGET_MHZ 180
//#include <sys/p2es_clock.h>
#include "Main/MaD.h"
#include <sys/vfs.h>
#include <sys/p2es_clock.h>
enum
{
    HEAPSIZE = 32400 * 1
};

/**
 * @brief Main method that is called on program startup.
 * Begins MaD Board instance
 *
 * @return int
 */
// all stdio must be done in main thread (or same cog)
int main()
{
    //_clkset(_SETFREQ, _CLOCKFREQ); // Change clock freq to 180 caused errors...
    mount("/sd", _vfs_open_sdcard());
    mad_begin();
    while (1)
        ;
    return 0;
}
