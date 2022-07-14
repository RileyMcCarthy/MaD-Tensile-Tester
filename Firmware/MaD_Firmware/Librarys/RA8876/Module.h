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
    MODULE_KEYBOARD,
    MODULE_WINDOW
} ModuleType;

#define MAX_GRAPH_MARKERS 10
#define MAX_GRAPH_TITLE_LENGTH 20
#define MAX_GRAPH_UNITS_LENGTH 10

typedef enum ModuleTextAignType_e
{
    MODULE_TEXT_ALIGN_NONE,
    MODULE_TEXT_ALIGN_INNER_LEFT,
    MODULE_TEXT_ALIGN_INNER_CENTER,
    MODULE_TEXT_ALIGN_INNER_RIGHT,
    MODULE_TEXT_ALIGN_VTOP,
    MODULE_TEXT_ALIGN_VCENTER,
    MODULE_TEXT_ALIGN_VBOTTOM,
} ModuleTextAignType;

typedef struct ModuleText_s
{
    char *value;
    int font;
    bool underline;
    ModuleTextAignType alignmentH;
    ModuleTextAignType alignmentV;
} ModuleText;

#define MODULE_MAX_CHILDREN 50

typedef union Data_u
{
    void *ptr;
    ModuleText text;
    Image *image;
} Data;

typedef struct Module_s
{
    struct Module_s *parent;
    struct Module_s *child[MODULE_MAX_CHILDREN];
    ModuleType type;
    Data data;
    int numChildren;
    int x, y, w, h, px, py, mx, my, r;
    int backgroundColor, foregroundColor, borderColor;
    bool visible;
    int lastPressed;
    int debouncems;
    int touchId;
    int borderWidth;
    void (*touchAnimate)(Display *display, struct Module_s *module);
    void (*drawAnimate)(Display *display, struct Module_s *module);
    void (*onTouch)(int id, void *arg);
    void (*onUpdate)(Display *display, struct Module_s *module, void *arg);
    void (*draw)(Display *display, struct Module_s *module);
} Module;

void module_animation_switch_page_right(Display *display, Module *module)__fromfile("Librarys/RA8876/Module.c");
void module_animation_switch_page_up(Display *display, Module *module)__fromfile("Librarys/RA8876/Module.c");
void module_animation_switch_page_down(Display *display, Module *module)__fromfile("Librarys/RA8876/Module.c");

void module_init(Module *module, Module *parent)__fromfile("Librarys/RA8876/Module.c"); // Used for static module creation

void module_update_callback(Module *module, void (*onUpdate)(Display *display, Module *module, void *arg))__fromfile("Librarys/RA8876/Module.c");
void module_update_check(Display *display, Module *module, void *arg)__fromfile("Librarys/RA8876/Module.c");

void module_touch_callback(Module *module, void (*onTouch)(int id, void *page), int id)__fromfile("Librarys/RA8876/Module.c");
int module_touch_check(Module *root, Display *display, void *arg)__fromfile("Librarys/RA8876/Module.c");

void module_set_visable(Module *module, bool visible)__fromfile("Librarys/RA8876/Module.c");
void module_copy(Module *to, Module *from)__fromfile("Librarys/RA8876/Module.c");

void module_save(Module *module, Display *display)__fromfile("Librarys/RA8876/Module.c");
void module_paste(Module *module, Display *display)__fromfile("Librarys/RA8876/Module.c");
void module_animate_draw(Module *module, void (*drawAnimate)(Display *display, struct Module_s *module))__fromfile("Librarys/RA8876/Module.c");

void module_set_window(Module *module, void *window)__fromfile("Librarys/RA8876/Module.c");
void module_set_line(Module *module, int w)__fromfile("Librarys/RA8876/Module.c");
void module_set_line_one(Module *module, int x, int y)__fromfile("Librarys/RA8876/Module.c");
void module_set_rectangle(Module *module, int w, int h)__fromfile("Librarys/RA8876/Module.c");
void module_set_rectangle_circle(Module *module, int w, int h)__fromfile("Librarys/RA8876/Module.c");
void module_set_radius(Module *module, int r)__fromfile("Librarys/RA8876/Module.c");
void module_set_image(Module *module, Image *image)__fromfile("Librarys/RA8876/Module.c");

void module_set_text(Module *module, char *text)__fromfile("Librarys/RA8876/Module.c");
void module_set_text2(Module *module, char *text)__fromfile("Librarys/RA8876/Module.c");
void module_text_underline(Module *module)__fromfile("Librarys/RA8876/Module.c");
char *module_text_get(Module *module)__fromfile("Librarys/RA8876/Module.c");
void module_text_set(Module *module, char *text)__fromfile("Librarys/RA8876/Module.c");
void module_text_fit(Module *module)__fromfile("Librarys/RA8876/Module.c");
void module_text_align(Module *module, ModuleTextAignType alignment)__fromfile("Librarys/RA8876/Module.c");
void module_text_align_verticle(Module *module, ModuleTextAignType alignment)__fromfile("Librarys/RA8876/Module.c");

void module_text_font(Module *module, int font)__fromfile("Librarys/RA8876/Module.c");
int module_text_font_width(Module *module)__fromfile("Librarys/RA8876/Module.c");
int module_text_font_height(Module *module)__fromfile("Librarys/RA8876/Module.c");
void module_text_update(Module *module, char *text)__fromfile("Librarys/RA8876/Module.c");

void module_set_padding(Module *module, int px, int py)__fromfile("Librarys/RA8876/Module.c");
void module_set_margin(Module *module, int mx, int my)__fromfile("Librarys/RA8876/Module.c");
void module_set_color(Module *module, int foreground, int background)__fromfile("Librarys/RA8876/Module.c");
void module_add_border(Module *module, int color, int width)__fromfile("Librarys/RA8876/Module.c");
void module_set_position(Module *module, int x, int y)__fromfile("Librarys/RA8876/Module.c");
void module_set_size(Module *module, int w, int h)__fromfile("Librarys/RA8876/Module.c");

void module_align_inner_top(Module *module)__fromfile("Librarys/RA8876/Module.c");
void module_align_inner_left(Module *module)__fromfile("Librarys/RA8876/Module.c");
void module_align_inner_right(Module *module)__fromfile("Librarys/RA8876/Module.c");
void module_align_inner_bottom(Module *module)__fromfile("Librarys/RA8876/Module.c");

void module_align_middle(Module *module)__fromfile("Librarys/RA8876/Module.c");
void module_align_center(Module *module)__fromfile("Librarys/RA8876/Module.c");
void module_align_center_sector(Module *module, int section, int sections)__fromfile("Librarys/RA8876/Module.c");
void module_align_space_even(Module *module, int section, int sections)__fromfile("Librarys/RA8876/Module.c");
void module_align_space_even_verticle(Module *module, int section, int sections)__fromfile("Librarys/RA8876/Module.c");
void module_align_above(Module *module, Module *ref)__fromfile("Librarys/RA8876/Module.c");
void module_align_left(Module *module, Module *ref)__fromfile("Librarys/RA8876/Module.c");
void module_align_below(Module *module, Module *ref)__fromfile("Librarys/RA8876/Module.c");
void module_align_right(Module *module, Module *ref)__fromfile("Librarys/RA8876/Module.c");

void module_fit_width(Module *module)__fromfile("Librarys/RA8876/Module.c");
void module_fit_space_even(Module *module, int sections)__fromfile("Librarys/RA8876/Module.c");
void module_fit_space_even_verticle(Module *module, int sections)__fromfile("Librarys/RA8876/Module.c");
void module_fit_height(Module *module)__fromfile("Librarys/RA8876/Module.c");
void module_fit_below(Module *module, Module *ref)__fromfile("Librarys/RA8876/Module.c");
void module_fit_right(Module *module, Module *ref)__fromfile("Librarys/RA8876/Module.c");

void module_draw(Display *display, Module *module)__fromfile("Librarys/RA8876/Module.c");

void module_destroy_children(Module *module)__fromfile("Librarys/RA8876/Module.c");
void module_trim(Module *module)__fromfile("Librarys/RA8876/Module.c");
void module_text_destroy(ModuleText *text)__fromfile("Librarys/RA8876/Module.c");
void module_destroy(Module *root)__fromfile("Librarys/RA8876/Module.c");
#endif
