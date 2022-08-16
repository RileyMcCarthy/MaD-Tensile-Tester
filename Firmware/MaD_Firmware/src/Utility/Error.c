#include "Error.h"
#include <stdlib.h>

void print_error(Error value)
{
    switch (value)
    {
    case SUCCESS:
        serial_debug("SUCCESS\n");
        break;
    case FAIL:
        serial_debug("FAIL\n");
        break;
    case DISPLAY_NOT_FOUND:
        serial_debug("DISPLAY_NOT_FOUND\n");
        break;
    case DISPLAY_PLL_FAIL:
        serial_debug("DISPLAY_PLL_FAIL\n");
        break;
    case DISPLAY_SDRAM_FAIL:
        serial_debug("DISPLAY_SDRAM_FAIL\n");
        break;
    case RTC_RESET:
        serial_debug("RTC_RESET\n");
        break;
    case RTC_NOT_FOUND:
        serial_debug("RTC_NOT_FOUND\n");
        break;
    case DYN4_NOT_RESPONDING:
        serial_debug("DYN4_NOT_RESPONDING\n");
        break;
    case DYN4_CONNECTION_LOST:
        serial_debug("DYN4_CONNECTION_LOST\n");
        break;
    case DYN4_COG_FAIL:
        serial_debug("DYN4_COG_FAIL\n");
        break;
    case FORCEGAUGE_NOT_RESPONDING:
        serial_debug("FORCEGAUGE_NOT_RESPONDING\n");
        break;
    case FORCEGAUGE_CONNECTION_LOST:
        serial_debug("FORCEGAUGE_CONNECTION_LOST\n");
        break;
    case FORCEGAUGE_COG_FAIL:
        serial_debug("FORCEGAUGE_COG_FAIL\n");
        break;
    case JSON_FILE_ERROR:
        serial_debug("JSON_FILE_ERROR\n");
        break;
    case JSON_FILE_NOT_FOUND:
        serial_debug("JSON_FILE_NOT_FOUND\n");
        break;
    case JSON_FILE_READ_FAIL:
        serial_debug("JSON_FILE_READ_FAIL\n");
        break;
    case JSON_FILE_WRITE_FAIL:
        serial_debug("JSON_FILE_WRITE_FAIL\n");
        break;
    case JSON_FILE_INVALID:
        serial_debug("JSON_FILE_INVALID\n");
        break;
    case JSON_FILE_EMPTY:
        serial_debug("JSON_FILE_EMPTY\n");
        break;
    case JSON_FILE_NOT_VALID:
        serial_debug("JSON_FILE_NOT_VALID\n");
        break;
    case MCP23017_NOT_FOUND:
        serial_debug("MCP23017_NOT_FOUND\n");
        break;
    default:
        serial_debug("UNKNOWN ERROR\n");
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
