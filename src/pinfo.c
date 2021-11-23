#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include "p.h"
extern int alphasort();
void pinfo(char **arguments, int NO_ARGS)
{
    struct dirent **process_list;
    char path[100] = "/proc";
    char req[100];
    int check = 0;
    int file_no = scandir(path, &process_list, NULL, alphasort);

    if (NO_ARGS == 1)
    {
        sprintf(req, "%d", shell_pid);
    }
    else
    {
        for (int i = 0; i < file_no; i++)
        {
            if (strcmp(process_list[i]->d_name, arguments[1]) == 0)
            {
                check = 1;
            }
        }
        if (check == 0)
        {
            printf("Given process ID does not exist\n");
            return;
        }
        strcpy(req, arguments[1]);
    }
    char path_status[100];
    char exec_path[100];
    strcpy(path_status, path);
    strcat(path_status, "/");
    strcat(path_status, req);
    strcat(path_status, "/");
    strcpy(exec_path, path_status);
    strcat(path_status, "status");
    FILE *status_file;
    status_file = fopen(path_status, "r");
    char *buf = (char *)malloc(10000 * sizeof(char));
    char *process_status = (char *)malloc(10000 * sizeof(char));
    char *vm_size = (char *)malloc(10000 * sizeof(char));
    while (fgets(buf, 10000, status_file))
    {
        if (strstr(buf, "State:"))
        {
            char *temp = (char *)malloc(10000);
            strcpy(temp, buf);
            process_status = __strtok_r(temp, "\t", &temp);
            process_status = __strtok_r(temp, " ", &temp);
        }
        if (strstr(buf, "VmSize:"))
        {
            char *temp = (char *)malloc(10000);
            strcpy(temp, buf);
            vm_size = __strtok_r(temp, "\t", &temp);
            vm_size = __strtok_r(temp, " ", &temp);
        }
    }
    fclose(status_file);
    strcat(exec_path, "exe");
    char *path_exec = (char *)malloc(10000 * sizeof(char));
    readlink(exec_path,path_exec,10000);
    printf("pid -- %s\n", req);
    printf("Process Status -- %s\n", process_status);
    printf("memory -- %s\n", vm_size);
    printf("Executable Path --​ ​ %s\n", path_exec);
}