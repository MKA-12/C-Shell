#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include "p.h"
char back_processes[1000][1000];
char process_status[1000][1000];
int p_id[1000];
int process_index = 0;
char global_tilda[100];
char display_name[100];
int wait_flag;
char *display_user(char *tilda, char *display_name);
void exit_child(int ignore)
{
    int status;
    pid_t pid_dead_child = waitpid(-1, &status, WNOHANG);
    int id = 0;
    while ((int)pid_dead_child != p_id[id++])
    {
        if (id == process_index)
            return;
    }
    p_id[id - 1] = -2;
    if (status == 0)
    {
        fprintf(stderr, "\n%s exited with pid %d normally\n%s", back_processes[id - 1], pid_dead_child, display_user(global_tilda, display_name));
    }
    else
    {
        fprintf(stderr, "\n%s exited with pid %d abruptly\n%s", back_processes[id - 1], pid_dead_child, display_user(global_tilda, display_name));
    }
    fflush(stdout);
}
char *getProcessStatus(int pid)
{
    char req[100];
    sprintf(req, "%d", pid);
    char path[100] = "/proc/";
    strcat(path, req);
    strcat(path, "/status");
    FILE *status_file = fopen(path, "r");
    if (status_file == NULL)
    {
        return NULL;
    }
    char *buf = (char *)malloc(10000 * sizeof(char));
    char *process_status = (char *)malloc(10000 * sizeof(char));
    while (fgets(buf, 10000, status_file))
    {
        if (strstr(buf, "State:"))
        {
            char *temp = (char *)malloc(10000);
            strcpy(temp, buf);
            process_status = __strtok_r(temp, "\t", &temp);
            process_status = __strtok_r(temp, " ", &temp);
            process_status = __strtok_r(temp, "(", &temp);
        }
    }
    process_status[strlen(process_status) - 2] = '\0';
    // printf("%s\n", process_status);
    return process_status;
}
void background(char **arguments, int NO_ARGS)
{
    arguments[NO_ARGS - 1] = NULL;
    int process_id = fork();
    if (process_id == 0)
    {
        int er = execvp(arguments[0], arguments);
        if (er == -1)
        {
            // printf("Command Not Found\n");
            perror("Command not found:");
        }
    }
    else
    {
        p_id[process_index] = process_id;
        strcpy(back_processes[process_index++], arguments[0]);
        setpgid(process_id, process_id);
    }
    signal(SIGCHLD, exit_child);
}
void jobs()
{
    int j = 1;
    for (int i = 0; i < process_index; i++)
    {
        if (p_id[i] > 0 && strcmp(getProcessStatus(p_id[i]), "zombie") != 0)
        {
            printf("[%d] %s %s [%d]\n", j++, getProcessStatus(p_id[i]), back_processes[i], p_id[i]);
        }
        // printf("%d\n", p_id[i]);
    }
}
void kjob(char **arguments, int NO_ARGS)
{
    int j = 0;
    char *id = (char *)malloc(100 * sizeof(char));
    for (int i = 0; i < process_index; i++)
    {
        if (p_id[i] > 0)
        {
            j++;
            sprintf(id, "%d", j);
            if (strcmp(arguments[1], id) == 0)
            {
                int sig_no = atoi(arguments[2]);
                if (sig_no > 0)
                {
                    kill(p_id[i], sig_no);
                }
            }
        }
    }
}
void overkill()
{
    for (int i = 0; i < process_index; i++)
    {
        if (p_id[i] > 0)
        {
            kill(((pid_t)p_id[i]), SIGKILL);
        }
    }
}
void bg(char **arguments, int NO_ARGS)
{
    if (NO_ARGS > 1)
    {
        int j = 0;
        char *id = (char *)malloc(100 * sizeof(char));
        for (int i = 0; i < process_index; i++)
        {
            if (p_id[i] > 0)
            {
                j++;
                sprintf(id, "%d", j);
            }
            if (strcmp(arguments[1], id) == 0)
            {
                kill(p_id[i], SIGCONT);
            }
        }
    }
}
void fg(char **arguments, int NO_ARGS)
{
    if (NO_ARGS > 1)
    {
        int j = 0;
        char *id = (char *)malloc(100 * sizeof(char));
        for (int i = 0; i < process_index; i++)
        {
            if (p_id[i] > 0)
            {
                j++;
                sprintf(id, "%d", j);
            }
            if (strcmp(arguments[1], id) == 0)
            {
                setpgid(p_id[i], getpgid(0));
                // signal(SIGTTIN, SIG_IGN);
                // signal(SIGTTOU, SIG_IGN);
                // tcsetpgrp(0, p_id[i]);

                kill(p_id[i], SIGCONT);
                foreground_pid = p_id[i];
                foreground_process = back_processes[i];
                int status;
                waitpid((pid_t)p_id[i], &status, WUNTRACED);
                // printf("%d %s\n", foreground_pid, foreground_process);

                // tcsetpgrp(0, getpgid(shell_pid));
                // signal(SIGTTOU, SIG_DFL);
                // signal(SIGTTIN, SIG_DFL);
                setpgid(p_id[i], p_id[i]);

                p_id[i] = -2;
            }
        }
    }
}
void fg1(char **arguments, int NO_ARGS)
{
    if (NO_ARGS > 1)
    {
        int j = 0;
        char *id = (char *)malloc(100 * sizeof(char));
        for (int i = 0; i < process_index; i++)
        {
            if (p_id[i] > 0)
            {
                j++;
                sprintf(id, "%d", j);
            }
            if (strcmp(arguments[1], id) == 0)
            {
            }
        }
    }
}
void handle_sigint(int ignore)
{
    if (foreground_pid > 0)
    {
        kill(foreground_pid, SIGINT);
    }
}
void handle_ctrlz(int sig_no)
{
    // printf("bayata %d\n", process_index);

    if (foreground_pid > 0)
    {
        // printf("lopala %d\n", process_index);

        kill(foreground_pid, SIGTSTP);
        // printf("after kill %d\n", process_index);

        p_id[process_index] = foreground_pid;
        strcpy(back_processes[process_index++], foreground_process);
        // printf("%d\n", process_index);
        // printf("%d %s\n", p_id[process_index - 1], back_processes[process_index - 1]);
        int status;
        // waitpid((pid_t)foreground_pid, &status, WNOHANG);
    }
    // signal(SIGTSTP,handle_ctrlz);
}