#include "Graph.h"
#include "Module.h"
Graph *graph_create(double maxY, double minY, double minX, const char *units, const char *title)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->display = display;
    graph->maxY = maxY;
    graph->minY = minY;
    graph->minX = minX;
    graph->lastY = NULL;
    graph->markerCount = 0;

    graph->units = (char *)malloc(strlen(units) + 1);
    strcpy(graph->units, units);

    graph->title = (char *)malloc(sizeof(char) * (strlen(title) + 1));
    strcpy(graph->title, title);

    return graph;
}

void graph_destroy(Graph *graph)
{
    free(graph->title);
    free(graph->units);
    free(graph->data);
    free(graph);
}

void module_graph_add_marker(Module *module, float value)
{
    Graph *graph = (Graph *)module->data;
    if (graph->markerCount < MAX_GRAPH_MARKERS)
    {
        graph->markerY[graph->markerCount] = value;
        graph->markerCount++;
    }
}

// Makes a empty window a graph, have position and size already defined
// Creates an empty graph that has data of size of the centerLine in pixels
void module_graph_set(Module *window)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));

    // Create Title
    Module *title = module_create(window);
    module_set_padding(title, 10, 10);
    module_set_text(title, graph->title);
    module_set_font(title, RA8876_CHAR_HEIGHT_32);
    module_set_color(title, COLOR65K_BLACK, title->parent->foregroundColor);
    module_align_inner_top(title);
    module_align_center(title);
    module_add_underline(title);

    // Create Graph Area
    Module *graphArea = module_create(window);
    module_align_below(graphArea, title);
    module_fit_below(graphArea, title);
    module_fit_width(graphArea);
    module_align_inner_left(graphArea);
    module_set_color(graphArea, graphArea->parent->foregroundColor, graphArea->parent->backgroundColor);
    module_set_padding(graphArea, 0, 0);

    // Create Graph Horizontal Line
    Module *centerLine = module_create(graphArea);
    module_set_rectangle(centerLine, 0, 0);
    module_fit_width(centerLine);
    module_align_center(centerLine);
    module_align_middle(centerLine);
    module_set_color(centerLine, COLOR65K_BLACK, COLOR65K_BLACK);

    // Create Graph Vertical Line
    Module *verticleLine = module_create(graphArea);
    module_set_rectangle(verticleLine, 0, 0);
    module_fit_height(verticleLine);
    module_align_inner_left(verticleLine);
    module_align_inner_top(verticleLine);
    module_set_color(verticleLine, COLOR65K_BLACK, COLOR65K_BLACK);

    graph->graphArea = graphArea;

    int maxY = graph->data[0];
    int minY = graph->data[0];

    for (int i = 0; i < graphArea->w; i++)
    {
        if (graph->data[i] > maxY)
        {
            maxY = graph->data[i];
        }
        if (graph->data[i] < minY)
        {
            minY = graph->data[i];
        }
    }

    int lastX = graph->startX;
    for (int i = 0; i < graphArea->w; i++)
    {
        int x = graph->startX + i;
        int y = graph->startY - (int)(graph->data[i] * (double)((graphArea->h / 2) / (maxY - minY)));
        Module *point = module_create(graphArea);
        module_set_rectangle(point, x - lastX, 1);
        module_set_position(point, lastX, y);
        module_set_color(point, COLOR65K_GREEN, COLOR65K_GREEN);
        // printf("creating point at: %d, %d,%d\n", x, y, graph->data[i]);
        lastX = x;
    }

    window->data = graph;
}

// Data length must match width of graph
/*bool graph_draw_static(Graph *graph, Display *display, int x, int y, int w, int h)
{
    // Create Background
    Module *root = module_create(NULL);

    // Create Window
    Module *window = module_create(root);
    module_set_rectangle_circle(window, w, h);
    module_set_position(window, x, y);
    module_set_padding(window, 10, 10);
    module_set_color(window, COLOR65K_LIGHTBLUE, COLOR65K_BLUE);

    // Create Title
    Module *title = module_create(window);
    module_set_padding(title, 10, 10);
    module_set_text(title, graph->title);
    module_set_font(title, RA8876_CHAR_HEIGHT_32);
    module_set_color(title, COLOR65K_BLACK, title->parent->foregroundColor);
    module_align_inner_top(title);
    module_align_center(title);
    module_add_underline(title);

    // Create Graph Area
    Module *graphArea = module_create(window);
    module_align_below(graphArea, title);
    module_fit_below(graphArea, title);
    module_fit_width(graphArea);
    module_align_inner_left(graphArea);
    module_set_color(graphArea, graphArea->parent->foregroundColor, graphArea->parent->backgroundColor);
    module_set_padding(graphArea, 0, 0);

    // Create Graph Horizontal Line
    Module *centerLine = module_create(graphArea);
    module_set_rectangle(centerLine, 0, 0);
    module_fit_width(centerLine);
    module_align_center(centerLine);
    module_align_middle(centerLine);
    module_set_color(centerLine, COLOR65K_BLACK, COLOR65K_BLACK);

    // Create Graph Vertical Line
    Module *verticleLine = module_create(graphArea);
    module_set_rectangle(verticleLine, 0, 0);
    module_fit_height(verticleLine);
    module_align_inner_left(verticleLine);
    module_align_inner_top(verticleLine);
    module_set_color(verticleLine, COLOR65K_BLACK, COLOR65K_BLACK);

    graph->width = centerLine->w;
    graph->height = verticleLine->h;
    graph->startX = centerLine->x;
    graph->startY = centerLine->y;

    double factor = (double)graph->width / (double)graph->dataCount;

    graph->maxY = graph->data[0];
    graph->minY = graph->data[0];

    for (int i = 0; i < graph->dataCount; i++)
    {
        if (graph->data[i] > graph->maxY)
        {
            graph->maxY = graph->data[i];
        }
        if (graph->data[i] < graph->minY)
        {
            graph->minY = graph->data[i];
        }
    }

    int lastX = graph->startX;
    for (int i = 0; i < graph->dataCount; i++)
    {
        int x = graph->startX + (int)(i * factor);
        int y = graph->startY - (int)(graph->data[i] * (double)((graph->height / 2) / (graph->maxY - graph->minY)));
        Module *point = module_create(graphArea);
        module_set_rectangle(point, x - lastX, 1);
        module_set_position(point, lastX, y);
        module_set_color(point, COLOR65K_GREEN, COLOR65K_GREEN);
        // printf("creating point at: %d, %d,%d\n", x, y, graph->data[i]);
        lastX = x;
    }

    module_draw(display, root);
    module_destroy(root);
}*/

// make graph an object so it has memory!
bool graph_draw_dynamic(Graph *graph, Display *display, int x, int y, int w, int h)
{

    // Create Background
    Module *root = module_create(NULL);

    // Create Window
    Module *window = module_create(root);
    module_set_size(window, w, h);
    module_set_position(window, x, y);
    module_set_padding(window, 10, 10);
    module_set_color(window, COLOR65K_LIGHTBLUE, COLOR65K_BLUE);

    // Create WIndow and initialize if first time (free data to reset graph)
    if (graph->data == NULL)
    {
        graph->data = (double *)malloc(sizeof(double));
        module_set_rectangle_circle(window, w, h); // Draw window on first time
    }

    // Create Title
    Module *title = module_create(window);
    module_set_padding(title, 10, 10);
    module_set_text(title, graph->title);
    module_set_font(title, RA8876_CHAR_HEIGHT_32);
    module_set_color(title, COLOR65K_BLACK, title->parent->foregroundColor);
    module_align_inner_top(title);
    module_align_center(title);
    module_add_underline(title);

    // Create Graph Area
    Module *graphArea = module_create(window);
    module_align_below(graphArea, title);
    module_fit_below(graphArea, title);
    module_fit_width(graphArea);
    module_align_inner_left(graphArea);
    module_set_color(graphArea, graphArea->parent->foregroundColor, graphArea->parent->backgroundColor);
    module_set_padding(graphArea, 0, 0);

    // Create Graph Horizontal Line
    Module *centerLine = module_create(graphArea);
    module_set_rectangle(centerLine, 0, 0);
    module_fit_width(centerLine);
    module_align_center(centerLine);
    module_align_middle(centerLine);
    module_set_color(centerLine, COLOR65K_BLACK, COLOR65K_BLACK);

    // Create Graph Vertical Line
    Module *verticleLine = module_create(graphArea);
    module_set_rectangle(verticleLine, 0, 0);
    module_fit_height(verticleLine);
    module_align_inner_left(verticleLine);
    module_align_inner_top(verticleLine);
    module_set_color(verticleLine, COLOR65K_BLACK, COLOR65K_BLACK);

    graph->width = centerLine->w;
    graph->height = verticleLine->h;
    graph->startX = centerLine->x;
    graph->startY = centerLine->y;

    // Copy Graph Data
    display_bte_memory_copy(display, PAGE1_START_ADDR, SCREEN_WIDTH, graph->startX + 2, graph->startY - graph->height / 2, PAGE1_START_ADDR, SCREEN_WIDTH, graph->startX, graph->startY - graph->height / 2, graph->width - 2, graph->height);

    module_draw(display, root);

    // display_draw_line(display, graph->startX, graph->startY, graph->startX, graph->startY + graph->height, COLOR65K_BLACK);
    // display_draw_line(display, graph->startX, graph->startY + graph->height, graph->startX + graph->width, graph->startY + graph->height, COLOR65K_BLACK);

    // Draw markerts
    int relativeHeight = ((newValue - graph->minY) / (graph->maxY - graph->minY)) * (graph->height); // Scale new value to graphs height
    if (relativeHeight < 0)
    {
        relativeHeight = 0;
    }
    else if (relativeHeight > graph->height)
    {
        relativeHeight = graph->height;
    }
    int x = graph->startX + graph->width;
    int y = (int)(graph->startY + graph->height - relativeHeight);

    if (graph->lastX == NULL)
    {
        printf("init\n");
        graph->lastX = x;
        graph->lastY = y;
    }

    // Erase old information
    display_draw_square(display, x - 2, graph->startY - graph->height / 2, graph->startX + graph->width, graph->startX + graph->width - 2, graph->startY - graph->height / 2, COLOR65K_BLACK);
    // display_draw_line(display, graph->lastX - 2, graph->startY, graph->lastX - 2, graph->startY + graph->height, MAINCOLOR);
    // display_draw_line(display, x, graph->startY, x, graph->startY + graph->height, MAINCOLOR);

    // Draw new value
    display_draw_line(display, graph->lastX - 2, graph->lastY, x, y, COLOR65K_GREEN);

    // draw markers
    for (int i = 0; i < graph->markerCount; i++)
    {
        int markerY = (int)(graph->startY + graph->height - ((graph->markerY[i] - graph->minY) / (graph->maxY - graph->minY)) * (graph->height));
        display_draw_line(display, graph->startX, markerY, graph->startX + graph->width, markerY, COLOR65K_RED);
    }

    graph->lastX = x;
    graph->lastY = y;
    return true;
}
