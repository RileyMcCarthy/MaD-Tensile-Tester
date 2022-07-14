#ifndef TestProfilePage_H
#define TestProfilePage_H

#include "simpletools.h"
#include "RA8876.h"
#include "Style.h"
#include "Module.h"
#include "Images.h"
#include "Keyboard.h"
#include "Selection.h"
#include "JSON.h"
#include "Graph.h"
#include "motionPlanning.h"

typedef struct TestProfilePage_s
{
    bool complete;
    Display *display;
    Images *images;
    int mode;
    char path[100];
    char filename[20];
    MotionQuartet quartet;
    MotionSet set;
    MotionProfile profile;
    TestProfile test;

    Module root;
    Module background;
    Module editWindow;
    Module openButton;
    Module openText;
    Module newButton;
    Module newText;
    Module saveButton;
    Module saveText;
    Module navigationButton;
    Module editWindowTitle;
    Module graphPositionContainer;
    ModuleGraph graphPosition;
    Module subroot;

    Module profileParameters[20];

    RunMotionProfile runProfile;

    char editWindowTitleBuffer[30];
    char profileParametersBuffer[20][30];
    FunctionInfo info;
    Selection selection;
} TestProfilePage;

void test_profile_page_init(TestProfilePage *page, Display *display, Images *images);
void test_profile_page_destroy(TestProfilePage *page);
void test_profile_page_run(TestProfilePage *page);
#endif
