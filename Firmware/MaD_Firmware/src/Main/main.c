#define P2_TARGET_MHZ 180
//#include <sys/p2es_clock.h>
#include "Main/MaD.h"
#include <sys/vfs.h>
enum
{
    HEAPSIZE = 32400 * 1
};

double atof(const char *str)
{
    int sign;
    double number = 0.0, power = 1.0;

    while (isspace(*str))
        ++str;

    sign = (*str == '-') ? -1 : 1; // Save the sign

    if (*str == '-' || *str == '+') // Skip the sign
        str++;

    while (isdigit(*str)) // Digits before the decimal point
    {
        number = 10.0 * number + (*str - '0');
        str++;
    }

    if (*str == '.') // Skip the decimal point
        str++;

    while (isdigit(*str)) // Digits after the decimal point
    {
        number = 10.0 * number + (*str - '0');
        power *= 10.0;
        str++;
    }

    return sign * number / power;
}

/**
 * @brief Main method that is called on program startup.
 * Begins MaD Board instance
 *
 * @return int
 */
// all stdio must be done in main thread (or same cog)
int main()
{
    //_clkset(_SETFREQ, _CLOCKFREQ);
    mount("/sd", _vfs_open_sdcard());
   mad_begin();
    while (1)
        ;
    return 0;
}
