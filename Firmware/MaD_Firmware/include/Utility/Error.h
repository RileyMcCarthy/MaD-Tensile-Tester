#ifndef Error_H
#define Error_H

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
    SUCCESS, // Generic Success
    FAIL,    // Generic failure

    DISPLAY_NOT_FOUND = 2, // Display RA8876 chip not responding
    DISPLAY_PLL_FAIL,      // PLL status fail
    DISPLAY_SDRAM_FAIL,    // SDRAM status fail

    RTC_RESET = 51, // RTC power was lost, reset time
    RTC_NOT_FOUND,  // RTC communication failed

    DYN4_NOT_RESPONDING = 101,
    DYN4_CONNECTION_LOST,
    DYN4_COG_FAIL,

    FORCEGAUGE_NOT_RESPONDING = 151,
    FORCEGAUGE_CONNECTION_LOST,
    FORCEGAUGE_COG_FAIL,

    JSON_FILE_ERROR = 201,
    JSON_FILE_NOT_FOUND,
    JSON_FILE_READ_FAIL,
    JSON_FILE_WRITE_FAIL,
    JSON_FILE_INVALID,
    JSON_FILE_EMPTY,
    JSON_FILE_NOT_VALID,

    MCP23017_NOT_FOUND = 251,

    W25QXX_NOT_FOUND = 301,
    W25QXX_NOT_AVAILABLE,
    W25QXX_INVALID_ID,

    SEM_NOT_AVAILABLE = 351
} Error;

void print_error(Error value);
void seterror(Error *err, Error value);

#endif
