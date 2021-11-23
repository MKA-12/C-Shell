#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

// #include <string.h>
char *parse_input()
{
    char *input = NULL;
    ssize_t bufsize = 0;
    getline(&input, &bufsize, stdin);
    return input;
    // char *input;
    // input = readline("");
    // if (strlen(input) != 0)
    // {
    //     add_history(input);
    // }

    // return input;
}