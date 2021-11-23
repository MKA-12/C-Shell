#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "p.h"
#include <string.h>
void foreground(char **arguments, int NO_ARGS, int wait_flag)
{
    if (wait_flag == 0)
    {
        arguments[NO_ARGS] = NULL;
        int process_id = fork();
        if (process_id > 0)
        {
            // foreground_pid = process_id;
            // foreground_process = (char *)malloc(100 * sizeof(char));
            // strcpy(foreground_process, arguments[0]);
            // for (int i = 1; i < NO_ARGS; i++)
            // {
            //     strcat(foreground_process, " ");
            //     strcat(foreground_process, arguments[i]);
            // }
            // setpgid(foreground_pid, foreground_pid);
            wait(NULL);
            // int *status;
            // waitpid(foreground_pid, status, WUNTRACED);
        }
        if (process_id == 0)
        {
            int er = execvp(arguments[0], arguments);
            if (er == -1)
            {
                // printf("Command Not Found\n");
                perror("Command not found:");
                exit(0);
            }
        }
    }
}