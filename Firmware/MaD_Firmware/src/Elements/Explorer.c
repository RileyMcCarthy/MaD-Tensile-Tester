#include "Explorer.h"

#define FILES_COLUMNS 6
#define FILE_ROWS_PER_PAGE 2

#define STATIC_BUTTONS 5
#define BUTTONCOUNT (STATIC_BUTTONS + MAX_FILES)

#define BUTTON_CANCEL 0
#define BUTTON_DOTDOT 1
#define BUTTON_PREV 2
#define BUTTON_NEXT 3
#define BUTTON_DONE 4

static char cancelBuffer[] = "Cancel";
static char dotdotBuffer[] = "..";
static char prevBuffer[] = "<";
static char nextBuffer[] = ">";
static char doneBuffer[] = "Done";
static char emptyBuffer[] = "";

static Explorer staticExplorer;
extern Images images;

static bool isFile(char *name)
{
    for (int i = 0; i < strlen(name); i++)
    {
        if (name[i] == '.')
        {
            return true;
        }
    }
    return false;
}

static void button_delete(int id, void *arg)
{
    Explorer *explorer = (Explorer *)arg;
    int pathLength = strlen(explorer->pathBuffer);
    strcat(explorer->pathBuffer, "/");
    strcat(explorer->pathBuffer, explorer->files[explorer->selection]);
    printf("Removing %s from %s\n", explorer->files[explorer->selection], explorer->pathBuffer);
    remove(explorer->pathBuffer);
    explorer->pathBuffer[pathLength] = '\0'; // Remove the file name from the path buffer
}

static void button_cancel(int id, void *arg)
{
    Explorer *explorer = (Explorer *)arg;
    strcpy(explorer->pathBuffer, "");
    explorer->complete = true;
}

static void button_dotdot(int id, void *arg)
{
    Explorer *explorer = (Explorer *)arg;
    for (int i = strlen(explorer->pathBuffer) - 1; i >= 0; i--)
    {
        if (explorer->pathBuffer[i] == '/')
        {
            explorer->pathBuffer[i] = '\0';
            explorer->selection = -1;
            explorer->page = 0;
            break;
        }
    }
}

static void button_prev(int id, void *arg)
{
    Explorer *explorer = (Explorer *)arg;
    if (explorer->page > 0)
        explorer->page--;
    explorer->selection = -1;
}

static void button_next(int id, void *arg)
{
    Explorer *explorer = (Explorer *)arg;
    explorer->page++;
    explorer->selection = -1;
}

static void button_done(int id, void *arg)
{
    Explorer *explorer = (Explorer *)arg;
    explorer->complete = true;
}

static void button_files(int id, void *arg)
{
    Explorer *explorer = (Explorer *)arg;
    const char *newpath = explorer->files[id];
    for (int i = 0; i < (int)strlen(newpath); i++)
    {
        if (newpath[i] == '.') // File type
        {
            if (explorer->mode == EXPLORER_MODE_FILE)
            {
                explorer->selection = id;
                return;
            }
            else
            {
                return;
            }
        }
    }
    strcat(explorer->pathBuffer, "/");
    strcat(explorer->pathBuffer, newpath);
    explorer->selection = -1;
    explorer->page = 0;
    printf("%s\n", explorer->pathBuffer);
}

// return static explorer
Explorer *explorer_create(Display *display, int x, int y, ExplorerMode mode, const char *path)
{
    staticExplorer.display = display;
    staticExplorer.x = x;
    staticExplorer.y = y;
    strcpy(staticExplorer.pathBuffer, path);
    staticExplorer.complete = false;
    staticExplorer.mode = mode;
    staticExplorer.page = 0;
    staticExplorer.selection = -1;
    return &staticExplorer;
}

void explorer_destroy(Explorer *explorer)
{
    // free(explorer);
}

char *explorer_run(Explorer *explorer)
{
    // Copy current page to memory
    display_bte_memory_copy(explorer->display, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, PAGE3_START_ADDR, SCREEN_WIDTH, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    int buttonWidth = 75;
    int buttonHeight = 32;
    int padding = 10;
    explorer->complete = false;
    // Create Background
    Module *root = &(explorer->root);
    module_init(root, NULL);

    // Create Window
    Module *window = &(explorer->window);
    module_init(window, root);
    module_set_rectangle_circle(window, EXPLORER_WIDTH, EXPLORER_HEIGHT);
    module_set_position(window, explorer->x, explorer->y);
    module_set_padding(window, 30, 10);
    module_set_color(window, SECONDARYCOLOR, SECONDARYCOLOR);
    module_add_border(window, COLOR65K_BLACK, 1);
    module_set_radius(window, 20);

    // Create Title
    Module *title = &(explorer->title);
    module_init(title, window);
    module_set_margin(title, padding, padding);
    module_set_text(title, explorer->pathBuffer);
    module_text_font(title, RA8876_CHAR_HEIGHT_32);
    module_text_fit(title);
    module_fit_width(title);
    module_set_color(title, SECONDARYTEXTCOLOR, title->parent->foregroundColor);
    module_align_inner_top(title);
    module_align_center(title);
    module_text_align(title, MODULE_TEXT_ALIGN_INNER_CENTER);
    module_text_underline(title);

    // Create Delete Button
    Module *deleteButton = &(explorer->deleteButton);
    module_init(deleteButton, window);
    module_set_margin(deleteButton, padding, padding);
    module_set_image(deleteButton, &(images.garbageImage));
    // module_set_rectangle(deleteButton, 25, 25);
    module_align_inner_top(deleteButton);
    module_align_inner_left(deleteButton);
    module_set_color(deleteButton, COLOR65K_RED, COLOR65K_RED);
    module_touch_callback(deleteButton, button_delete, 0);

    // Create Cancel Button
    Module *cancel = &(explorer->cancel);
    module_init(cancel, window);
    module_set_rectangle_circle(cancel, buttonWidth, buttonHeight);
    module_align_center_sector(cancel, 1, 6);
    module_align_inner_bottom(cancel);
    module_set_color(cancel, COLOR65K_RED, cancel->parent->foregroundColor);
    module_set_margin(cancel, padding, padding);
    module_touch_callback(cancel, button_cancel, 0);

    // Create Cancel Text
    Module *cancelText = &(explorer->cancelText);
    module_init(cancelText, cancel);
    module_set_text(cancelText, cancelBuffer);
    module_text_font(cancelText, RA8876_CHAR_HEIGHT_16);
    module_text_fit(cancelText);
    module_align_center(cancelText);
    module_align_middle(cancelText);
    module_set_color(cancelText, SECONDARYTEXTCOLOR, cancelText->parent->foregroundColor);

    // Create .. Button
    Module *dotdot = &(explorer->dotdot);
    module_init(dotdot, window);
    module_copy(dotdot, cancel);
    module_align_right(dotdot, cancel);
    module_set_color(dotdot, COLOR65K_YELLOW, COLOR65K_YELLOW);
    module_touch_callback(dotdot, button_dotdot, 0);

    // Create .. Text
    Module *dotdotText = &(explorer->dotdotText);
    module_init(dotdotText, dotdot);
    module_set_text(dotdotText, dotdotBuffer);
    module_text_font(dotdotText, RA8876_CHAR_HEIGHT_16);
    module_text_fit(dotdotText);
    module_align_center(dotdotText);
    module_align_middle(dotdotText);
    module_set_color(dotdotText, COLOR65K_BLACK, dotdotText->parent->foregroundColor);

    // Create left Button
    Module *left = &(explorer->left);
    module_init(left, window);
    module_copy(left, dotdot);
    module_align_right(left, dotdot);
    module_touch_callback(left, button_prev, 0);

    // Create .. Text
    Module *leftText = &(explorer->leftText);
    module_init(leftText, left);
    module_set_text(leftText, prevBuffer);
    module_text_font(leftText, RA8876_CHAR_HEIGHT_16);
    module_text_fit(leftText);
    module_align_center(leftText);
    module_align_middle(leftText);
    module_set_color(leftText, COLOR65K_BLACK, leftText->parent->foregroundColor);

    // Create right Button
    Module *right = &(explorer->right);
    module_init(right, window);
    module_copy(right, left);
    module_align_right(right, left);
    module_touch_callback(right, button_next, 0);

    // Create right Text
    Module *rightText = &(explorer->rightText);
    module_init(rightText, right);
    module_set_text(rightText, nextBuffer);
    module_text_font(rightText, RA8876_CHAR_HEIGHT_16);
    module_text_fit(rightText);
    module_align_center(rightText);
    module_align_middle(rightText);
    module_set_color(rightText, COLOR65K_BLACK, rightText->parent->foregroundColor);

    // Create done Button
    Module *done = &(explorer->done);
    module_init(done, window);
    module_copy(done, right);
    module_align_center_sector(done, 5, 6);
    module_set_color(done, COLOR65K_GREEN, done->parent->foregroundColor);
    module_touch_callback(done, button_done, 0);

    // Create done Text
    Module *doneText = &(explorer->doneText);
    module_init(doneText, done);
    module_set_text(doneText, doneBuffer);
    module_text_font(doneText, RA8876_CHAR_HEIGHT_16);
    module_text_fit(doneText);
    module_align_center(doneText);
    module_align_middle(doneText);
    module_set_color(doneText, COLOR65K_BLACK, doneText->parent->foregroundColor);

    Module *subroot = &(explorer->subroot);
    module_init(subroot, window);
    module_align_below(subroot, title);
    module_fit_below(subroot, title);
    module_fit_width(subroot);
    module_align_inner_left(subroot);
    module_set_color(subroot, subroot->parent->foregroundColor, subroot->parent->backgroundColor);

    // Create Files
    Module *below = title;
    int index = 0;
    for (int i = 0; i < FILE_ROWS_PER_PAGE; i++)
    {
        Module *file;
        for (int j = 0; j < FILES_COLUMNS; j++)
        {
            file = &(explorer->fileModule[index]);
            module_init(file, subroot);
            module_set_margin(file, padding, padding);
            module_set_rectangle(file, 100, 100);
            module_set_color(file, COLOR65K_RED, file->parent->foregroundColor);
            module_align_below(file, below);
            module_align_space_even(file, (j + 1), FILES_COLUMNS);
            module_set_visable(file, false);

            Module *text = &(explorer->fileText[index]);
            module_init(text, file);
            module_set_text(text, explorer->files[index]);
            module_text_font(text, RA8876_CHAR_HEIGHT_16);
            module_text_fit(text);
            module_set_margin(text, padding, padding);
            module_set_color(text, COLOR65K_BLACK, text->parent->backgroundColor);
            module_fit_width(text);
            module_align_center(text);
            module_text_align(text, MODULE_TEXT_ALIGN_INNER_CENTER);
            module_align_below(text, text->parent);
            index++;
        }
        below = file->child[0]; // Previous file location
    }

    while (!explorer->complete)
    {
        chdir(explorer->pathBuffer);
        DIR *dir = opendir(explorer->pathBuffer);
        if (dir == NULL)
        {
            printf("Error: %d\n", _geterror());
            return NULL;
        }

        for (int i = 0; i < explorer->page * MAX_FILES; i++)
        {
            if (readdir(dir) == NULL)
            {
                break;
            }
        }

        for (int i = 0; i < MAX_FILES; i++)
        {
            Module *file = subroot->child[i];
            struct dirent *dirent;
            if ((dirent = readdir(dir)) != NULL)
            {
                strcpy(explorer->files[i], dirent->d_name);

                printf("%s\n", explorer->files[i]);
                // module_text_update(file->child[0], explorer->files[i]);
                if (isFile(dirent->d_name))
                {
                    module_set_image(file, &(images.fileImage));
                }
                else
                {
                    module_set_image(file, &(images.folderImage));
                }
                module_set_size(file, 100, 100);
                module_set_visable(file, true);
                if (explorer->selection == i)
                {
                    printf("selection: %d\n", explorer->selection);
                    module_set_color(file->child[0], COLOR65K_GREEN, SECONDARYCOLOR);
                }
                else
                {
                    module_set_color(file->child[0], SECONDARYTEXTCOLOR, SECONDARYCOLOR);
                }
                module_touch_callback(file, button_files, i);
            }
            else
            {
                module_set_visable(file, false);
                module_touch_callback(file, NULL, 0);
            }
        }

        module_draw(explorer->display, root);
        printf("done drawing\n");
        do
        {
            display_update_touch(explorer->display);
        } while (module_touch_check(root, explorer->display, explorer) == 0);
    }

    // Paste memory page to screen
    display_bte_memory_copy(explorer->display, PAGE3_START_ADDR, SCREEN_WIDTH, 0, 0, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (explorer->mode == EXPLORER_MODE_FILE)
    {
        if (explorer->selection == -1)
        {
            strcpy(explorer->pathBuffer, "");
            return explorer->pathBuffer;
        }
        strcat(explorer->pathBuffer, "/");
        strcat(explorer->pathBuffer, explorer->files[explorer->selection]);
    }
    printf("Path:%s\n", explorer->pathBuffer);
    return explorer->pathBuffer;
}