// status page
#include "StatusPage.h"
#include "Images.h"
#include "propeller2.h"

#define BUTTONCOUNT 2
#define BUTTON_MACHINE 0
#define BUTTON_NAVIGATION 1

static bool complete = false;

static char machineStateWindowTitleBuffer[] = "Machine State";
static char chargePumpTextBuffer[] = "Charge Pump";
static char switchedPowerTextBuffer[] = "Switched Power";
static char estSwitchTextBuffer[] = "ESD Switch";
static char esdUpperTextBuffer[] = "ESD Upper";
static char estLowerTextBuffer[] = "ESD Lower";
static char servoReadyTextBuffer[] = "Servo Ready";
static char forceGaugeComTextBuffer[] = "Force Comm";
static char servoComTextBuffer[] = "Servo Comm";
static char machineInfoWindowTitleBuffer[] = "Machine Info";
static char forceTextBuffer[] = "Force (N)";
static char positionTextBuffer[] = "Position (mm)";

static void button_navigation(int id, void *arg)
{
    StatusPage *page = (StatusPage *)arg;
    complete = true;
}

void status_page_init(StatusPage *page, Display *display, MachineState *machineState, MachineProfile *machineProfile, MonitorData *data, Images *images)
{
    page->display = display;
    page->stateMachine = machineState;
    page->machineProfile = machineProfile;
    page->data = data;
    page->images = images;

    int padding = 8;
    // Create Background
    Module *root = &(page->root);
    module_init(root, NULL);

    Module *background = &(page->background);
    module_init(background, root);
    module_set_rectangle_circle(background, SCREEN_WIDTH, SCREEN_HEIGHT);
    module_set_position(background, 0, 0);
    module_set_padding(background, 10, 10);
    module_set_color(background, BACKCOLOR, BACKCOLOR);
    module_animate_draw(background, module_animation_switch_page_up);

    // Create navigation button
    Module *navigationButton = &(page->navigationButton);
    module_init(navigationButton, background);
    module_set_image(navigationButton, &(page->images->navigationImage));
    module_align_inner_top(navigationButton);
    module_align_inner_right(navigationButton);
    module_touch_callback(navigationButton, button_navigation, 0);

    // Create machine state window
    Module *machineStateWindow = &(page->machineStateWindow);
    module_init(machineStateWindow, background);
    module_set_padding(machineStateWindow, padding, padding);
    module_add_border(machineStateWindow, MAINTEXTCOLOR, 1);
    module_set_margin(machineStateWindow, 10, 10);
    module_set_rectangle_circle(machineStateWindow, 0, 0);
    module_fit_space_even(machineStateWindow, 3);
    module_fit_height(machineStateWindow);
    module_set_padding(machineStateWindow, padding, padding);
    module_set_color(machineStateWindow, MAINCOLOR, BACKCOLOR);
    module_align_inner_left(machineStateWindow);
    module_align_inner_top(machineStateWindow);

    Module *machineStateImages = &(page->machineStateImages);
    module_init(machineStateImages, machineStateWindow);
    module_set_padding(machineStateImages, 0, 0);
    module_align_inner_left(machineStateImages);
    module_align_inner_top(machineStateImages);
    module_fit_width(machineStateImages);
    module_fit_height(machineStateImages);

    // Create machine state window title
    Module *machineStateWindowTitle = &(page->machineStateWindowTitle);
    module_init(machineStateWindowTitle, machineStateWindow);
    module_set_text(machineStateWindowTitle, machineStateWindowTitleBuffer);
    module_set_margin(machineStateWindowTitle, 8, 8);
    module_text_font(machineStateWindowTitle, RA8876_CHAR_HEIGHT_32);
    module_text_fit(machineStateWindowTitle);
    module_set_color(machineStateWindowTitle, MAINTEXTCOLOR, MAINCOLOR);
    module_align_inner_top(machineStateWindowTitle);
    module_align_center(machineStateWindowTitle);
    module_text_underline(machineStateWindowTitle);

    // Charge Pump
    Module *chargePumpText = &(page->chargePumpText);
    module_init(chargePumpText, machineStateWindow);
    module_set_margin(chargePumpText, 8, 8);
    module_set_text(chargePumpText, chargePumpTextBuffer);
    module_text_font(chargePumpText, RA8876_CHAR_HEIGHT_24);
    module_set_color(chargePumpText, MAINTEXTCOLOR, MAINCOLOR);
    module_align_below(chargePumpText, machineStateWindowTitle);
    module_align_inner_left(chargePumpText);

    Module *chargePumpImage = &(page->chargePumpImage);
    module_init(chargePumpImage, machineStateImages);
    module_set_image(chargePumpImage, &(page->images->failImage));
    module_align_below(chargePumpImage, machineStateWindowTitle);
    module_align_inner_right(chargePumpImage);

    // Switched Power
    Module *switchedPowerText = &(page->switchedPowerText);
    module_init(switchedPowerText, machineStateWindow);
    module_copy(switchedPowerText, chargePumpText);
    module_set_text(switchedPowerText, switchedPowerTextBuffer);
    module_text_font(switchedPowerText, RA8876_CHAR_HEIGHT_24);
    module_align_below(switchedPowerText, chargePumpText);

    Module *switchedPowerImage = &(page->switchedPowerImage);
    module_init(switchedPowerImage, machineStateImages);
    module_set_image(switchedPowerImage, &(page->images->failImage));
    module_align_below(switchedPowerImage, chargePumpText);
    module_align_inner_right(switchedPowerImage);

    // ESD Switch
    Module *estSwitchText = &(page->estSwitchText);
    module_init(estSwitchText, machineStateWindow);
    module_copy(estSwitchText, chargePumpText);
    module_set_text(estSwitchText, estSwitchTextBuffer);
    module_text_font(estSwitchText, RA8876_CHAR_HEIGHT_24);
    module_align_below(estSwitchText, switchedPowerText);

    Module *estSwitchImage = &(page->estSwitchImage);
    module_init(estSwitchImage, machineStateImages);
    module_set_image(estSwitchImage, &(page->images->failImage));
    module_align_below(estSwitchImage, switchedPowerText);
    module_align_inner_right(estSwitchImage);

    // ESD Upper
    Module *esdUpperText = &(page->esdUpperText);
    module_init(esdUpperText, machineStateWindow);
    module_copy(esdUpperText, chargePumpText);
    module_set_text(esdUpperText, esdUpperTextBuffer);
    module_text_font(esdUpperText, RA8876_CHAR_HEIGHT_24);
    module_align_below(esdUpperText, estSwitchText);

    Module *esdUpperImage = &(page->esdUpperImage);
    module_init(esdUpperImage, machineStateImages);
    module_set_image(esdUpperImage, &(page->images->failImage));
    module_align_below(esdUpperImage, estSwitchText);
    module_align_inner_right(esdUpperImage);

    // ESD Lower
    Module *estLowerText = &(page->estLowerText);
    module_init(estLowerText, machineStateWindow);
    module_copy(estLowerText, chargePumpText);
    module_set_text(estLowerText, estLowerTextBuffer);
    module_text_font(estLowerText, RA8876_CHAR_HEIGHT_24);
    module_align_below(estLowerText, esdUpperText);

    Module *estLowerImage = &(page->estLowerImage);
    module_init(estLowerImage, machineStateImages);
    module_set_image(estLowerImage, &(page->images->failImage));
    module_align_below(estLowerImage, esdUpperText);
    module_align_inner_right(estLowerImage);

    // Servo Ready
    Module *servoReadyText = &(page->servoReadyText);
    module_init(servoReadyText, machineStateWindow);
    module_copy(servoReadyText, chargePumpText);
    module_set_text(servoReadyText, servoReadyTextBuffer);
    module_text_font(servoReadyText, RA8876_CHAR_HEIGHT_24);
    module_align_below(servoReadyText, estLowerText);

    Module *servoReadyImage = &(page->servoReadyImage);
    module_init(servoReadyImage, machineStateImages);
    module_set_image(servoReadyImage, &(page->images->failImage));
    module_align_below(servoReadyImage, estLowerText);
    module_align_inner_right(servoReadyImage);

    // Force Gauge Comm
    Module *forceGaugeComText = &(page->forceGaugeComText);
    module_init(forceGaugeComText, machineStateWindow);
    module_copy(forceGaugeComText, chargePumpText);
    module_set_text(forceGaugeComText, forceGaugeComTextBuffer);
    module_text_font(forceGaugeComText, RA8876_CHAR_HEIGHT_24);
    module_align_below(forceGaugeComText, servoReadyText);

    Module *forceGaugeComImage = &(page->forceGaugeComImage);
    module_init(forceGaugeComImage, machineStateImages);
    module_set_image(forceGaugeComImage, &(page->images->failImage));
    module_align_below(forceGaugeComImage, servoReadyText);
    module_align_inner_right(forceGaugeComImage);

    // Servo Comm
    Module *servoComText = &(page->servoComText);
    module_init(servoComText, machineStateWindow);
    module_copy(servoComText, chargePumpText);
    module_set_text(servoComText, servoComTextBuffer);
    module_text_font(servoComText, RA8876_CHAR_HEIGHT_24);
    module_align_below(servoComText, forceGaugeComText);

    Module *servoComImage = &(page->servoComImage);
    module_init(servoComImage, machineStateImages);
    module_set_image(servoComImage, &(page->images->failImage));
    module_align_below(servoComImage, forceGaugeComText);
    module_align_inner_right(servoComImage);

    // Create machine info window
    Module *machineInfoWindow = &(page->machineInfoWindow);
    module_init(machineInfoWindow, background);
    module_set_padding(machineInfoWindow, padding, padding);
    module_add_border(machineInfoWindow, MAINTEXTCOLOR, 1);
    module_set_margin(machineInfoWindow, 10, 10);
    module_set_rectangle_circle(machineInfoWindow, 0, 0);
    module_fit_space_even(machineInfoWindow, 3);
    module_set_padding(machineInfoWindow, padding, padding);
    module_fit_height(machineInfoWindow);
    module_set_color(machineInfoWindow, MAINCOLOR, BACKCOLOR);
    module_align_right(machineInfoWindow, machineStateWindow);
    module_align_inner_top(machineInfoWindow);

    // Create machine info window title
    Module *machineInfoWindowTitle = &(page->machineInfoWindowTitle);
    module_init(machineInfoWindowTitle, machineInfoWindow);
    module_set_text(machineInfoWindowTitle, machineInfoWindowTitleBuffer);
    module_set_margin(machineInfoWindowTitle, 8, 8);
    module_text_font(machineInfoWindowTitle, RA8876_CHAR_HEIGHT_32);
    module_text_fit(machineInfoWindowTitle);
    module_set_color(machineInfoWindowTitle, MAINTEXTCOLOR, MAINCOLOR);
    module_align_inner_top(machineInfoWindowTitle);
    module_align_center(machineInfoWindowTitle);
    module_text_underline(machineInfoWindowTitle);

    Module *forceText = &(page->forceText);
    module_init(forceText, machineInfoWindow);
    module_set_margin(forceText, padding, padding);
    module_set_text(forceText, forceTextBuffer);
    module_text_font(forceText, RA8876_CHAR_HEIGHT_24);
    module_text_fit(forceText);
    module_set_color(forceText, MAINTEXTCOLOR, MAINCOLOR);
    module_align_below(forceText, machineInfoWindowTitle);
    module_align_inner_left(forceText);

    Module *forceValue = &(page->forceValue);
    module_init(forceValue, machineInfoWindow);
    module_copy(forceValue, forceText);
    module_set_text(forceValue, page->forceValueBuffer);
    module_text_font(forceValue, RA8876_CHAR_HEIGHT_24);
    module_text_fit(forceValue);
    module_fit_right(forceValue, forceText);
    module_align_right(forceValue, forceText);
    module_text_align(forceValue, MODULE_TEXT_ALIGN_INNER_RIGHT);

    Module *positionText = &(page->positionText);
    module_init(positionText, machineInfoWindow);
    module_set_margin(positionText, padding, padding);
    module_set_text(positionText, positionTextBuffer);
    module_text_font(positionText, RA8876_CHAR_HEIGHT_24);
    module_text_fit(positionText);
    module_set_color(positionText, MAINTEXTCOLOR, MAINCOLOR);
    module_align_below(positionText, forceText);
    module_align_inner_left(positionText);

    Module *positionValue = &(page->positionValue);
    module_init(positionValue, machineInfoWindow);
    module_copy(positionValue, positionText);
    module_set_text(positionValue, page->positionValueBuffer);
    module_text_font(positionValue, RA8876_CHAR_HEIGHT_24);
    module_text_fit(positionValue);
    module_fit_right(positionValue, positionText);
    module_align_right(positionValue, positionText);
    module_text_align(positionValue, MODULE_TEXT_ALIGN_INNER_RIGHT);

    Module *positionGraphContainer = &(page->positionGraphContainer);
    module_init(positionGraphContainer, machineInfoWindow);
    module_set_margin(positionGraphContainer, 10, 10);
    module_set_size(positionGraphContainer, 0, 200);
    module_align_below(positionGraphContainer, positionText);
    module_align_inner_left(positionGraphContainer);
    module_set_color(positionGraphContainer, positionGraphContainer->parent->foregroundColor, positionGraphContainer->parent->backgroundColor);
    module_fit_width(positionGraphContainer);
    module_set_graph(positionGraphContainer, &(page->positionGraph), "Position", "mm");
    module_graph_set_range(positionGraphContainer, 10.0, -10.0);

    Module *forceGraphContainer = &(page->forceGraphContainer);
    module_init(forceGraphContainer, machineInfoWindow);
    module_set_margin(forceGraphContainer, 10, 10);
    module_set_size(forceGraphContainer, 0, 200);
    module_align_below(forceGraphContainer, positionGraphContainer);
    forceGraphContainer->y += 10;
    module_align_inner_left(forceGraphContainer);
    module_set_color(forceGraphContainer, forceGraphContainer->parent->foregroundColor, forceGraphContainer->parent->backgroundColor);
    module_fit_width(forceGraphContainer);
    module_set_graph(forceGraphContainer, &(page->forceGraph), "Force", "N");
    module_graph_set_range(forceGraphContainer, 5.0, -5.0);

    // Create Function Window Container
    Module *functionWindow = &(page->functionWindow);
    module_init(functionWindow, background);
    module_set_padding(functionWindow, padding, padding);
    module_set_margin(functionWindow, 10, 10);
    module_set_size(functionWindow, 0, 200);
    module_fit_space_even(functionWindow, 3);
    module_align_space_even(functionWindow, 3, 3);
    module_align_inner_bottom(functionWindow);

    function_window_init(&(page->functionWindowData),functionWindow, page->stateMachine);

    // Create Function Window Container
    Module *motionStateWindow = &(page->motionStateWindow);
    module_init(motionStateWindow, background);
    module_set_padding(motionStateWindow, padding, padding);
    module_set_margin(motionStateWindow, 10, 10);
    module_set_size(motionStateWindow, 0, 110);
    module_fit_space_even(motionStateWindow, 3);
    module_align_space_even(motionStateWindow, 3, 3);
    module_align_above(motionStateWindow, functionWindow);

    motion_state_window_init(&(page->motionStateWindowData), motionStateWindow, page->stateMachine);
}

void status_page_run(StatusPage *page)
{
    complete = false;

    module_draw(page->display, &(page->root));

    MachineState previousState = *(page->stateMachine);
    bool initial = true;
    bool updateMachineState = true;

    long lastDataUpdate = 0;

    while (!complete)
    {
        MachineState currentState = *(page->stateMachine);
        /*Self Check State*/
        // charge pump
        if (currentState.selfCheckParameters.chargePump != previousState.selfCheckParameters.chargePump || initial)
        {
            updateMachineState = true;
            if (currentState.selfCheckParameters.chargePump)
            {
                module_set_image(&(page->chargePumpImage), &(page->images->successImage));
            }
            else
            {
                module_set_image(&(page->chargePumpImage), &(page->images->failImage));
            }
        }

        /*Machine Check State*/
        // switched power enabled
        if (currentState.machineCheckParameters.switchedPower != previousState.machineCheckParameters.switchedPower || initial)
        {
            updateMachineState = true;

            if (page->stateMachine->machineCheckParameters.switchedPower)
            {
                module_set_image(&(page->switchedPowerImage), &(page->images->successImage));
            }
            else
            {
                module_set_image(&(page->switchedPowerImage), &(page->images->failImage));
            }
        }

        // Over Travel Limits
        if (currentState.machineCheckParameters.esdTravelLimit != previousState.machineCheckParameters.esdTravelLimit || initial)
        {
            updateMachineState = true;
            if (page->stateMachine->machineCheckParameters.esdTravelLimit == MOTION_LIMIT_OK)
            {
                module_set_image(&(page->esdUpperImage), &(page->images->successImage));
                module_set_image(&(page->estLowerImage), &(page->images->successImage));
            }
            else if (page->stateMachine->machineCheckParameters.esdTravelLimit == MOTION_LIMIT_LOWER)
            {

                module_set_image(&(page->esdUpperImage), &(page->images->successImage));
                module_set_image(&(page->estLowerImage), &(page->images->failImage));
            }
            else if (page->stateMachine->machineCheckParameters.esdTravelLimit == MOTION_LIMIT_UPPER)
            {
                module_set_image(&(page->esdUpperImage), &(page->images->failImage));
                module_set_image(&(page->estLowerImage), &(page->images->successImage));
            }
        }

        // ESD
        if (currentState.machineCheckParameters.esdSwitch != previousState.machineCheckParameters.esdSwitch || initial)
        {
            updateMachineState = true;

            if (page->stateMachine->machineCheckParameters.esdSwitch)
            {
                module_set_image(&(page->estSwitchImage), &(page->images->successImage));
            }
            else
            {
                module_set_image(&(page->estSwitchImage), &(page->images->failImage));
            }
        }

        // Servo OK signal
        if (currentState.machineCheckParameters.servoOK != previousState.machineCheckParameters.servoOK || initial)
        {
            updateMachineState = true;

            if (page->stateMachine->machineCheckParameters.servoOK)
            {
                module_set_image(&(page->servoReadyImage), &(page->images->successImage));
            }
            else
            {
                module_set_image(&(page->servoReadyImage), &(page->images->failImage));
            }
        }

        // Force Gauge
        if (currentState.machineCheckParameters.forceGaugeCom != previousState.machineCheckParameters.forceGaugeCom || initial)
        {
            updateMachineState = true;

            if (page->stateMachine->machineCheckParameters.forceGaugeCom)
            {
                module_set_image(&(page->forceGaugeComImage), &(page->images->successImage));
            }
            else
            {
                module_set_image(&(page->forceGaugeComImage), &(page->images->failImage));
            }
        }

        // Servo Comm
        if (currentState.machineCheckParameters.servoCom != previousState.machineCheckParameters.servoCom || initial)
        {
            updateMachineState = true;

            if (page->stateMachine->machineCheckParameters.servoCom)
            {
                module_set_image(&(page->servoComImage), &(page->images->successImage));
            }
            else
            {
                module_set_image(&(page->servoComImage), &(page->images->failImage));
            }
        }

        module_touch_check(&(page->root), page->display, page); // Check for button presses
        module_update_check(page->display, &(page->root), page);

        // module_draw(page->display, positionGraphContainer);
        // module_draw(page->display, machineStateImages);

        // module_draw(page->display, functionWindow);
        // module_draw(page->display, motionStateWindow);
        if (updateMachineState)
        {
            module_draw(page->display, &(page->machineStateWindow));
            updateMachineState = false;
        }
        while (display_update_touch(page->display) == 0 && state_machine_equal(page->stateMachine, &currentState))
        {
            if ((_getms() - lastDataUpdate) > 100)
            {
                lastDataUpdate = _getms();
                sprintf(page->positionValueBuffer, "%0.3fmm", steps_to_mm(page->data->encoderRaw, &(page->machineProfile->configuration)));
                // module_set_text(&(page->positionValue), page->positionValueBuffer);
                sprintf(page->forceValueBuffer, "%0.3fN", raw_to_force(page->data->forceRaw, &(page->machineProfile->configuration))/1000.0);
                // module_set_text(&(page->forceValue), page->forceValueBuffer);
                module_draw(page->display, &(page->forceValue));
                module_draw(page->display, &(page->positionValue));
            }
            module_graph_insert(&(page->positionGraphContainer), steps_to_mm(page->data->encoderRaw, &(page->machineProfile->configuration)));
            module_draw(page->display, &(page->positionGraphContainer));

            module_graph_insert(&(page->forceGraphContainer), raw_to_force(page->data->forceRaw, &(page->machineProfile->configuration))/1000.0);
            module_draw(page->display, &(page->forceGraphContainer));
        } // Update touch register

        previousState = currentState;
        initial = false;
    }
}
