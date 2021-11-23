#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
void cd(char **arguments, char *tilda, int NO_ARGS)
{
    char *dir_name = (char *)malloc(1000 * sizeof(char));
    if (arguments[1][0] == '~')
    {
        arguments[1] = arguments[1] + 1;
        strcpy(dir_name, tilda);
        strcat(dir_name, arguments[1]);
    }
    else
    {
        if (NO_ARGS == 1)
        {
            strcpy(dir_name, tilda);
        }
        else
        {
            strcpy(dir_name, arguments[1]);
        }
    }
    if (chdir(dir_name) != 0)
        perror("cd");
}