#ifndef EXPLORER_H
#define EXPLORER_H

#include "simpletools.h"
#include "RA8876.h"
#include "Style.h"
#include <dirent.h>
#include "Module.h"

#define EXPLORER_WIDTH 768
#define EXPLORER_HEIGHT 400

#define FILES_COLUMNS 6
#define FILE_ROWS_PER_PAGE 2
#define MAX_FILES (FILES_COLUMNS * FILE_ROWS_PER_PAGE)

typedef enum explorer_mode_e
{
    EXPLORER_MODE_FILE,
    EXPLORER_MODE_DIRECTORY
} ExplorerMode;

typedef struct explorer_s
{
    Display *display;
    ExplorerMode mode;
    char files[MAX_FILES][20];
    int fileCount;
    int x, y;
    bool complete;

    Module root;
    Module window;
    Module title;
    Module deleteButton;
    Module cancel;
    Module cancelText;
    Module dotdot;
    Module dotdotText;
    Module left;
    Module leftText;
    Module right;
    Module rightText;
    Module done;
    Module doneText;
    Module subroot;
    Module fileModule[MAX_FILES];
    Module fileText[MAX_FILES];
    char pathBuffer[100];
    int page;
    int selection;
} Explorer;

Explorer *explorer_create(Display *display, int x, int y, ExplorerMode mode, const char *path);
char *explorer_run(Explorer *explorer);
void explorer_destroy(Explorer *explorer);

#endif
