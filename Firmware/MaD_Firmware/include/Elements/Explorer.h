#ifndef EXPLORER_H
#define EXPLORER_H

#include "simpletools.h"
#include "RA8876.h"
#include "Style.h"
#include <dirent.h>
#include "Module.h"
#include "Button.h"

#define EXPLORER_WIDTH 768
#define EXPLORER_HEIGHT 400

typedef enum explorer_mode_e
{
    EXPLORER_MODE_FILE,
    EXPLORER_MODE_DIRECTORY
} ExplorerMode;

typedef struct explorer_s
{
    char *path;
    Display *display;
    Button *buttons;
    ExplorerMode mode;
    char **files;
    int fileCount;
    int x, y;
    bool complete;
} Explorer;

Explorer *explorer_create(Display *display, int x, int y, ExplorerMode mode, const char *path);
char *explorer_run(Explorer *explorer);
void explorer_destroy(Explorer *explorer);

#endif