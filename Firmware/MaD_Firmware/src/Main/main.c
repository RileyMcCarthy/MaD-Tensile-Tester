#include "Main/MaD.h"
#include <sys/vfs.h>
#include "Encoder.h"
enum
{
    HEAPSIZE = 32400 * 4
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
    mount("/sd", _vfs_open_sdcard());
    mad_begin();
    while (1)
        ;
    return 0;
}
