#ifndef TestProfilePage_H
#define TestProfilePage_H

#include "simpletools.h"
#include "RA8876.h"
#include "Style.h"
#include "Module.h"
#include "Images.h"
#include "Button.h"
#include "Keyboard.h"
#include "Selection.h"
#include "JSON.h"

typedef struct TestProfilePage_s
{
    bool complete;
    Display *display;
    Images *images;
    int mode;
    char *path;
    char *filename;
    MotionQuartet *quartet;
    MotionSet *set;
    MotionProfile *profile;
    TestProfile *test;
} TestProfilePage;

void test_profile_page_init(TestProfilePage *page, Display *display, Images *images);
void test_profile_page_destroy(TestProfilePage *page);
void test_profile_page_run(TestProfilePage *page);
#endif
