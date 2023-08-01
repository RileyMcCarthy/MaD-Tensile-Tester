#define P2_TARGET_MHZ 180
//#include <sys/p2es_clock.h>
#include "Main/MaD.h"
#include <sys/vfs.h>
#include <sys/p2es_clock.h>
#include "Encoder.h"
enum
{
    HEAPSIZE = 32400 * 2
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
    Encoder encoder;
    _pinl(PIN_SERVO_DIR);
    _pinh(CHARGE_PUMP_PIN);
    encoder.start(SERVO_ENCODER_A, SERVO_ENCODER_B, -1, false, 0, -100000, 100000);
    while(1)
    {
        printf("Encoder: %d\n", encoder.value());
        _pinl(PIN_SERVO_PUL);
        _waitms(10);
        _pinh(PIN_SERVO_PUL);
        _waitms(10);
    }
    //_clkset(_SETFREQ, _CLOCKFREQ); // Change clock freq to 180 caused errors...
    mount("/sd", _vfs_open_sdcard());
    mad_begin();
    while (1)
        ;
    return 0;
}
