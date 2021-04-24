#ifndef Error_H
#define Error_H
/**
 * @brief Error codes:
 * 0: Generic Success
 * 1: Generic Failure
 * 2-50: Display Error Codes
 * 51-100: RTC Error Codes
 */
enum Error
{
    SUCCESS, //Generic Success
    FAIL,    //Generic failure

    DISPLAY_NOT_FOUND = 2, //Display RA8876 chip not responding
    DISPLAY_PLL_FAIL,      //PLL status fail
    DISPLAY_SDRAM_FAIL,    //SDRAM status fail

    RTC_RESET = 51, //RTC power was lost, reset time
    RTC_NOT_FOUND,  //RTC communication failed
};

#endif