#include "MachineView.h"

static char title[] = "Machine View";

static void redraw_position(Display *display, Module *module, void *arg)
{
    MachineView *page = (MachineView *)arg;
    char buf[50];
    sprintf(buf, "%0.3fmm", page->data->position);
    if (strcmp(buf, page->positionValueBuffer) == 0)
        return;
    strcpy(page->positionValueBuffer, buf);
    module_draw(display, module);
}

static void redraw_force(Display *display, Module *module, void *arg)
{
    MachineView *page = (MachineView *)arg;
    char buf[50];
    sprintf(buf, "%0.3fN", page->data->force);
    if (strcmp(buf, page->forceValueBuffer) == 0)
        return;
    strcpy(page->forceValueBuffer, buf);
    module_draw(display, module);
}

void machine_view_init(MachineView *view, Module *container, MonitorData *data)
{
    int padding = 10;
    view->data = data;

    module_set_window(container,view);

    strcpy(view->forceValueBuffer, "0.0N");
    strcpy(view->positionValueBuffer, "0.0mm");

    // Create edit window
    Module *window = &(view->window);
    module_init(window, container);
    module_set_rectangle_circle(window, 0, 0);
    module_add_border(window, COLOR65K_WHITE, 1);
    module_fit_height(window);
    module_fit_width(window);
    module_set_padding(window, 10, 10);
    module_set_color(window, MAINCOLOR, BACKCOLOR);
    module_align_inner_left(window);
    module_align_inner_top(window);

    // Create machine info window title
    Module *machineInfoWindowTitle = &(view->title);
    module_init(machineInfoWindowTitle, window);
    module_set_text(machineInfoWindowTitle, title);
    module_set_margin(machineInfoWindowTitle, 8, 8);
    module_text_font(machineInfoWindowTitle, RA8876_CHAR_HEIGHT_32);
    module_text_fit(machineInfoWindowTitle);
    module_set_color(machineInfoWindowTitle, MAINTEXTCOLOR, MAINCOLOR);
    module_align_inner_top(machineInfoWindowTitle);
    module_align_center(machineInfoWindowTitle);
    module_text_underline(machineInfoWindowTitle);

    Module *forceText = &(view->forceValue);
    module_init(forceText, window);
    module_set_margin(forceText, padding, padding);
    module_set_text(forceText, view->forceValueBuffer);
    module_text_font(forceText, RA8876_CHAR_HEIGHT_24);
    module_text_fit(forceText);
    module_fit_width(forceText);
    module_set_color(forceText, MAINTEXTCOLOR, MAINCOLOR);
    module_align_below(forceText, machineInfoWindowTitle);
    module_align_inner_left(forceText);
    module_redraw_callback(forceText, redraw_force);

    Module *positionText = &(view->positionValue);
    module_init(positionText, window);
    module_set_margin(positionText, padding, padding);
    module_set_text(positionText, view->positionValueBuffer);
    module_text_font(positionText, RA8876_CHAR_HEIGHT_24);
    module_text_fit(positionText);
    module_set_color(positionText, MAINTEXTCOLOR, MAINCOLOR);
    module_align_below(positionText, forceText);
    module_align_inner_left(positionText);
    module_redraw_callback(positionText, redraw_position);

    Module *positionGraphContainer = &(view->positionContainer);
    module_init(positionGraphContainer, window);
    module_set_margin(positionGraphContainer, 10, 10);
    module_set_size(positionGraphContainer, 0, 200);
    module_align_below(positionGraphContainer, positionText);
    module_align_inner_left(positionGraphContainer);
    module_set_color(positionGraphContainer, positionGraphContainer->parent->foregroundColor, positionGraphContainer->parent->backgroundColor);
    module_fit_width(positionGraphContainer);
    module_set_graph(positionGraphContainer, &(view->positionGraph), "Position", "mm", &(view->data->position));
    module_graph_set_range(positionGraphContainer, 10.0, -10.0);

    Module *forceGraphContainer = &(view->forceContainer);
    module_init(forceGraphContainer, window);
    module_set_margin(forceGraphContainer, 10, 10);
    module_set_size(forceGraphContainer, 0, 200);
    module_align_below(forceGraphContainer, positionGraphContainer);
    forceGraphContainer->y += 10;
    module_align_inner_left(forceGraphContainer);
    module_set_color(forceGraphContainer, forceGraphContainer->parent->foregroundColor, forceGraphContainer->parent->backgroundColor);
    module_fit_width(forceGraphContainer);
    module_set_graph(forceGraphContainer, &(view->forceGraph), "Force", "N",&(view->data->force));
    module_graph_set_range(forceGraphContainer, 5.0, -5.0);
}
