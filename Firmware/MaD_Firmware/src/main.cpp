#include "MaD.h"

/**
 * @brief Main method that is called on program startup.
 * Begins MaD Board instance
 * 
 * @return int 
 */

/**
 * @brief Global instance of MaD Board object
 * 
 */
static MAD mad;
int main()
{
    mad.begin();
    while (1)
        ;
}