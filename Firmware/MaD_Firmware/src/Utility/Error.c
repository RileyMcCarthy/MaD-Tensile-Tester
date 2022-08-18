#include "Error.h"
#include <stdlib.h>

void print_error(Error value)
{
    switch (value)
    {
    case SUCCESS:
        printf("SUCCESS\n");
        break;
    case FAIL:
        printf("FAIL\n");
        break;
    case DISPLAY_NOT_FOUND:
        printf("DISPLAY_NOT_FOUND\n");
        break;
    case DISPLAY_PLL_FAIL:
        printf("DISPLAY_PLL_FAIL\n");
        break;
    case DISPLAY_SDRAM_FAIL:
        printf("DISPLAY_SDRAM_FAIL\n");
        break;
    case RTC_RESET:
        printf("RTC_RESET\n");
        break;
    case RTC_NOT_FOUND:
        printf("RTC_NOT_FOUND\n");
        break;
    case DYN4_NOT_RESPONDING:
        printf("DYN4_NOT_RESPONDING\n");
        break;
    case DYN4_CONNECTION_LOST:
        printf("DYN4_CONNECTION_LOST\n");
        break;
    case DYN4_COG_FAIL:
        printf("DYN4_COG_FAIL\n");
        break;
    case FORCEGAUGE_NOT_RESPONDING:
        printf("FORCEGAUGE_NOT_RESPONDING\n");
        break;
    case FORCEGAUGE_CONNECTION_LOST:
        printf("FORCEGAUGE_CONNECTION_LOST\n");
        break;
    case FORCEGAUGE_COG_FAIL:
        printf("FORCEGAUGE_COG_FAIL\n");
        break;
    case JSON_FILE_ERROR:
        printf("JSON_FILE_ERROR\n");
        break;
    case JSON_FILE_NOT_FOUND:
        printf("JSON_FILE_NOT_FOUND\n");
        break;
    case JSON_FILE_READ_FAIL:
        printf("JSON_FILE_READ_FAIL\n");
        break;
    case JSON_FILE_WRITE_FAIL:
        printf("JSON_FILE_WRITE_FAIL\n");
        break;
    case JSON_FILE_INVALID:
        printf("JSON_FILE_INVALID\n");
        break;
    case JSON_FILE_EMPTY:
        printf("JSON_FILE_EMPTY\n");
        break;
    case JSON_FILE_NOT_VALID:
        printf("JSON_FILE_NOT_VALID\n");
        break;
    case MCP23017_NOT_FOUND:
        printf("MCP23017_NOT_FOUND\n");
        break;
    default:
        printf("UNKNOWN ERROR\n");
        break;
    }
}

inline void seterror(Error *err, Error value)
{
    if (err != NULL)
    {
        *err = value;
    }
}
