#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "p.h"
int wait_flag = 0;
int NO_COMMAND = 0;
int NO_ARGS = 0;
char global_tilda[100];
char display_name[100];
char *display_user(char *tilda, char *display_name);
char *pwd();
char *parse_input();
void cd(char **arguments, char *tilda, int NO_ARGS);
void echo(char **arguments, int NO_ARGS);
void ls(char **arguments, char *tilda, int NO_ARGS);
void pinfo(char **arguments, int NO_ARGS);
void foreground(char **arguments, int NO_ARGS, int wait_flag);
void exit_child(int ignore, char *global_tilda, char *display_name);
void background(char **arguments, int NO_ARGS);
void jobs();
void bg(char **arguments, int NO_ARGS);
void fg(char **arguments, int NO_ARGS);
void overkill();
// int foreground_pid;
void cronjob(char **arguments, int NO_ARGS);
void kjob(char **arguments, int NO_ARGS);
void handle_sigint(int ignore);
void handle_ctrlz(int ignore);
char **store_command(char *input)
{
    char *token = (char *)malloc(1000);
    char *input_tokenize = input;
    NO_ARGS = 0;
    char **arguments = (char **)malloc(1000);
    for (int i = 0; i < 1000; i++)
    {
        arguments[i] = (char *)malloc(1000);
    }
    while (token = __strtok_r(input_tokenize, " ", &input_tokenize))
    {
        char *token_next = (char *)calloc(1000, sizeof(char));
        char *token_copy = token;
        while (token_next = __strtok_r(token_copy, "\t", &token_copy))
        {
            if (strcmp(token_next, "\n") != 0)
            {
                if (token_next[strlen(token_next) - 1] == '\n')
                {
                    token_next[strlen(token_next) - 1] = '\0';
                }
                strcpy(arguments[NO_ARGS], token_next);
                NO_ARGS++;
            }
        }
    }
    return arguments;
}
char **store_input(char *input)
{
    char *token = (char *)malloc(1000);
    char *input_tokenize = input;
    char **arguments = (char **)malloc(1000);
    for (int i = 0; i < 1000; i++)
    {
        arguments[i] = (char *)malloc(1000);
    }
    while (token = __strtok_r(input_tokenize, ";", &input_tokenize))
    {
        if (strcmp(token, "\n") != 0)
        {
            strcpy(arguments[NO_COMMAND], token);
            NO_COMMAND++;
        }
    }
    return arguments;
}
void unsetenvvar(char **arguments, int NO_ARGS)
{
    if (NO_ARGS == 2)
    {
        unsetenv(arguments[1]);
    }
    else
    {
        printf("nope, here's your error\n");
    }
}
void setenvvar(char **arguments, int NO_ARGS)
{
    if (NO_ARGS > 3)
    {
        printf("nope, here's your error\n");
    }
    else if (NO_ARGS == 3)
    {
        setenv(arguments[1], arguments[2], 1);
    }
    else if (NO_ARGS == 2)
    {
        setenv(arguments[1], "", 1);
    }
}
void execute_commands(char **arguments, char *tilda, int NO_ARGS, int pipe_arr[])
{

    if (strcmp(arguments[0], "quit") == 0)
    {
        overkill();
        exit(0);
    }
    else if (strcmp(arguments[0], "cd") == 0) // cd
    {
        cd(arguments, tilda, NO_ARGS);
    }
    else if (strcmp(arguments[NO_ARGS - 1], "&") == 0)
    {
        wait_flag = 1;
        background(arguments, NO_ARGS);
    }
    else if (strcmp(arguments[0], "setenv") == 0)
    {
        setenvvar(arguments, NO_ARGS);
    }
    else if (strcmp(arguments[0], "unsetenv") == 0)
    {
        unsetenvvar(arguments, NO_ARGS);
    }
    else if (strcmp(arguments[0], "fg") == 0)
    {
        fg(arguments, NO_ARGS);
    }
    else if (strcmp(arguments[0], "vim") == 0)
    {
    }
    else if (strcmp(arguments[0], "cronjob") == 0)
    {
        cronjob(arguments, NO_ARGS);
    }
    else
    {
        int process_id = fork();
        if (process_id == 0)
        {
            int p = getpid();
            setpgid(p, p);
            dup2(pipe_arr[1], 1);
            if (strcmp(arguments[0], "pwd") == 0) //pwd
            {
                char curr_wd[1000];
                strcpy(curr_wd, pwd());
                if (curr_wd != NULL)
                {
                    printf("%s\n", curr_wd);
                }
            }
            else if (strcmp(arguments[0], "echo") == 0) // echo
            {
                echo(arguments, NO_ARGS);
            }
            else if (strcmp(arguments[0], "ls") == 0) // ls
            {
                ls(arguments, tilda, NO_ARGS);
            }
            else if (strcmp(arguments[0], "pinfo") == 0)
            {
                pinfo(arguments, NO_ARGS);
            }
            else if (strcmp(arguments[0], "jobs") == 0)
            {
                jobs();
            }
            else if (strcmp(arguments[0], "kjob") == 0)
            {
                kjob(arguments, NO_ARGS);
            }
            else if (strcmp(arguments[0], "overkill") == 0)
            {
                overkill();
            }
            else if (strcmp(arguments[0], "bg") == 0)
            {
                bg(arguments, NO_ARGS);
            }
            else
            {
                arguments[NO_ARGS] = '\0';
                int er = execvp(arguments[0], arguments);
                if (er == -1)
                {
                    // printf("Command Not Found\n");
                    perror("Command not found:");
                }
            }
            exit(0);
        }
        else
        {
            foreground_pid = process_id;
            foreground_process = (char *)malloc(100 * sizeof(char));
            strcpy(foreground_process, arguments[0]);
            for (int i = 1; i < NO_ARGS; i++)
            {
                strcat(foreground_process, " ");
                strcat(foreground_process, arguments[i]);
            }
            setpgid(process_id, process_id); //..
            // signal(SIGTTIN, SIG_IGN);
            // signal(SIGTTOU, SIG_IGN);
            // tcsetpgrp(0, process_id);

            int *status;
            waitpid(process_id, status, WUNTRACED);
            // wait(NULL);
            // tcsetpgrp(0, getpgid(shell_pid));
            // signal(SIGTTOU, SIG_DFL);
            // signal(SIGTTIN, SIG_DFL);
        }
    }
}
int main()
{
    signal(SIGINT, handle_sigint);
    signal(SIGTSTP, handle_ctrlz);
    shell_pid = getpid();
    int std_out = dup(1);
    int std_in = dup(0);
    char *path_to_exe = (char *)malloc(1000 * sizeof(char));
    strcpy(path_to_exe, "/proc");
    strcat(path_to_exe, "/");
    char *id_for_curr = (char *)malloc(1000 * sizeof(char));
    sprintf(id_for_curr, "%d", getpid());
    strcat(path_to_exe, id_for_curr);
    strcat(path_to_exe, "/exe");
    char *tilda = (char *)malloc(1000 * sizeof(char));
    readlink(path_to_exe, tilda, 1000);
    for (int i = strlen(tilda) - 1; i >= 0; i--)
    {
        if (tilda[i] == '/')
        {
            tilda[i] = '\0';
            break;
        }
    }
    strcpy(global_tilda, tilda);
    while (1)
    {
        printf("%s", display_user(tilda, display_name));
        NO_COMMAND = 0;
        char *input = (char *)malloc(10000 * sizeof(char));
        input = parse_input();
        char **commands = store_input(input);
        for (int i = 0; i < NO_COMMAND; i++)
        {
            char **arguments = (char **)calloc(1000 * 1000, sizeof(char));
            arguments = store_command(commands[i]);
            int out_redirection = 0;
            int inp_redirection = 0;
            int piping = 0;
            int numOfPipes = 0;
            int left_arrow = 0;
            int file_ind = 0;
            int single_arr = 0;
            int double_arr = 0;
            int pipe_pos[100];
            for (int j = 1; j < NO_ARGS; j++)
            {
                if (strcmp(arguments[j], "<") == 0)
                {
                    left_arrow++;
                    file_ind = j + 1;
                }
                if (strcmp(arguments[j], "|") == 0)
                {
                    pipe_pos[numOfPipes++] = j;
                    piping = 1;
                }

                if (strcmp(arguments[j], ">") == 0)
                {
                    single_arr++;
                }
                else if (strcmp(arguments[j], ">>") == 0)
                {
                    double_arr++;
                }
            }
            if (left_arrow > 1)
            {
                printf("Invalid number of input redirections.\n");
                break;
            }
            if (single_arr + double_arr > 1)
            {
                printf("Invalid number of output redirections given.\n");
                break;
            }
            if (left_arrow == 1)
            {
                inp_redirection = 1;
                int input_file = open(arguments[file_ind], O_RDONLY);
                if (input_file != -1)
                {
                    dup2(input_file, 0);
                    close(input_file);
                }
                else
                {
                    perror("");
                    break;
                }
            }
            for (int j = 0; j < numOfPipes; j++)
            {
                char **command = (char **)malloc(1000);
                for (int f = 0; f < 1000; f++)
                {
                    command[f] = (char *)malloc(1000);
                }
                int size = 0;
                if (j == 0)
                {
                    if (inp_redirection == 1)
                    {
                        for (int k = 0; k < file_ind - 1; k++)
                        {
                            strcpy(command[size++], arguments[k]);
                        }
                    }
                    else
                    {
                        for (int k = 0; k < pipe_pos[0]; k++)
                        {
                            strcpy(command[size++], arguments[k]);
                        }
                    }
                }
                else
                {
                    for (int k = pipe_pos[j - 1] + 1; k < pipe_pos[j]; k++)
                    {
                        strcpy(command[size++], arguments[k]);
                    }
                }
                strcpy(command[size], "\0");
                int pipe_arr[2];
                pipe(pipe_arr);
                if (size > 0)
                {
                    execute_commands((char **)command, tilda, size, pipe_arr);
                }
                close(pipe_arr[1]);
                dup2(pipe_arr[0], 0);
            }

            char **last_pipe = (char **)malloc(1000);
            for (int f = 0; f < 1000; f++)
            {
                last_pipe[f] = (char *)malloc(1000);
            }
            int last_size = 0;
            if (piping == 1)
            {
                for (int k = pipe_pos[numOfPipes - 1] + 1; k < NO_ARGS; k++)
                {
                    strcpy(last_pipe[last_size++], arguments[k]);
                }
            }
            else
            {
                if (inp_redirection == 1 && single_arr + double_arr == 0)
                {
                    for (int k = 0; k < file_ind - 1; k++)
                    {
                        strcpy(last_pipe[last_size++], arguments[k]);
                    }
                }
                else if (inp_redirection == 0 && single_arr + double_arr == 1)
                {
                    for (int k = 0; k < NO_ARGS; k++)
                    {
                        strcpy(last_pipe[last_size++], arguments[k]);
                    }
                }
                else if (inp_redirection == 1 && single_arr + double_arr == 1)
                {
                    for (int k = 0; k < file_ind - 1; k++)
                    {
                        strcpy(last_pipe[last_size++], arguments[k]);
                    }
                    strcpy(last_pipe[last_size++], arguments[NO_ARGS - 2]);
                    strcpy(last_pipe[last_size++], arguments[NO_ARGS - 1]);
                }
                else
                {
                    for (int k = 0; k < NO_ARGS; k++)
                    {
                        strcpy(last_pipe[last_size++], arguments[k]);
                    }
                }
            }
            strcpy(last_pipe[last_size], "\0");
            // printf("%d\n", piping);
            // for (int k = 0; k < last_size; k++)
            // {
            //     printf("%s\n", last_pipe[k]);
            // }
            // if ((strcmp(arguments[NO_ARGS - 2], ">") == 0 || strcmp(arguments[NO_ARGS - 2], ">>") == 0) && single_arr + double_arr == 1)
            // {
            //     out_redirection = 1;
            //     int output_file = -1;
            //     if (strcmp(arguments[NO_ARGS - 2], ">>") == 0 && strcmp(arguments[NO_ARGS - 1], ">") != 0 && strcmp(arguments[NO_ARGS - 1], ">>") != 0)
            //     {
            //         output_file = open(arguments[NO_ARGS - 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
            //     }
            //     else if (strcmp(arguments[NO_ARGS - 2], ">") == 0 && strcmp(arguments[NO_ARGS - 1], ">") != 0 && strcmp(arguments[NO_ARGS - 1], ">>") != 0)
            //     {
            //         output_file = open(arguments[NO_ARGS - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
            //     }
            //     if (output_file != -1)
            //     {
            //         strcpy(arguments[NO_ARGS - 2], "\0");
            //         strcpy(arguments[NO_ARGS - 1], "\0");

            //         // for (int k = 0; k < NO_ARGS; k++)
            //         // {
            //         //     printf("%s\n", arguments[k]);
            //         // }
            //         dup2(output_file, 1);

            //         close(output_file);
            //         // execute_commands(arguments, tilda, NO_ARGS - 2);
            //         // dup2(std_out, 1);
            //     }
            // }

            int arr[2];
            int output_file = -1;
            if (single_arr + double_arr == 1)
            {
                if (last_size > 2)
                {
                    if (last_pipe[last_size - 2][0] == '>')
                    {
                        out_redirection = 1;
                        if (strcmp(last_pipe[last_size - 2], ">>") == 0 && strcmp(last_pipe[last_size - 1], ">") != 0 && strcmp(last_pipe[last_size - 1], ">>") != 0)
                        {
                            output_file = open(last_pipe[last_size - 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
                        }
                        else if (strcmp(last_pipe[last_size - 2], ">") == 0 && strcmp(last_pipe[last_size - 1], ">") != 0 && strcmp(last_pipe[last_size - 1], ">>") != 0)
                        {
                            output_file = open(last_pipe[last_size - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
                        }
                        // strcpy(last_pipe[last_size - 2], "\0");
                        // strcpy(last_pipe[last_size - 1], "\0");
                        // last_pipe[last_size - 2][0] = '\0';
                        // last_pipe[last_size - 1][0] = '\0';
                        last_size -= 2;
                    }
                }
            }
            // dup2(output_file, 1);
            arr[1] = output_file;
            // fprintf(stderr, "%d\n", last_size);
            // for (int k = 0; k < last_size; k++)
            // {
            //     fprintf(stderr, "%s ", last_pipe[k]);
            // }
            // fprintf(stderr, "\n");
            if (last_size > 0)
                execute_commands(last_pipe, tilda, last_size, arr);
            // dup2(std_out, 1);
            // int arr[2];
            // if (out_redirection == 0)
            // {
            //     execute_commands(last_pipe, tilda, last_size, arr);
            // }
            // else
            // {
            //     execute_commands(last_pipe, tilda, last_size - 2, arr);
            // }
            // int req = NO_ARGS;
            // int start = 0;
            // int end = NO_ARGS;
            // if (out_redirection == 1)
            // {
            //     req -= 2;
            //     end -= 2;
            // }
            // if (inp_redirection == 1)
            // {
            //     req = file_ind - 1;
            //     start = file_ind + 1;
            // }
            // if (inp_redirection == 1)
            // {
            dup2(std_in, 0);
            // }
            // if (out_redirection == 1)
            // {
            dup2(std_out, 1);
            // }
            // }
            // else
            // {
            // int pipe_arr[2];
            // execute_commands(arguments, tilda, NO_ARGS, pipe_arr);
            // }
        }
        // if (out_redirection == 0 && inp_redirection == 0)
        // {
        //     execute_commands(arguments, tilda, NO_ARGS);
        // }
    } // output redirection
}
