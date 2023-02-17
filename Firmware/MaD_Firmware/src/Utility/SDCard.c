#include "SDCard.h"
#include "StaticQueue.h"
#include <propeller2.h>
#include <stdio.h>
#include <stdlib.h>

#define length(x) (sizeof(x) / sizeof(x[0]))

#define SDCARD_MEMORY_SIZE 30000
static long sdcard_stack[SDCARD_MEMORY_SIZE];

#define MAX_SIZE_READ 10
#define MAX_SIZE_WRITE 10

#define MAX_FILENAME 255
#define MAX_SIZE_DATA 255

static char write_data_buffer[MAX_SIZE_DATA];
static void *write_data = NULL;
static void *read_data = NULL; 
static int read_index = -1;
static int read_size = 0;
static int write_size = 0;

static FILE *file = NULL;
static char filename[MAX_FILENAME];
static char mode[5];
static bool open = false;
static bool close = false;
static int _lockid = -1;
static bool isEOF = false;

bool sdcard_open(const char *name, const char *m)
{
    if (!_locktry(_lockid))
    {
        printf("Error: Could not get lock\n");
        return false;
    }
    if (file != NULL)
    {
        printf("Error: file already open\n");
        return false;
    }

    strncpy(filename, name, MAX_FILENAME);
    strncpy(mode, m, 5);
    open = true;
    while(!sdcard_is_open())
    {
        if (!open)
        {
            printf("Error: failed to open file\n");
            return false;
        }
    }
    return true;
    
}

bool sdcard_close()
{
    if (file != NULL)
    {
        close = true;
        while(sdcard_is_open())
        {
            printf("Waiting for sd card to close\n");
        }
        _lockrel(_lockid);
        return true;
    }
    return false;
}

bool sdcard_read(const void *mem, int size)
{
    read_size = size;
    read_data = (void*)mem;
    while (read_data != NULL)
    {
        if (!sdcard_is_open())
        {
            printf("Error: file is not open\n");
            return false;
        }
    }
    return !isEOF;
}

bool sdcard_read_index(const void *mem, int size, int index)
{
    read_index = index*size; // Treat index like file is array of size
    read_size = size;
    read_data = (void*)mem;
    while (read_data != NULL)
    {
        if (!sdcard_is_open())
        {
            printf("Error: file is not open\n");
            return false;
        }
        //printf("waiting for read sd card data\n");
    }
    return !isEOF;
    
}

bool sdcard_write(const void *mem, int size)
{
    while (write_data != NULL)
    {
        //printf("Error: write data already in queue\n");
        return false;
    }
    if (size > MAX_SIZE_DATA)
    {
        printf("Error: write size too large\n");
        return false;
    }
    //printf("Writing %d to sd card\n", size);
    memcpy(write_data_buffer, (char*)mem, size);
    write_size = size;
    write_data = &write_data_buffer;
}


bool sdcard_is_open()
{
    return file != NULL;
}

bool sdcard_is_busy()
{
    return write_data != NULL || read_data != NULL;
}

static void sdcard_cog(void *arg)
{
    _lockid = _locknew();
    while (!_locktry)
        printf("waiting for lock\n");
    arg;
    mount("/sd", _vfs_open_sdcard());

    strncpy(filename, "", MAX_FILENAME);
    strncpy(mode, "", 5);

    file = NULL;
    isEOF = false;

    _lockrel(_lockid);
    while(true)
    {
        if (open && !sdcard_is_open())
        {
            char path[255];
            strcpy(path, "/sd/");
            strcat(path, filename);
            //printf("Opening file %s with mode %s\n", path, mode);
            file = fopen(path, mode);
            if (file == NULL)
            {
                printf("Failed to open file %s with mode %s\n", path, mode);
                open = false;
            }
            else
            {
                //printf("opened file\n");
                open = false;
            }
        }

        if (close && sdcard_is_open())
        {
            close = false;
            fclose(file);
            file = NULL;
            printf("Closing file\n");
        }

        if (sdcard_is_open())
        {
            if (write_data != NULL)
            {
                //printf("Writing %d to file\n", write_size);
                fwrite(write_data, write_size, 1, file);
                write_data = NULL;
                write_size = 0;
            }

            if (read_data != NULL)
            {
                //printf("Reading from file\n");
                off_t file_position = ftell(file);
                fseek(file, 0, SEEK_END);
                off_t file_length = ftell(file);
                if (read_index != -1)
                {
                    file_position = read_index;
                    read_index = -1;
                }
                if (file_position + read_size > file_length)
                {
                    read_size = file_length - file_position;
                    isEOF = true;
                    printf("End of file calculated\n");
                }
                fseek(file, file_position, SEEK_SET);
                printf("Reading %d bytes from file at position %d/%d\n", read_size, file_position, file_length);
                if (read_size > 0)
                {
                    fread(read_data, read_size, 1, file);
                }
                read_data = NULL;
                read_size = 0;
            }
        }
    }
}

bool sdcard_begin()
{
    int id = _cogstart_C(sdcard_cog, NULL, &sdcard_stack[0], sizeof(long) * SDCARD_MEMORY_SIZE);
    while (_lockid == -1)
    {
        printf("Waiting for sd card cog to start\n");
        _waitms(10);
    }
    return id != -1;
}