#ifndef MODULE_H
#define MODULE_H
#include "RA8876.h"
#include <stdbool.h>
#include <math.h>
#include "Images.h"

#define MAX_GRAPH_MARKERS 10

typedef enum ModuleType_e
{
    MODULE_ROOT,
    MODULE_TEXT,
    MODULE_IMAGE,
    MODULE_LINE,
    MODULE_RECTANGLE,
    MODULE_RECTANGLE_CIRCLE,
    MODULE_GRAPH,
    MODULE_WINDOW
} ModuleType;

typedef struct Module_s
{
    struct Module_s *parent;
    struct Module_s **child;
    ModuleType type;
    void *data;
    int numChildren;
    int x, y, w, h, px, py;
    int backgroundColor, foregroundColor;
    bool visible;
    int lastPressed;
    int debouncems;
    int touchId;
    void (*onTouch)(int id, void *arg);
    void (*onUpdate)(struct Module_s *module, void *arg);
} Module;

typedef struct graph_t
{
    int dataCount;
    double *data;
    double maxY;
    double minY;
    Module *graphArea;
    float markerY[MAX_GRAPH_MARKERS];
    int markerCount;
    char *title;
    char *units;
} Graph;

Module *module_create(Module *parent);

void module_update_callback(Module *module, void (*onUpdate)(Module *module, void *arg));
void module_update_check(Module *module, void *arg);

void module_touch_callback(Module *module, void (*onTouch)(int id, void *page), int id);
int module_touch_check(Module *root, Display *display, void *arg);

void module_set_visable(Module *module, bool visible);
void module_copy(Module *to, Module *from);

void module_save(Module *module, Display *display);
void module_paste(Module *module, Display *display);

void module_set_graph(Module *window);
void module_graph_add_marker(Module *module, float value);
void module_graph_insert(Module *module, double value);
void module_graph_set_range(Module *module, float maxY, float minY);

void module_set_window(Module *module, void *window);
void module_set_line(Module *module, int w);
void module_set_line_one(Module *module, int x, int y);
void module_set_rectangle(Module *module, int w, int h);
void module_set_rectangle_circle(Module *module, int w, int h);
void module_set_image(Module *module, Image *image);
void module_add_underline(Module *module);

void module_set_text(Module *module, char *text);
void module_set_font(Module *module, int font);
void module_set_padding(Module *module, int px, int py);
void module_set_color(Module *module, int foreground, int background);
void module_set_position(Module *module, int x, int y);
void module_set_size(Module *module, int w, int h);

void module_align_inner_top(Module *module);
void module_align_inner_left(Module *module);
void module_align_inner_right(Module *module);
void module_align_inner_bottom(Module *module);

void module_align_middle(Module *module);
void module_align_center(Module *module);
void module_align_center_sector(Module *module, int section, int sections);
void module_align_space_even(Module *module, int section, int sections);
void module_align_space_even_verticle(Module *module, int section, int sections);
void module_align_above(Module *module, Module *ref);
void module_align_left(Module *module, Module *ref);
void module_align_below(Module *module, Module *ref);
void module_align_right(Module *module, Module *ref);

void module_fit_width(Module *module);
void module_fit_space_even(Module *module, int sections);
void module_fit_space_even_verticle(Module *module, int sections);
void module_fit_height(Module *module);
void module_fit_below(Module *module, Module *ref);

void module_draw(Display *display, Module *module);

void module_destroy_children(Module *module);
void module_trim(Module *module);
void module_destroy(Module *root);
#endif
