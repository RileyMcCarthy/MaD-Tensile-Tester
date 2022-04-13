#include "Explorer.h"

#define FILES_COLUMNS 6
#define FILE_ROWS_PER_PAGE 2

#define STATIC_BUTTONS 5
#define DYNAMIC_BUTTONS FILES_COLUMNS *FILE_ROWS_PER_PAGE
#define BUTTONCOUNT (STATIC_BUTTONS + DYNAMIC_BUTTONS)

#define BUTTON_CANCEL 0
#define BUTTON_DOTDOT 1
#define BUTTON_PREV 2
#define BUTTON_NEXT 3
#define BUTTON_DONE 4

static void button_cancel(int id, void *arg)
{
    Explorer *explorer = (Explorer *)arg;
    free(explorer->path);
    explorer->path = NULL;
    explorer->complete = true;
}

static void button_dotdot(int id, void *arg)
{
    Explorer *explorer = (Explorer *)arg;
    for (int i = strlen(explorer->path) - 1; i >= 0; i--)
    {
        if (explorer->path[i] == '/')
        {
            explorer->path[i] = '\0';
            explorer->path = realloc(explorer->path, strlen(explorer->path) + 1);
            break;
        }
    }
}

static void button_prev(int id, void *arg)
{
    Explorer *explorer = (Explorer *)arg;
}

static void button_next(int id, void *arg)
{
    Explorer *explorer = (Explorer *)arg;
}

static void button_done(int id, void *arg)
{
    Explorer *explorer = (Explorer *)arg;
    explorer->complete = true;
}

static void button_files(int id, void *arg)
{
    Explorer *explorer = (Explorer *)arg;
    char *newpath = explorer->files[id];
    for (int i = 0; i < strlen(newpath); i++)
    {
        if (newpath[i] == '.') // File type
        {
            if (explorer->mode == EXPLORER_MODE_FILE)
            {
                explorer->complete = true;
            }
            else
            {
                return;
            }
        }
    }
    explorer->path = realloc(explorer->path, strlen(explorer->path) + strlen(newpath) + 2);
    strcat(explorer->path, "/");
    strcat(explorer->path, newpath);
}

Explorer *explorer_create(Display *display, int x, int y, ExplorerMode mode, char *path)
{
    Explorer *explorer = (Explorer *)malloc(sizeof(Explorer));
    explorer->display = display;
    explorer->x = x;
    explorer->y = y;
    explorer->path = (char *)malloc(sizeof(char) * (strlen(path) + 1));
    strcpy(explorer->path, path);
    explorer->files = (char **)malloc(sizeof(char *) * DYNAMIC_BUTTONS);
    explorer->complete = false;
    explorer->mode = mode;
    return explorer;
}

void explorer_destroy(Explorer *explorer)
{
    free(explorer->path);
    free(explorer->buttons);
    for (int i = 0; i < DYNAMIC_BUTTONS; i++)
    {
        free(explorer->files[i]);
    }
    free(explorer->files);
    free(explorer);
}

char *explorer_run(Explorer *explorer)
{
    // Copy current page to memory
    display_bte_memory_copy(explorer->display, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, PAGE3_START_ADDR, SCREEN_WIDTH, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    int buttonWidth = 75;
    int buttonHeight = 32;
    int padding = 10;

    // Create Background
    Module *root = module_create(NULL);

    // Create Window
    Module *window = module_create(root);
    module_set_rectangle_circle(window, EXPLORER_WIDTH, EXPLORER_HEIGHT);
    module_set_position(window, explorer->x, explorer->y);
    module_set_padding(window, padding, padding);
    module_set_color(window, COLOR65K_LIGHTBLUE, COLOR65K_BLUE);

    // Create Title
    Module *title = module_create(window);
    module_set_padding(title, padding, padding);
    module_set_text(title, explorer->path);
    module_set_font(title, -1);
    module_set_color(title, COLOR65K_BLACK, title->parent->foregroundColor);
    module_align_inner_top(title);
    module_align_center(title);
    module_add_underline(title);

    // Create Cancel Button
    Module *cancel = module_create(window);
    module_set_rectangle_circle(cancel, buttonWidth, buttonHeight);
    module_align_center_sector(cancel, 1, 6);
    module_align_inner_bottom(cancel);
    module_set_color(cancel, COLOR65K_RED, cancel->parent->foregroundColor);
    module_set_padding(cancel, padding, padding);
    module_touch_callback(cancel, button_cancel, 0);

    // Create Cancel Text
    Module *cancelText = module_create(cancel);
    module_set_text(cancelText, "Cancel");
    module_set_font(cancelText, RA8876_CHAR_HEIGHT_16);
    module_align_center(cancelText);
    module_align_middle(cancelText);
    module_set_color(cancelText, COLOR65K_BLACK, cancelText->parent->foregroundColor);

    // Create .. Button
    Module *dotdot = module_create(window);
    module_copy(dotdot, cancel);
    module_align_right(dotdot, cancel);
    module_set_color(dotdot, COLOR65K_YELLOW, COLOR65K_YELLOW);
    module_touch_callback(dotdot, button_dotdot, 0);

    // Create .. Text
    Module *dotdotText = module_create(dotdot);
    module_set_text(dotdotText, "..");
    module_set_font(dotdotText, RA8876_CHAR_HEIGHT_16);
    module_align_center(dotdotText);
    module_align_middle(dotdotText);
    module_set_color(dotdotText, COLOR65K_BLACK, dotdotText->parent->foregroundColor);

    // Create left Button
    Module *left = module_create(window);
    module_copy(left, dotdot);
    module_align_right(left, dotdot);
    module_touch_callback(left, button_prev, 0);

    // Create .. Text
    Module *leftText = module_create(left);
    module_set_text(leftText, "<");
    module_set_font(leftText, RA8876_CHAR_HEIGHT_16);
    module_align_center(leftText);
    module_align_middle(leftText);
    module_set_color(leftText, COLOR65K_BLACK, leftText->parent->foregroundColor);

    // Create right Button
    Module *right = module_create(window);
    module_copy(right, left);
    module_align_right(right, left);
    module_touch_callback(right, button_next, 0);

    // Create right Text
    Module *rightText = module_create(right);
    module_set_text(rightText, ">");
    module_set_font(rightText, RA8876_CHAR_HEIGHT_16);
    module_align_center(rightText);
    module_align_middle(rightText);
    module_set_color(rightText, COLOR65K_BLACK, rightText->parent->foregroundColor);

    // Create done Button
    Module *done = module_create(window);
    module_copy(done, right);
    module_align_center_sector(done, 5, 6);
    module_set_color(done, COLOR65K_GREEN, done->parent->foregroundColor);
    module_touch_callback(done, button_done, 0);

    // Create done Text
    Module *doneText = module_create(done);
    module_set_text(doneText, "Done");
    module_set_font(doneText, RA8876_CHAR_HEIGHT_16);
    module_align_center(doneText);
    module_align_middle(doneText);
    module_set_color(doneText, COLOR65K_BLACK, doneText->parent->foregroundColor);

    Module *subroot = module_create(window);
    module_align_below(subroot, title);
    module_fit_below(subroot, title);
    module_fit_width(subroot);
    module_align_inner_left(subroot);
    module_set_color(subroot, subroot->parent->foregroundColor, subroot->parent->backgroundColor);
    module_set_padding(subroot, 0, 0);

    // Create Files
    Module *below = title;
    for (int i = 0; i < FILE_ROWS_PER_PAGE; i++)
    {
        Module *file;
        for (int j = 0; j < FILES_COLUMNS; j++)
        {
            file = module_create(subroot);
            module_set_padding(file, padding, padding);
            module_set_rectangle(file, 100, 100);
            module_set_color(file, COLOR65K_RED, file->parent->foregroundColor);
            module_align_below(file, below);
            module_align_space_even(file, (j + 1), FILES_COLUMNS);
            module_set_visable(file, true);

            Module *text = module_create(file);
            module_set_text(text, "HELLO.txt");
            module_set_font(text, RA8876_CHAR_HEIGHT_16);
            module_set_padding(text, padding, padding);
            module_set_color(text, COLOR65K_BLACK, text->parent->backgroundColor);
            module_align_center(text);
            module_align_below(text, text->parent);
        }
        below = file->child[0]; // Previous file location
    }

    while (!explorer->complete)
    {
        chdir(explorer->path);
        DIR *dir = opendir(explorer->path);
        if (dir == NULL)
        {
            printf("Error: %d\n", _geterror());
            return NULL;
        }

        for (int i = 0; i < explorer->fileCount; i++)
        {
            free(explorer->files[i]);
        }

        struct dirent *dirent;
        for (int i = 0; i < DYNAMIC_BUTTONS; i++)
        {
            Module *file = subroot->child[i];
            if ((dirent = readdir(dir)) != NULL)
            {
                explorer->files[i] = malloc(strlen(dirent->d_name) + 1);
                strcpy(explorer->files[i], dirent->d_name);

                printf("%s\n", dirent->d_name);
                module_set_text(file->child[0], dirent->d_name);
                module_set_font(file->child[0], RA8876_CHAR_HEIGHT_16);
                module_align_center(file->child[0]);
                module_set_visable(file, true);
                module_touch_callback(file, button_files, i);
            }
            else
            {
                module_set_visable(file, false);
                module_touch_callback(file, NULL, 0);
            }
        }
        module_set_text(title, explorer->path);
        module_set_font(title, RA8876_CHAR_HEIGHT_32);
        module_align_center(title);
        module_add_underline(title);
        module_draw(explorer->display, root);
        do
        {
            display_update_touch(explorer->display);
        } while (module_touch_check(root, explorer->display, explorer) == 0);

        printf("is complete:%d", explorer->complete);
    }

    // Paste memory page to screen
    display_bte_memory_copy(explorer->display, PAGE3_START_ADDR, SCREEN_WIDTH, 0, 0, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (explorer->path == NULL)
    {
        return NULL;
    }
    char *finalPath = (char *)malloc(sizeof(char) * (strlen(explorer->path) + 1));
    strcpy(finalPath, explorer->path);
    free(explorer->path);
    module_destroy(root);
    printf("Returning %s\n", finalPath);
    return finalPath;
}