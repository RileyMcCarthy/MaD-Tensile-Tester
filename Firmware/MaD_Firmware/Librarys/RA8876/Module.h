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

typedef struct Module_s Module;
typedef void (*ModuleOnTouch)(int id, void *arg);
typedef void (*ModuleTouchAnimate)(Display *display, Module *module);
typedef void (*ModuleDrawAnimate)(Display *display, Module *module);
typedef void (*ModuleReDraw)(Display *display, Module *module, void *arg);
typedef void (*ModuleDraw)(Display *display, Module *module);

struct Module_s
{
    Module *parent;
    Module *child[MODULE_MAX_CHILDREN];
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
    ModuleTouchAnimate touchAnimate; // Animation when the module is touched
    ModuleDrawAnimate drawAnimate; // Animation when the module is drawn
    ModuleOnTouch onTouch; // Callback to execute when the module is touched
    ModuleReDraw reDraw; // Callback to execute when the module is redrawn
    ModuleDraw draw; // Callback to execute when the module is drawn
};

void module_animation_switch_page_right(Display *display, Module *module);
void module_animation_switch_page_up(Display *display, Module *module);
void module_animation_switch_page_down(Display *display, Module *module);

void module_init(Module *module, Module *parent); // Used for static module creation

void module_redraw_callback(Module *module, ModuleReDraw reDraw);
void module_redraw(Display *display, Module *module, void *arg);

void module_touch_callback(Module *module, ModuleOnTouch onTouch, int id);
int module_touch_check(Module *root, Display *display, void *arg);

void module_set_visable(Module *module, bool visible);
void module_copy(Module *to, Module *from);

void module_save(Module *module, Display *display);
void module_paste(Module *module, Display *display);
void module_animate_draw(Module *module, ModuleDrawAnimate drawAnimate);

void module_set_window(Module *module, void *window);
void module_set_line(Module *module, int w);
void module_set_line_one(Module *module, int x, int y);
void module_set_rectangle(Module *module, int w, int h);
void module_set_rectangle_circle(Module *module, int w, int h);
void module_set_radius(Module *module, int r);
void module_set_image(Module *module, Image *image);

void module_set_text(Module *module, char *text);
void module_set_text2(Module *module, char *text);
void module_text_underline(Module *module);
char *module_text_get(Module *module);
void module_text_set(Module *module, char *text);
void module_text_fit(Module *module);
void module_text_align(Module *module, ModuleTextAignType alignment);
void module_text_align_verticle(Module *module, ModuleTextAignType alignment);

void module_text_font(Module *module, int font);
int module_text_font_width(Module *module);
int module_text_font_height(Module *module);
void module_text_update(Module *module, char *text);

void module_set_padding(Module *module, int px, int py);
void module_set_margin(Module *module, int mx, int my);
void module_set_color(Module *module, int foreground, int background);
void module_add_border(Module *module, int color, int width);
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
void module_fit_right(Module *module, Module *ref);

void module_draw(Display *display, Module *module);

void module_destroy_children(Module *module);
void module_trim(Module *module);
void module_text_destroy(ModuleText *text);
void module_destroy(Module *root);
#endif
