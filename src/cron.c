#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
void cronjob(char **arguments, int NO_ARGS)
{
    if (NO_ARGS >= 7)
    {
        int c = 0;
        int t = 0;
        int p = 0;
        for (int i = 0; i < NO_ARGS; i++)
        {
            if (strcmp(arguments[i], "-c") == 0)
            {
                c = 1;
            }
            else if (strcmp(arguments[i], "-t") == 0)
            {
                t = 1;
            }
            else if (strcmp(arguments[i], "-p") == 0)
            {
                p = 1;
            }
        }
        if (t == 1 && p == 1 && c == 1)
        {
            char **command = (char **)malloc(sizeof(char) * (NO_ARGS + 5));
            for (int i = 2; i < (NO_ARGS - 4); i++)
            {
                command[i - 2] = (char *)malloc(100);
                strcpy(command[i - 2], arguments[i]);
            }
            pid_t process_id_cron;
            command[NO_ARGS - 6] = NULL;
            int end_time = atoi(arguments[NO_ARGS - 1]);
            int freq = atoi(arguments[NO_ARGS - 3]);
            process_id_cron = fork();
            if (process_id_cron == 0)
            {
                for (int i = 0; i < end_time / freq; i++)
                {
                    sleep(freq);
                    int child = fork();
                    if (child == 0)
                    {
                        if (execvp(command[0], command) < 0)
                        {
                            fprintf(stderr, "command not found\n");
                        }
                        exit(1);
                    }
                }
            }
        }
        else
        {
            fprintf(stderr, "give proper arguments for cronjob\n");
        }
    }
    else
    {
        fprintf(stderr, "too few arguments for cronjob\n");
    }
}