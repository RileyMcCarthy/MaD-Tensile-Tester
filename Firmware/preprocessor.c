#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_LENGTH 256
#define NUM_TOKENS 1

typedef void (*Exec)(char *line, char *needle);

typedef struct Token_s
{
    char macro_name[MAX_LENGTH];
    Exec exec;
} Token;

static void init_token(Token *token, char *macro_name, Exec exec)
{
    strcpy(token->macro_name, macro_name);
    token->exec = exec;
}

static void __using(char *line, char *needle)
{
    printf("__using:\n");

    // Find start/end of structure name
    char *structStart = strchr(needle, '"') + 1;
    char *structEnd = strchr(structStart, '.');
    char *needleEnd = strchr(structEnd, ')') + 1;
    if (structStart == NULL || structEnd == NULL)
    {
        printf("Error: __using: Could not find structure name\n");
        return;
    }

    // Replace __using("structure_name.spin2") with structure_name
    char new_line[MAX_LENGTH];
    strncpy(new_line, line, needle - line);                  // Copy before __using
    strncat(new_line, structStart, structEnd - structStart); // Copy structure name
    strcat(new_line, needleEnd);                             // Copy after __using("")

    // Compile spin2 object to cpp file

    strcpy(line, new_line); // Replace line with new line
}

int main(int argc, char *argv[])
{
    Token tokens[NUM_TOKENS];

    init_token(&tokens[0], "__using", __using);

    if (argc == 2)
    {
        printf("Please provide filename to preprocess and file for output\n");
        return -1;
    }
    printf("Preprocessing %s\n", argv[1]);
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Could not open file\n");
        return -1;
    }

    // Open the output file
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open output file\n");
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        // printf("LINE:%s", line);
        bool found = false;
        for (int i = 0; i < NUM_TOKENS; i++)
        {
            char *needle = NULL;
            if ((needle = strstr(line, tokens[i].macro_name)) != NULL)
            {
                //     printf("Found __using:%s\n", needle);
                tokens[i].exec(line, needle);
            }
        }
        fputs(line, output);
    }
    printf("Done\n");
    fclose(file);
}