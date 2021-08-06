#ifndef Error_H
#define Error_H
#ifdef __cplusplus
extern "C"
{
#endif
    /**
 * @brief Error codes:
 * 0: Generic Success
 * 1: Generic Failure
 * 2-50: Display Error Codes
 * 51-100: RTC Error Codes
 * 101-150: DYN4 Error Codes
 */
    typedef enum Error_e
    {
        SUCCESS, //Generic Success
        FAIL,    //Generic failure

        DISPLAY_NOT_FOUND = 2, //Display RA8876 chip not responding
        DISPLAY_PLL_FAIL,      //PLL status fail
        DISPLAY_SDRAM_FAIL,    //SDRAM status fail

        RTC_RESET = 51, //RTC power was lost, reset time
        RTC_NOT_FOUND,  //RTC communication failed

        DYN4_NOT_RESPONDING = 101,
        DYN4_CONNECTION_LOST,
        DYN4_COG_FAIL,

        FORCEGAUGE_NOT_RESPONDING = 151,
        FORCEGAUGE_CONNECTION_LOST,
        FORCEGAUGE_COG_FAIL,

    } Error;
#ifdef __cplusplus
}
#endif
#endif
