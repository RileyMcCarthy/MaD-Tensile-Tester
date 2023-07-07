#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

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

#ifdef _DEBUG_WARNING
#define DEBUG_WARNING(fmt, ...) while (!_locktry(_stdio_debug_lock)); fprintf(stdout, ANSI_COLOR_YELLOW "%s:%d: " fmt ANSI_COLOR_RESET,__FILE__+strlen(__WORKSPACE__), __LINE__, __VA_ARGS__); _lockrel(_stdio_debug_lock)
#else
#define DEBUG_WARNING(...)
#endif
//fprintf(stderr, "%s:%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#ifdef _DEBUG_INFO
#define DEBUG_INFO(fmt, ...) while (!_locktry(_stdio_debug_lock)); fprintf(stdout, ANSI_COLOR_GREEN "%s:%d: " fmt ANSI_COLOR_RESET,__FILE__+strlen(__WORKSPACE__), __LINE__, __VA_ARGS__); _lockrel(_stdio_debug_lock)
#else
#define DEBUG_INFO(...)
#endif

#ifdef _DEBUG_ERROR
#define DEBUG_ERROR(fmt, ...) while (!_locktry(_stdio_debug_lock)); fprintf(stdout, ANSI_COLOR_RED "%s:%d: " fmt ANSI_COLOR_RESET,__FILE__+strlen(__WORKSPACE__), __LINE__, __VA_ARGS__); _lockrel(_stdio_debug_lock)
#else
#define DEBUG_ERROR(...)
#endif

#endif
