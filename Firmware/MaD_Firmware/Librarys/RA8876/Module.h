#ifndef MODULE_H
#define MODULE_H
#include "RA8876.h"
#include <stdbool.h>
#include <math.h>
#include "Graph.h"
#include "Images.h"

typedef enum ModuleType_e
{
    MODULE_ROOT,
    MODULE_TEXT,
    MODULE_IMAGE,
    MODULE_LINE,
    MODULE_RECTANGLE,
    MODULE_RECTANGLE_CIRCLE,
    MODULE_GRAPH
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
} Module;

Module *module_create(Module *parent);

void module_touch_callback(Module *module, void (*onTouch)(int id, void *page), int id);
int module_touch_check(Module *root, Display *display, void *arg);

void module_set_visable(Module *module, bool visible);
void module_copy(Module *to, Module *from);

void module_set_line(Module *module, int w);
void module_set_rectangle(Module *module, int w, int h);
void module_set_rectangle_circle(Module *module, int w, int h);
void module_set_image(Module *module, Image *image);
void module_set_graph(Module *module, double *data, int dataCount, const char *units, const char *title);
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
void module_align_above(Module *module, Module *ref);
void module_align_left(Module *module, Module *ref);
void module_align_below(Module *module, Module *ref);
void module_align_right(Module *module, Module *ref);

void module_fit_width(Module *module);
void module_fit_height(Module *module);
void module_fit_below(Module *module, Module *ref);

void module_draw(Display *display, Module *module);

void module_trim(Module *module);
void module_destroy(Module *root);
#endif
