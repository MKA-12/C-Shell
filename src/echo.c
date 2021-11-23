#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void echo(char **arguments, int NO_ARGS)
{
    for (int i = 1; i < NO_ARGS; i++)
    {
        if (strstr(arguments[i], "$") != NULL)
        {
            printf("%s ", getenv(&arguments[i][1]));
        }
        else
        {
            printf("%s ", arguments[i]);
        }
    }
    printf("\n");
}