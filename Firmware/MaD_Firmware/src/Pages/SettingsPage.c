#include "SettingsPage.h"
#include <stdlib.h>

#define BUTTONCOUNT 23
#define BUTTON_NAVIGATION 0
#define BUTTON_NAME 1
#define BUTTON_NUMBER 2
#define BUTTON_CONFIGURATION_MOTORTYPE 3
#define BUTTON_CONFIGURATION_MAXMOTORTORQUE 4
#define BUTTON_CONFIGURATION_MAXMOTORRPM 5
#define BUTTON_CONFIGURATION_GEARDIAMETER 6
#define BUTTON_CONFIGURATION_GEARPITCH 7
#define BUTTON_CONFIGURATION_SYSTEMINTERTIA 8
#define BUTTON_CONFIGURATION_STATICTORQUE 9
#define BUTTON_CONFIGURATION_LOAD 10
#define BUTTON_CONFIGURATION_POSITIONENCODERTYPE 11
#define BUTTON_CONFIGURATION_POSITIONENCODERSCALEFACTOR 12
#define BUTTON_CONFIGURATION_FORCEGAUGE 13
#define BUTTON_CONFIGURATION_FORCEGAUGESCALEFACTOR 14
#define BUTTON_CONFIGURATION_FORCEGAUGENEUTRALOFFSET 15
#define BUTTON_PERFORMANCE_MINPOSITION 16
#define BUTTON_PERFORMANCE_MAXPOSITION 17
#define BUTTON_PERFORMANCE_MAXVELOCITY 18
#define BUTTON_PERFORMANCE_MAXACCELERATION 19
#define BUTTON_PERFORMANCE_MAXFORCETENSILE 20
#define BUTTON_PERFORMANCE_MAXFORCECOMPRESSION 21
#define BUTTON_PERFORMANCE_FORCEGAUGENEUTRALOFFSET 22

static bool isNumber(char *string)
{
    int i = 0;
    while (string[i] != '\0')
    {
        if (!isdigit(string[i]) && string[i] != '-')
        {
            return false;
        }
        i++;
    }
    return true;
}

static bool isFloat(char *string)
{
    int i = 0;
    while (string[i] != '\0')
    {
        if (!isdigit(string[i]) && string[i] != '.' && string[i] != '-')
        {
            return false;
        }
        i++;
    }
    return true;
}

static bool check_buttons(SettingsPage *page)
{
    button_update(page->display);

    if (button_check(page->display, page->buttons, BUTTONCOUNT) > 0)
    {
        for (int i = 0; i < BUTTONCOUNT; i++)
        {
            if (page->buttons[i].pressed)
            {
                switch (page->buttons[i].name)
                {
                case BUTTON_NAVIGATION:
                    page->complete = true;
                    break;
                case BUTTON_NAME:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *name = keyboard_get_input(keyboard, "Name: ");
                    if (name != NULL && strlen(name) > 0)
                    {
                        page->machineProfile->name = (char *)realloc(page->machineProfile->name, strlen(name) + 1);
                        strcpy(page->machineProfile->name, name);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_NUMBER:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *number = keyboard_get_input(keyboard, "Number: ");
                    if (number != NULL && isNumber(number))
                    {
                        page->machineProfile->number = atoi(number);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_CONFIGURATION_MOTORTYPE:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *type = keyboard_get_input(keyboard, "Motor Type: ");
                    if (type != NULL && strlen(type) > 0)
                    {
                        page->machineProfile->configuration->motorType = (char *)realloc(page->machineProfile->configuration->motorType, strlen(type) + 1);
                        strcpy(page->machineProfile->configuration->motorType, type);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_CONFIGURATION_MAXMOTORTORQUE:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *torque = keyboard_get_input(keyboard, "Max Torque: ");
                    if (torque != NULL && isFloat(torque))
                    {
                        page->machineProfile->configuration->maxMotorTorque = atof(torque);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_CONFIGURATION_MAXMOTORRPM:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *rpm = keyboard_get_input(keyboard, "Max RPM: ");
                    if (rpm != NULL && isFloat(rpm))
                    {
                        page->machineProfile->configuration->maxMotorRPM = atof(rpm);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_CONFIGURATION_GEARDIAMETER:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *diameter = keyboard_get_input(keyboard, "Gear Diameter: ");
                    if (diameter != NULL && isFloat(diameter))
                    {
                        page->machineProfile->configuration->gearDiameter = atof(diameter);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_CONFIGURATION_GEARPITCH:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *pitch = keyboard_get_input(keyboard, "Gear Pitch: ");
                    if (pitch != NULL && isFloat(pitch))
                    {
                        page->machineProfile->configuration->gearPitch = atof(pitch);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_CONFIGURATION_SYSTEMINTERTIA:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *inertia = keyboard_get_input(keyboard, "Inertia: ");
                    if (inertia != NULL && isFloat(inertia))
                    {
                        page->machineProfile->configuration->systemIntertia = atof(inertia);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_CONFIGURATION_STATICTORQUE:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *torque = keyboard_get_input(keyboard, "Static Torque: ");
                    if (torque != NULL && isFloat(torque))
                    {
                        page->machineProfile->configuration->staticTorque = atof(torque);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_CONFIGURATION_LOAD:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *load = keyboard_get_input(keyboard, "Load: ");
                    if (load != NULL && isFloat(load))
                    {
                        page->machineProfile->configuration->load = atof(load);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_CONFIGURATION_POSITIONENCODERTYPE:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *encoder = keyboard_get_input(keyboard, "Encoder Type: ");
                    if (encoder != NULL && strlen(encoder) > 0)
                    {
                        page->machineProfile->configuration->positionEncoderType = (char *)realloc(page->machineProfile->configuration->positionEncoderType, strlen(encoder) + 1);
                        strcpy(page->machineProfile->configuration->positionEncoderType, encoder);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_CONFIGURATION_POSITIONENCODERSCALEFACTOR:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *scale = keyboard_get_input(keyboard, "Encoder Slope: ");
                    if (scale != NULL && isFloat(scale))
                    {
                        page->machineProfile->configuration->positionEncoderStepsPerRev = atof(scale);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_CONFIGURATION_FORCEGAUGE:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *forcegauge = keyboard_get_input(keyboard, "Force Gauge: ");
                    if (forcegauge != NULL && strlen(forcegauge) > 0)
                    {
                        page->machineProfile->configuration->forceGauge = (char *)realloc(page->machineProfile->configuration->forceGauge, strlen(forcegauge) + 1);
                        strcpy(page->machineProfile->configuration->forceGauge, forcegauge);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_CONFIGURATION_FORCEGAUGESCALEFACTOR:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *scale = keyboard_get_input(keyboard, "Scale: ");
                    if (scale != NULL && isFloat(scale))
                    {
                        page->machineProfile->configuration->forceGaugeScaleFactor = atof(scale);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_CONFIGURATION_FORCEGAUGENEUTRALOFFSET:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *zero = keyboard_get_input(keyboard, "Force Zero: ");
                    if (zero != NULL && isNumber(zero))
                    {
                        page->machineProfile->configuration->forceGaugeZeroFactor = atoi(zero);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_PERFORMANCE_MINPOSITION:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *minpos = keyboard_get_input(keyboard, "Minimum Position: ");
                    if (minpos != NULL && isFloat(minpos))
                    {
                        page->machineProfile->performance->minPosition = atof(minpos);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_PERFORMANCE_MAXPOSITION:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *maxpos = keyboard_get_input(keyboard, "Maximum Position: ");
                    if (maxpos != NULL && isFloat(maxpos))
                    {
                        page->machineProfile->performance->maxPosition = atof(maxpos);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_PERFORMANCE_MAXVELOCITY:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *maxvel = keyboard_get_input(keyboard, "Maximum Velocity: ");
                    if (maxvel != NULL && isFloat(maxvel))
                    {
                        page->machineProfile->performance->maxVelocity = atof(maxvel);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_PERFORMANCE_MAXACCELERATION:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *maxaccel = keyboard_get_input(keyboard, "Maximum Acceleration: ");
                    if (maxaccel != NULL && isFloat(maxaccel))
                    {
                        page->machineProfile->performance->maxAcceleration = atof(maxaccel);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_PERFORMANCE_MAXFORCETENSILE:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *maxtensile = keyboard_get_input(keyboard, "Maximum Tensile: ");
                    if (maxtensile != NULL && isFloat(maxtensile))
                    {
                        page->machineProfile->performance->maxForceTensile = atof(maxtensile);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_PERFORMANCE_MAXFORCECOMPRESSION:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *maxcompression = keyboard_get_input(keyboard, "Maximum Compression: ");
                    if (maxcompression != NULL && isFloat(maxcompression))
                    {
                        page->machineProfile->performance->maxForceCompression = atof(maxcompression);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                case BUTTON_PERFORMANCE_FORCEGAUGENEUTRALOFFSET:
                {
                    Keyboard *keyboard = keyboard_create(page->display, page->images);
                    char *neutral = keyboard_get_input(keyboard, "Force Offset: ");
                    if (neutral != NULL && isFloat(neutral))
                    {
                        page->machineProfile->performance->forceGaugeNeutralOffset = atof(neutral);
                    }
                    keyboard_destroy(keyboard);
                    break;
                }
                }
                return true;
            }
        }
        return false;
    }
}

SettingsPage *settings_page_create(Display *display, MachineProfile *machineProfile, Images *images)
{
    SettingsPage *page = (SettingsPage *)malloc(sizeof(SettingsPage));
    page->display = display;
    page->images = images;
    page->machineProfile = machineProfile;
    page->complete = false;
    page->buttons = (Button *)malloc(sizeof(Button) * BUTTONCOUNT);
    return page;
}

void settings_page_destroy(SettingsPage *page)
{
    free(page->buttons);
    free(page);
}

bool settings_page_run(SettingsPage *page)
{
    page->complete = false;
    printf("Settings page running\n");
    display_draw_square_fill(page->display, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BACKCOLOR);
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display_set_text_parameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);

    int machineProfileX0 = 20;
    int machineProfileX1 = SCREEN_WIDTH * 2 / 3 - 10;
    int machineProfileY0 = 20;
    int machineProfileY1 = SCREEN_HEIGHT - 20;
    int machineProfileWidth = machineProfileX1 - machineProfileX0;

    /*Section windows*/
    // Machine State window
    display_draw_circle_square_fill(page->display, machineProfileX0, machineProfileY0, machineProfileX1, machineProfileY1, 20, 20, MAINCOLOR);

    page->buttons[0].name = BUTTON_NAVIGATION;
    page->buttons[0].xmin = SCREEN_WIDTH - 100;
    page->buttons[0].xmax = page->buttons[0].xmin + 100;
    page->buttons[0].ymin = 0;
    page->buttons[0].ymax = page->buttons[0].ymin + 100;
    page->buttons[0].pressed = false;
    page->buttons[0].lastPress = 0;

    Image *navigationImg = page->images->navigationImage;
    display_bte_memory_copy_image(page->display, navigationImg, SCREEN_WIDTH - navigationImg->width - 5, 5);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);

    /*Machine profile header*/
    char buf[100];
    strcpy(buf, "Machine Profile");
    int machineProfileStartX = machineProfileX0 + machineProfileWidth / 2 - strlen(buf) * 8;
    int machineProfileStartY = 40;
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    display_draw_string(page->display, machineProfileX0 + machineProfileWidth / 2 - strlen(buf) * 8, machineProfileY0 + 20, buf);
    display_draw_line(page->display, machineProfileStartX, machineProfileStartY + 30, machineProfileStartX + strlen(buf) * 16, machineProfileStartY + 30, MAINTEXTCOLOR);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);

    /*Machine state fields*/
    // Name
    sprintf(buf, "%s: %s", "Name", page->machineProfile->name);
    int nameStartX = machineProfileX0 + machineProfileWidth / 2 - strlen(buf) * 6;
    int nameStartY = machineProfileStartY + 40;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    display_draw_string(page->display, nameStartX, nameStartY, buf);
    page->buttons[1].name = BUTTON_NAME;
    page->buttons[1].xmin = nameStartX;
    page->buttons[1].xmax = page->buttons[1].xmin + strlen(buf) * 12;
    page->buttons[1].ymin = nameStartY;
    page->buttons[1].ymax = page->buttons[1].ymin + 30;
    page->buttons[1].pressed = false;
    page->buttons[1].lastPress = 0;

    // Number
    sprintf(buf, "%s: %d", "Number", page->machineProfile->number);
    int numberStartX = machineProfileX0 + machineProfileWidth / 2 - strlen(buf) * 6;
    int numberStartY = nameStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    display_draw_string(page->display, numberStartX, numberStartY, buf);
    page->buttons[2].name = BUTTON_NUMBER;
    page->buttons[2].xmin = numberStartX;
    page->buttons[2].xmax = page->buttons[2].xmin + strlen(buf) * 12;
    page->buttons[2].ymin = numberStartY;
    page->buttons[2].ymax = page->buttons[2].ymin + 30;
    page->buttons[2].pressed = false;
    page->buttons[2].lastPress = 0;

    /*Machine configuration header*/
    int configurationStartY = numberStartY + 30;
    int configurationStartX = machineProfileX0 + 20;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    strcpy(buf, "Configuration");
    display_draw_string(page->display, configurationStartX, configurationStartY, buf);
    display_draw_line(page->display, configurationStartX, configurationStartY + 22, configurationStartX + strlen(buf) * 12, configurationStartY + 22, MAINTEXTCOLOR);

    /*Machine configuration fields*/
    // Motor Type
    int motorTypeStartX = configurationStartX;
    int motorTypeStartY = configurationStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    sprintf(buf, "%s: %s", "Motor", page->machineProfile->configuration->motorType);
    display_draw_string(page->display, motorTypeStartX, motorTypeStartY, buf);

    page->buttons[3].name = BUTTON_CONFIGURATION_MOTORTYPE;
    page->buttons[3].xmin = motorTypeStartX;
    page->buttons[3].xmax = page->buttons[3].xmin + strlen(buf) * 12;
    page->buttons[3].ymin = motorTypeStartY;
    page->buttons[3].ymax = page->buttons[3].ymin + 30;
    page->buttons[3].pressed = false;
    page->buttons[3].lastPress = 0;

    // max motor torque
    int maxMotorTorqueStartX = motorTypeStartX;
    int maxMotorTorqueStartY = motorTypeStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    sprintf(buf, "%s: %0.3f", "Max Torque", page->machineProfile->configuration->maxMotorTorque);
    display_draw_string(page->display, maxMotorTorqueStartX, maxMotorTorqueStartY, buf);
    page->buttons[4].name = BUTTON_CONFIGURATION_MAXMOTORTORQUE;
    page->buttons[4].xmin = maxMotorTorqueStartX;
    page->buttons[4].xmax = page->buttons[4].xmin + strlen(buf) * 12;
    page->buttons[4].ymin = maxMotorTorqueStartY;
    page->buttons[4].ymax = page->buttons[4].ymin + 30;
    page->buttons[4].pressed = false;
    page->buttons[4].lastPress = 0;
    // maxMotorRPM
    int maxMotorRPMStartX = maxMotorTorqueStartX;
    int maxMotorRPMStartY = maxMotorTorqueStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    sprintf(buf, "%s: %0.3f", "Max RPM", page->machineProfile->configuration->maxMotorRPM);
    display_draw_string(page->display, maxMotorRPMStartX, maxMotorRPMStartY, buf);
    page->buttons[5].name = BUTTON_CONFIGURATION_MAXMOTORRPM;
    page->buttons[5].xmin = maxMotorRPMStartX;
    page->buttons[5].xmax = page->buttons[5].xmin + strlen(buf) * 12;
    page->buttons[5].ymin = maxMotorRPMStartY;
    page->buttons[5].ymax = page->buttons[5].ymin + 30;
    page->buttons[5].pressed = false;
    page->buttons[5].lastPress = 0;
    // gearDiameter
    int gearDiameterStartX = maxMotorRPMStartX;
    int gearDiameterStartY = maxMotorRPMStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    sprintf(buf, "%s: %0.3f", "Gear Diameter", page->machineProfile->configuration->gearDiameter);
    display_draw_string(page->display, gearDiameterStartX, gearDiameterStartY, buf);
    page->buttons[6].name = BUTTON_CONFIGURATION_GEARDIAMETER;
    page->buttons[6].xmin = gearDiameterStartX;
    page->buttons[6].xmax = page->buttons[6].xmin + strlen(buf) * 12;
    page->buttons[6].ymin = gearDiameterStartY;
    page->buttons[6].ymax = page->buttons[6].ymin + 30;
    page->buttons[6].pressed = false;
    page->buttons[6].lastPress = 0;
    // systemIntertia
    int systemIntertiaStartX = gearDiameterStartX;
    int systemIntertiaStartY = gearDiameterStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    sprintf(buf, "%s: %0.3f", "Intertia", page->machineProfile->configuration->systemIntertia);
    display_draw_string(page->display, systemIntertiaStartX, systemIntertiaStartY, buf);
    page->buttons[7].name = BUTTON_CONFIGURATION_SYSTEMINTERTIA;
    page->buttons[7].xmin = systemIntertiaStartX;
    page->buttons[7].xmax = page->buttons[7].xmin + strlen(buf) * 12;
    page->buttons[7].ymin = systemIntertiaStartY;
    page->buttons[7].ymax = page->buttons[7].ymin + 30;
    page->buttons[7].pressed = false;
    page->buttons[7].lastPress = 0;
    // staticTorque
    int staticTorqueStartX = systemIntertiaStartX;
    int staticTorqueStartY = systemIntertiaStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    sprintf(buf, "%s: %0.3f", "Static Torque", page->machineProfile->configuration->staticTorque);
    display_draw_string(page->display, staticTorqueStartX, staticTorqueStartY, buf);
    page->buttons[8].name = BUTTON_CONFIGURATION_STATICTORQUE;
    page->buttons[8].xmin = staticTorqueStartX;
    page->buttons[8].xmax = page->buttons[8].xmin + strlen(buf) * 12;
    page->buttons[8].ymin = staticTorqueStartY;
    page->buttons[8].ymax = page->buttons[8].ymin + 30;
    page->buttons[8].pressed = false;
    page->buttons[8].lastPress = 0;
    // load
    int loadStartX = staticTorqueStartX;
    int loadStartY = staticTorqueStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    sprintf(buf, "%s: %0.3f", "Load", page->machineProfile->configuration->load);
    display_draw_string(page->display, loadStartX, loadStartY, buf);
    page->buttons[9].name = BUTTON_CONFIGURATION_LOAD;
    page->buttons[9].xmin = loadStartX;
    page->buttons[9].xmax = page->buttons[9].xmin + strlen(buf) * 12;
    page->buttons[9].ymin = loadStartY;
    page->buttons[9].ymax = page->buttons[9].ymin + 30;
    page->buttons[9].pressed = false;
    page->buttons[9].lastPress = 0;
    // positionEncoderType
    int positionEncoderTypeStartX = loadStartX;
    int positionEncoderTypeStartY = loadStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    sprintf(buf, "%s: %s", "Encoder", page->machineProfile->configuration->positionEncoderType);
    display_draw_string(page->display, positionEncoderTypeStartX, positionEncoderTypeStartY, buf);
    page->buttons[10].name = BUTTON_CONFIGURATION_POSITIONENCODERTYPE;
    page->buttons[10].xmin = positionEncoderTypeStartX;
    page->buttons[10].xmax = page->buttons[10].xmin + strlen(buf) * 12;
    page->buttons[10].ymin = positionEncoderTypeStartY;
    page->buttons[10].ymax = page->buttons[10].ymin + 30;
    page->buttons[10].pressed = false;
    page->buttons[10].lastPress = 0;
    // positionEncoderScaleFactor
    int positionEncoderScaleFactorStartX = positionEncoderTypeStartX;
    int positionEncoderScaleFactorStartY = positionEncoderTypeStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    sprintf(buf, "%s: %0.3f", "Encoder Slope", page->machineProfile->configuration->positionEncoderStepsPerRev);
    display_draw_string(page->display, positionEncoderScaleFactorStartX, positionEncoderScaleFactorStartY, buf);
    page->buttons[11].name = BUTTON_CONFIGURATION_POSITIONENCODERSCALEFACTOR;
    page->buttons[11].xmin = positionEncoderScaleFactorStartX;
    page->buttons[11].xmax = page->buttons[11].xmin + strlen(buf) * 12;
    page->buttons[11].ymin = positionEncoderScaleFactorStartY;
    page->buttons[11].ymax = page->buttons[11].ymin + 30;
    page->buttons[11].pressed = false;
    page->buttons[11].lastPress = 0;
    // forceGauge
    int forceGaugeStartX = positionEncoderScaleFactorStartX;
    int forceGaugeStartY = positionEncoderScaleFactorStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    sprintf(buf, "%s: %s", "Force Gauge", page->machineProfile->configuration->forceGauge);
    display_draw_string(page->display, forceGaugeStartX, forceGaugeStartY, buf);
    page->buttons[12].name = BUTTON_CONFIGURATION_FORCEGAUGE;
    page->buttons[12].xmin = forceGaugeStartX;
    page->buttons[12].xmax = page->buttons[12].xmin + strlen(buf) * 12;
    page->buttons[12].ymin = forceGaugeStartY;
    page->buttons[12].ymax = page->buttons[12].ymin + 30;
    page->buttons[12].pressed = false;
    page->buttons[12].lastPress = 0;
    // forceGaugeScaleFactor
    int forceGaugeScaleFactorStartX = forceGaugeStartX;
    int forceGaugeScaleFactorStartY = forceGaugeStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    sprintf(buf, "%s: %0.3f", "Force Slope", page->machineProfile->configuration->forceGaugeScaleFactor);
    display_draw_string(page->display, forceGaugeScaleFactorStartX, forceGaugeScaleFactorStartY, buf);
    page->buttons[13].name = BUTTON_CONFIGURATION_FORCEGAUGESCALEFACTOR;
    page->buttons[13].xmin = forceGaugeScaleFactorStartX;
    page->buttons[13].xmax = page->buttons[13].xmin + strlen(buf) * 12;
    page->buttons[13].ymin = forceGaugeScaleFactorStartY;
    page->buttons[13].ymax = page->buttons[13].ymin + 30;
    page->buttons[13].pressed = false;
    page->buttons[13].lastPress = 0;
    // forceGaugeZeroFactor
    int forceGaugeZeroFactorStartX = forceGaugeScaleFactorStartX;
    int forceGaugeZeroFactorStartY = forceGaugeScaleFactorStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    sprintf(buf, "%s: %d", "Force Zero", page->machineProfile->configuration->forceGaugeZeroFactor);
    display_draw_string(page->display, forceGaugeZeroFactorStartX, forceGaugeZeroFactorStartY, buf);
    page->buttons[14].name = BUTTON_CONFIGURATION_FORCEGAUGENEUTRALOFFSET;
    page->buttons[14].xmin = forceGaugeZeroFactorStartX;
    page->buttons[14].xmax = page->buttons[14].xmin + strlen(buf) * 12;
    page->buttons[14].ymin = forceGaugeZeroFactorStartY;
    page->buttons[14].ymax = page->buttons[14].ymin + 30;
    page->buttons[14].pressed = false;
    page->buttons[14].lastPress = 0;
    // Draw division line
    display_draw_line(page->display, machineProfileX0 + machineProfileWidth / 2, configurationStartY, machineProfileX0 + machineProfileWidth / 2, machineProfileY1 - 20, COLOR65K_BLACK);

    /*Machine performance header*/
    int performanceStartY = numberStartY + 30;
    int performanceStartX = machineProfileX0 + machineProfileWidth / 2 + 20;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    strcpy(buf, "Performance");
    display_draw_string(page->display, performanceStartX, performanceStartY, buf);
    display_draw_line(page->display, performanceStartX, performanceStartY + 22, performanceStartX + strlen(buf) * 12, performanceStartY + 22, MAINTEXTCOLOR);

    /*Machine performance fields*/
    //  minPosition
    int minPositionStartX = performanceStartX;
    int minPositionStartY = performanceStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    sprintf(buf, "%s: %0.3f", "Min Position", page->machineProfile->performance->minPosition);
    display_draw_string(page->display, minPositionStartX, minPositionStartY, buf);
    page->buttons[15].name = BUTTON_PERFORMANCE_MINPOSITION;
    page->buttons[15].xmin = minPositionStartX;
    page->buttons[15].xmax = page->buttons[15].xmin + strlen(buf) * 12;
    page->buttons[15].ymin = minPositionStartY;
    page->buttons[15].ymax = page->buttons[15].ymin + 30;
    page->buttons[15].pressed = false;
    page->buttons[15].lastPress = 0;
    //  maxPosition
    int maxPositionStartX = minPositionStartX;
    int maxPositionStartY = minPositionStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    sprintf(buf, "%s: %0.3f", "Max Position", page->machineProfile->performance->maxPosition);
    display_draw_string(page->display, maxPositionStartX, maxPositionStartY, buf);
    page->buttons[16].name = BUTTON_PERFORMANCE_MAXPOSITION;
    page->buttons[16].xmin = maxPositionStartX;
    page->buttons[16].xmax = page->buttons[16].xmin + strlen(buf) * 12;
    page->buttons[16].ymin = maxPositionStartY;
    page->buttons[16].ymax = page->buttons[16].ymin + 30;
    page->buttons[16].pressed = false;
    page->buttons[16].lastPress = 0;
    //  maxVelocity
    int maxVelocityStartX = maxPositionStartX;
    int maxVelocityStartY = maxPositionStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    sprintf(buf, "%s: %0.3f", "Max Velocity", page->machineProfile->performance->maxVelocity);
    display_draw_string(page->display, maxVelocityStartX, maxVelocityStartY, buf);
    page->buttons[17].name = BUTTON_PERFORMANCE_MAXVELOCITY;
    page->buttons[17].xmin = maxVelocityStartX;
    page->buttons[17].xmax = page->buttons[17].xmin + strlen(buf) * 12;
    page->buttons[17].ymin = maxVelocityStartY;
    page->buttons[17].ymax = page->buttons[17].ymin + 30;
    page->buttons[17].pressed = false;
    page->buttons[17].lastPress = 0;
    //  maxAcceleration
    int maxAccelerationStartX = maxVelocityStartX;
    int maxAccelerationStartY = maxVelocityStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    sprintf(buf, "%s: %0.3f", "Max Acceleration", page->machineProfile->performance->maxAcceleration);
    display_draw_string(page->display, maxAccelerationStartX, maxAccelerationStartY, buf);
    page->buttons[18].name = BUTTON_PERFORMANCE_MAXACCELERATION;
    page->buttons[18].xmin = maxAccelerationStartX;
    page->buttons[18].xmax = page->buttons[18].xmin + strlen(buf) * 12;
    page->buttons[18].ymin = maxAccelerationStartY;
    page->buttons[18].ymax = page->buttons[18].ymin + 30;
    page->buttons[18].pressed = false;
    page->buttons[18].lastPress = 0;
    //  maxForceTensile
    int maxForceTensileStartX = maxAccelerationStartX;
    int maxForceTensileStartY = maxAccelerationStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    sprintf(buf, "%s: %0.3f", "Max Tensile", page->machineProfile->performance->maxForceTensile);
    display_draw_string(page->display, maxForceTensileStartX, maxForceTensileStartY, buf);
    page->buttons[19].name = BUTTON_PERFORMANCE_MAXFORCETENSILE;
    page->buttons[19].xmin = maxForceTensileStartX;
    page->buttons[19].xmax = page->buttons[19].xmin + strlen(buf) * 12;
    page->buttons[19].ymin = maxForceTensileStartY;
    page->buttons[19].ymax = page->buttons[19].ymin + 30;
    page->buttons[19].pressed = false;
    page->buttons[19].lastPress = 0;
    // maxForceCompression
    int maxForceCompressionStartX = maxForceTensileStartX;
    int maxForceCompressionStartY = maxForceTensileStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    sprintf(buf, "%s: %0.3f", "Max Compression", page->machineProfile->performance->maxForceCompression);
    display_draw_string(page->display, maxForceCompressionStartX, maxForceCompressionStartY, buf);
    page->buttons[20].name = BUTTON_PERFORMANCE_MAXFORCECOMPRESSION;
    page->buttons[20].xmin = maxForceCompressionStartX;
    page->buttons[20].xmax = page->buttons[20].xmin + strlen(buf) * 12;
    page->buttons[20].ymin = maxForceCompressionStartY;
    page->buttons[20].ymax = page->buttons[20].ymin + 30;
    page->buttons[20].pressed = false;
    page->buttons[20].lastPress = 0;
    // forceGaugeNeutralOffset
    int forceGaugeNeutralOffsetStartX = maxForceCompressionStartX;
    int forceGaugeNeutralOffsetStartY = maxForceCompressionStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    sprintf(buf, "%s: %0.3f", "Force Neutral", page->machineProfile->performance->forceGaugeNeutralOffset);
    display_draw_string(page->display, forceGaugeNeutralOffsetStartX, forceGaugeNeutralOffsetStartY, buf);
    page->buttons[21].name = BUTTON_PERFORMANCE_FORCEGAUGENEUTRALOFFSET;
    page->buttons[21].xmin = forceGaugeNeutralOffsetStartX;
    page->buttons[21].xmax = page->buttons[21].xmin + strlen(buf) * 12;
    page->buttons[21].ymin = forceGaugeNeutralOffsetStartY;
    page->buttons[21].ymax = page->buttons[21].ymin + 30;
    page->buttons[21].pressed = false;
    page->buttons[21].lastPress = 0;

    button_check(page->display, page->buttons, BUTTONCOUNT); // Clear button presses
    while (!page->complete)
    {
        if (check_buttons(page) && !page->complete)
        {
            printf("reloading page\n");
            return true; // reload page
        }
    }
    printf("exiting page\n");
    return false; // exit page
}
