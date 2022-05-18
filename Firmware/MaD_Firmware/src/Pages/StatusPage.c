// status page
#include "StatusPage.h"
#include "Images.h"

#define BUTTONCOUNT 2
#define BUTTON_MACHINE 0
#define BUTTON_NAVIGATION 1

static void button_navigation(int id, void *arg)
{
    StatusPage *page = (StatusPage *)arg;
    page->complete = true;
}

static void check_buttons(StatusPage *page)
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
                default:
                    break;
                }
            }
        }
    }
}

static void drawSuccessIndicator(StatusPage *page, int x, int y)
{
    Image *check = page->images->successImage;
    display_draw_square_fill(page->display, x, y, x + check->width - 1, y + check->height - 1, MAINCOLOR);
    display_bte_memory_copy_image(page->display, check, x, y);
}

static void drawFailIndicator(StatusPage *page, int x, int y)
{
    Image *ex = page->images->failImage;
    display_draw_square_fill(page->display, x, y, x + ex->width - 1, y + ex->height - 1, MAINCOLOR);
    display_bte_memory_copy_image(page->display, ex, x, y);
}

StatusPage *status_page_create(Display *display, MachineState *machineState, MonitorData *data, Images *images)
{
    StatusPage *page = malloc(sizeof(StatusPage));
    page->display = display;
    page->stateMachine = machineState;
    page->complete = false;
    page->data = data;
    page->images = images;
    return page;
}

void status_page_destroy(StatusPage *page)
{
    free(page->buttons);
    free(page);
}

void status_page_run(StatusPage *page)
{
    printf("Status page running\n");

    int padding = 8;
    // Create Background
    Module *root = module_create(NULL);
    Module *background = module_create(root);
    module_set_rectangle_circle(background, SCREEN_WIDTH, SCREEN_HEIGHT);
    module_set_position(background, 0, 0);
    module_set_padding(background, padding, padding);
    module_set_color(background, BACKCOLOR, BACKCOLOR);

    // Create navigation button
    Module *navigationButton = module_create(background);
    module_set_image(navigationButton, page->images->navigationImage);
    module_align_inner_top(navigationButton);
    module_align_inner_right(navigationButton);
    module_touch_callback(navigationButton, button_navigation, 0);

    // Create machine state window
    Module *machineStateWindow = module_create(background);
    module_set_padding(machineStateWindow, padding, padding);
    module_set_rectangle_circle(machineStateWindow, 0, 0);
    module_fit_space_even(machineStateWindow, 3);
    module_fit_height(machineStateWindow);
    module_set_padding(machineStateWindow, padding, padding);
    module_set_color(machineStateWindow, MAINCOLOR, BACKCOLOR);
    module_align_space_even(machineStateWindow, 1, 3);
    module_align_inner_top(machineStateWindow);

    Module *machineStateImages = module_create(machineStateWindow);
    module_set_padding(machineStateImages, 0, 0);
    module_align_inner_left(machineStateImages);
    module_align_inner_top(machineStateImages);
    module_fit_width(machineStateImages);
    module_fit_height(machineStateImages);

    // Create machine state window title
    Module *machineStateWindowTitle = module_create(machineStateWindow);
    module_set_padding(machineStateWindowTitle, padding, padding);
    module_set_text(machineStateWindowTitle, "Machine State");
    module_set_font(machineStateWindowTitle, RA8876_CHAR_HEIGHT_32);
    module_set_color(machineStateWindowTitle, COLOR65K_BLACK, MAINCOLOR);
    module_align_inner_top(machineStateWindowTitle);
    module_align_center(machineStateWindowTitle);
    module_add_underline(machineStateWindowTitle);

    // Charge Pump
    Module *chargePumpText = module_create(machineStateWindow);
    module_set_padding(chargePumpText, padding, padding);
    module_set_text(chargePumpText, "Charge Pump");
    module_set_font(chargePumpText, RA8876_CHAR_HEIGHT_24);
    module_set_color(chargePumpText, COLOR65K_BLACK, MAINCOLOR);
    module_align_below(chargePumpText, machineStateWindowTitle);
    module_align_inner_left(chargePumpText);

    Module *chargePumpImage = module_create(machineStateImages);
    module_set_image(chargePumpImage, page->images->failImage);
    module_align_below(chargePumpImage, machineStateWindowTitle);
    module_align_inner_right(chargePumpImage);

    // Switched Power
    Module *switchedPowerText = module_create(machineStateWindow);
    module_copy(switchedPowerText, chargePumpText);
    module_set_text(switchedPowerText, "Switched Power");
    module_set_font(switchedPowerText, RA8876_CHAR_HEIGHT_24);
    module_align_below(switchedPowerText, chargePumpText);

    Module *switchedPowerImage = module_create(machineStateImages);
    module_set_image(switchedPowerImage, page->images->failImage);
    module_align_below(switchedPowerImage, chargePumpText);
    module_align_inner_right(switchedPowerImage);

    // ESD Switch
    Module *estSwitchText = module_create(machineStateWindow);
    module_copy(estSwitchText, chargePumpText);
    module_set_text(estSwitchText, "ESD Switch");
    module_set_font(estSwitchText, RA8876_CHAR_HEIGHT_24);
    module_align_below(estSwitchText, switchedPowerText);

    Module *estSwitchImage = module_create(machineStateImages);
    module_set_image(estSwitchImage, page->images->failImage);
    module_align_below(estSwitchImage, switchedPowerText);
    module_align_inner_right(estSwitchImage);

    // ESD Upper
    Module *esdUpperText = module_create(machineStateWindow);
    module_copy(esdUpperText, chargePumpText);
    module_set_text(esdUpperText, "ESD Upper");
    module_set_font(esdUpperText, RA8876_CHAR_HEIGHT_24);
    module_align_below(esdUpperText, estSwitchText);

    Module *esdUpperImage = module_create(machineStateImages);
    module_set_image(esdUpperImage, page->images->failImage);
    module_align_below(esdUpperImage, estSwitchText);
    module_align_inner_right(esdUpperImage);

    // ESD Lower
    Module *estLowerText = module_create(machineStateWindow);
    module_copy(estLowerText, chargePumpText);
    module_set_text(estLowerText, "ESD Lower");
    module_set_font(estLowerText, RA8876_CHAR_HEIGHT_24);
    module_align_below(estLowerText, esdUpperText);

    Module *estLowerImage = module_create(machineStateImages);
    module_set_image(estLowerImage, page->images->failImage);
    module_align_below(estLowerImage, esdUpperText);
    module_align_inner_right(estLowerImage);

    // Servo Ready
    Module *servoReadyText = module_create(machineStateWindow);
    module_copy(servoReadyText, chargePumpText);
    module_set_text(servoReadyText, "Servo Ready");
    module_set_font(servoReadyText, RA8876_CHAR_HEIGHT_24);
    module_align_below(servoReadyText, estLowerText);

    Module *servoReadyImage = module_create(machineStateImages);
    module_set_image(servoReadyImage, page->images->failImage);
    module_align_below(servoReadyImage, estLowerText);
    module_align_inner_right(servoReadyImage);

    // Force Gauge Comm
    Module *forceGaugeComText = module_create(machineStateWindow);
    module_copy(forceGaugeComText, chargePumpText);
    module_set_text(forceGaugeComText, "Force Comm");
    module_set_font(forceGaugeComText, RA8876_CHAR_HEIGHT_24);
    module_align_below(forceGaugeComText, servoReadyText);

    Module *forceGaugeComImage = module_create(machineStateImages);
    module_set_image(forceGaugeComImage, page->images->failImage);
    module_align_below(forceGaugeComImage, servoReadyText);
    module_align_inner_right(forceGaugeComImage);

    // Servo Comm
    Module *servoComText = module_create(machineStateWindow);
    module_copy(servoComText, chargePumpText);
    module_set_text(servoComText, "Servo Comm");
    module_set_font(servoComText, RA8876_CHAR_HEIGHT_24);
    module_align_below(servoComText, forceGaugeComText);

    Module *servoComImage = module_create(machineStateImages);
    module_set_image(servoComImage, page->images->failImage);
    module_align_below(servoComImage, forceGaugeComText);
    module_align_inner_right(servoComImage);

    // Create machine info window
    Module *machineInfoWindow = module_create(background);
    module_set_padding(machineInfoWindow, padding, padding);
    module_set_rectangle_circle(machineInfoWindow, 0, 0);
    module_fit_space_even(machineInfoWindow, 3);
    module_set_padding(machineInfoWindow, padding, padding);
    module_fit_height(machineInfoWindow);
    module_set_color(machineInfoWindow, MAINCOLOR, BACKCOLOR);
    module_align_space_even(machineInfoWindow, 2, 3);
    module_align_inner_top(machineInfoWindow);

    // Create machine info window title
    Module *machineInfoWindowTitle = module_create(machineInfoWindow);
    module_set_padding(machineInfoWindowTitle, padding, padding);
    module_set_text(machineInfoWindowTitle, "Machine Info");
    module_set_font(machineInfoWindowTitle, RA8876_CHAR_HEIGHT_32);
    module_set_color(machineInfoWindowTitle, COLOR65K_BLACK, MAINCOLOR);
    module_align_inner_top(machineInfoWindowTitle);
    module_align_center(machineInfoWindowTitle);
    module_add_underline(machineInfoWindowTitle);

    Module *forceText = module_create(machineInfoWindow);
    module_set_padding(forceText, padding, padding);
    module_set_text(forceText, "Force (N)");
    module_set_font(forceText, RA8876_CHAR_HEIGHT_24);
    module_set_color(forceText, COLOR65K_BLACK, MAINCOLOR);
    module_align_below(forceText, machineInfoWindowTitle);
    module_align_inner_left(forceText);

    Module *forceValue = module_create(machineInfoWindow);
    module_copy(forceValue, forceText);
    module_set_text(forceValue, "0.00");
    module_text_max_char(forceValue, 10);
    module_set_font(forceValue, RA8876_CHAR_HEIGHT_24);
    module_text_align(forceValue, MODULE_TEXT_ALIGN_RIGHT);

    Module *positionText = module_create(machineInfoWindow);
    module_copy(positionText, forceText);
    module_set_text(positionText, "Position (mm)");
    module_set_font(positionText, RA8876_CHAR_HEIGHT_24);
    module_align_below(positionText, forceText);

    Module *positionValue = module_create(machineInfoWindow);
    module_copy(positionValue, positionText);
    module_set_text(positionValue, "0.00");
    module_text_max_char(positionValue, 10);
    module_set_font(positionValue, RA8876_CHAR_HEIGHT_24);
    module_text_align(positionValue, MODULE_TEXT_ALIGN_RIGHT);

    Module *positionGraph = module_create(machineInfoWindow);
    module_set_padding(positionGraph, padding, padding);
    module_align_below(positionGraph, positionText);
    module_align_inner_left(positionGraph);
    module_set_size(positionGraph, 0, 200);
    module_fit_width(positionGraph);
    module_set_graph(positionGraph);
    module_graph_set_range(positionGraph, -100, 100);

    Module *forceGraph = module_create(machineInfoWindow);
    module_set_padding(forceGraph, padding, padding);
    module_align_below(forceGraph, positionGraph);
    module_align_inner_left(forceGraph);
    module_set_size(forceGraph, 0, 200);
    module_fit_width(forceGraph);
    module_set_graph(forceGraph);
    module_graph_set_range(forceGraph, -5, 5);

    // Create Function Window Container
    Module *functionWindow = module_create(background);
    module_set_padding(functionWindow, padding, padding);
    module_set_size(functionWindow, 0, 200);
    module_fit_space_even(functionWindow, 3);
    module_align_space_even(functionWindow, 3, 3);
    module_align_inner_bottom(functionWindow);

    function_window_create(functionWindow, page->stateMachine);

    // Create Function Window Container
    Module *motionStateWindow = module_create(background);
    module_set_padding(motionStateWindow, padding, padding);
    module_set_size(motionStateWindow, 0, 100);
    module_fit_space_even(motionStateWindow, 3);
    module_align_space_even(motionStateWindow, 3, 3);
    module_align_above(motionStateWindow, functionWindow);

    motion_state_window_create(motionStateWindow, page->stateMachine);

    module_draw(page->display, root);

    MachineState previousState = *(page->stateMachine);
    bool initial = true;
    bool updateMachineState = true;

    long lastDataUpdate = 0;

    while (page->complete == false)
    {
        MachineState currentState = *(page->stateMachine);

        /*Self Check State*/
        // charge pump
        if (currentState.selfCheckParameters.chargePump != previousState.selfCheckParameters.chargePump || initial)
        {
            updateMachineState = true;
            if (currentState.selfCheckParameters.chargePump)
            {
                module_set_image(chargePumpImage, page->images->successImage);
            }
            else
            {
                module_set_image(chargePumpImage, page->images->failImage);
            }
        }

        /*Machine Check State*/
        // switched power enabled
        if (currentState.machineCheckParameters.switchedPower != previousState.machineCheckParameters.switchedPower || initial)
        {
            updateMachineState = true;

            if (page->stateMachine->machineCheckParameters.switchedPower)
            {
                module_set_image(switchedPowerImage, page->images->successImage);
            }
            else
            {
                module_set_image(switchedPowerImage, page->images->failImage);
            }
        }

        // Over Travel Limits
        if (currentState.machineCheckParameters.esdTravelLimit != previousState.machineCheckParameters.esdTravelLimit || initial)
        {
            updateMachineState = true;
            if (page->stateMachine->machineCheckParameters.esdTravelLimit == MOTION_LIMIT_OK)
            {
                module_set_image(esdUpperImage, page->images->successImage);
                module_set_image(estLowerImage, page->images->successImage);
            }
            else if (page->stateMachine->machineCheckParameters.esdTravelLimit == MOTION_LIMIT_LOWER)
            {

                module_set_image(esdUpperImage, page->images->successImage);
                module_set_image(estLowerImage, page->images->failImage);
            }
            else if (page->stateMachine->machineCheckParameters.esdTravelLimit == MOTION_LIMIT_UPPER)
            {
                module_set_image(esdUpperImage, page->images->failImage);
                module_set_image(estLowerImage, page->images->successImage);
            }
        }

        // ESD
        if (currentState.machineCheckParameters.esdSwitch != previousState.machineCheckParameters.esdSwitch || initial)
        {
            updateMachineState = true;

            if (page->stateMachine->machineCheckParameters.esdSwitch)
            {
                module_set_image(estSwitchImage, page->images->successImage);
            }
            else
            {
                module_set_image(estSwitchImage, page->images->failImage);
            }
        }

        // Servo OK signal
        if (currentState.machineCheckParameters.servoOK != previousState.machineCheckParameters.servoOK || initial)
        {
            updateMachineState = true;

            if (page->stateMachine->machineCheckParameters.servoOK)
            {
                module_set_image(servoReadyImage, page->images->successImage);
            }
            else
            {
                module_set_image(servoReadyImage, page->images->failImage);
            }
        }

        // Force Gauge
        if (currentState.machineCheckParameters.forceGaugeCom != previousState.machineCheckParameters.forceGaugeCom || initial)
        {
            updateMachineState = true;

            if (page->stateMachine->machineCheckParameters.forceGaugeCom)
            {
                module_set_image(forceGaugeComImage, page->images->successImage);
            }
            else
            {
                module_set_image(forceGaugeComImage, page->images->failImage);
            }
        }

        // Servo Comm
        if (currentState.machineCheckParameters.servoCom != previousState.machineCheckParameters.servoCom || initial)
        {
            updateMachineState = true;

            if (page->stateMachine->machineCheckParameters.servoCom)
            {
                module_set_image(servoComImage, page->images->successImage);
            }
            else
            {
                module_set_image(servoComImage, page->images->failImage);
            }
        }

        module_touch_check(root, page->display, page); // Check for button presses
        module_update_check(page->display, root, page);

        // module_draw(page->display, positionGraph);
        // module_draw(page->display, machineStateImages);

        // module_draw(page->display, functionWindow);
        // module_draw(page->display, motionStateWindow);

        if (updateMachineState)
        {
            module_draw(page->display, machineStateWindow);
            updateMachineState = false;
        }

        while (display_update_touch(page->display) == 0 && state_machine_equal(page->stateMachine, &currentState))
        {
            if ((_getms() - lastDataUpdate) > 100)
            {
                lastDataUpdate = _getms();
                char buf[10];
                sprintf(buf, "%0.3fmm", page->data->positionum / 1000.0);
                module_set_text(positionValue, buf);
                sprintf(buf, "%0.3fN", page->data->force / 1000.0);
                module_set_text(forceValue, buf);
                module_draw(page->display, forceValue);
                module_draw(page->display, positionValue);
            }
            module_graph_insert(positionGraph, page->data->positionum / 1000.0);
            module_draw(page->display, positionGraph);

            module_graph_insert(forceGraph, page->data->force / 1000.0);
            module_draw(page->display, forceGraph);
        } // Update touch register

        previousState = currentState;
        initial = false;
    }
}