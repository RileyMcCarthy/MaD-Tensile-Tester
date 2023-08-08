#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include "StaticQueue.h"
#include"Main/Communication/Communication.h"

#ifndef __WORKSPACE__
#define __WORKSPACE__ ""
#endif

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

extern _stdio_debug_lock;

#define DEBUG_PRINTF_RAW(color, fmt, ...) while (!_locktry(_stdio_debug_lock));\
fprintf(stdout, color "%s:%d: " fmt ANSI_COLOR_RESET,__FILE__+strlen(__WORKSPACE__), __LINE__,__VA_ARGS__);\
_lockrel(_stdio_debug_lock);

#define DEBUG_SERIAL_COMM(type, fmt, ...) notification_add_debug(type, "%s:%d:: " fmt, __FILE__+strlen(__WORKSPACE__), __LINE__, __VA_ARGS__);

#if defined(_DEBUG_WARNING) && defined(_DEBUG_PRINTF_RAW) && defined(_DEBUG_SERIAL_COMM)
#define DEBUG_WARNING(fmt, ...) DEBUG_SERIAL_COMM(NOTIFICATION_WARNING, fmt, __VA_ARGS__); DEBUG_PRINTF_RAW(ANSI_COLOR_YELLOW, fmt, __VA_ARGS__);
#elif defined(_DEBUG_WARNING) && defined(_DEBUG_PRINTF_RAW)
#define DEBUG_WARNING(fmt, ...) DEBUG_PRINTF_RAW(ANSI_COLOR_YELLOW, fmt, __VA_ARGS__);
#elif defined(_DEBUG_WARNING) && defined(_DEBUG_SERIAL_COMM)
#define DEBUG_WARNING(fmt, ...) DEBUG_SERIAL_COMM(NOTIFICATION_WARNING, fmt, __VA_ARGS__);
#else
#define DEBUG_WARNING(...)
#endif

#if defined(_DEBUG_INFO) && defined(_DEBUG_PRINTF_RAW)
#define DEBUG_INFO(fmt, ...) DEBUG_PRINTF_RAW(ANSI_COLOR_GREEN, fmt, __VA_ARGS__);
#else
#define DEBUG_INFO(...)
#endif

#if defined(_DEBUG_SERIAL_COMM)
#define DEBUG_NOTIFY(fmt, ...) DEBUG_SERIAL_COMM(NOTIFICATION_INFO, fmt, __VA_ARGS__);
#else
#define DEBUG_NOTIFY(...)
#endif

#if defined (_DEBUG_ERROR) && defined(_DEBUG_PRINTF_RAW) && defined(_DEBUG_SERIAL_COMM)
#define DEBUG_ERROR(fmt, ...) DEBUG_SERIAL_COMM(NOTIFICATION_ERROR, fmt, __VA_ARGS__); DEBUG_PRINTF_RAW(ANSI_COLOR_RED, fmt, __VA_ARGS__);
#elif defined (_DEBUG_ERROR) &&  defined(_DEBUG_PRINTF_RAW)
#define DEBUG_ERROR(fmt, ...) DEBUG_PRINTF_RAW(ANSI_COLOR_RED, fmt, __VA_ARGS__);
#elif defined (_DEBUG_ERROR) && defined(_DEBUG_SERIAL_COMM)
#define DEBUG_ERROR(fmt, ...) DEBUG_SERIAL_COMM(NOTIFICATION_ERROR, fmt, __VA_ARGS__);
#else
#define DEBUG_ERROR(...)
#endif

#endif
