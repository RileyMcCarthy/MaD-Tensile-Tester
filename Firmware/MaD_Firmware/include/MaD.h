#ifndef MaD_H
#define MaD_H

#include "simpletools.h"
#include "Clock.h"
#include "IOBoard.h"
#include "MCP23017.h"
#include "DS3231.h"
#include "State.h"
#include "Style.h"
#include "Ra8876_Lite.h"
#include "i2cNavKey.h"
#include "ForceGauge.h"
#include "DYN4.h"
#include "NavigationPage.h"
#include "StatusPage.h"
#include "ManualPage.h"
#include "AutomaticPage.h"

#define GT9271_INT 7
#define CLK 28
#define DATA 29
#define RA8876_XNSCS 10
#define RA8876_XNRESET 9
#define BACKLIGHT 8

#define MAD_SD_DO 22
#define MAD_SD_CLK 23
#define MAD_CD_DI 24
#define MAD_CD_CS 25

class MAD
{
public:
    void begin();

private:
    Error startDisplay();

    //utility objects
    Ra8876_Lite display;
    Clock clock;
    MachineState machineState;
    DYN4 dyn4;
    ForceGauge forceGauge;
};
#endif