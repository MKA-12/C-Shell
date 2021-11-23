#include<stdio.h>
#include<stdlib.h>
#include<sys/utsname.h>
#include<string.h>
char *pwd();
char *display_user(char *tilda,char * display_name)
{
    struct utsname mach_info;
    int check_error = uname(&mach_info);
    if (check_error)
    {
        perror("");
    }

    char system_name[100];
    strcpy(system_name, mach_info.nodename);
    char user_name[50];
    strcpy(user_name, getenv("USER"));

    char *cwd = (char *)calloc(1000, sizeof(char));
    char shell_wd[1000];
    strcpy(shell_wd, pwd());
    if (strlen(tilda) <= strlen(shell_wd))
    {
        if (strstr(shell_wd, tilda) != NULL)
        {
            char temp[100] = "~";
            strcat(temp, &shell_wd[strlen(tilda)]);
            strcpy(cwd, temp);
        }
        else
        {
            strcpy(cwd, shell_wd);
        }
    }
    else
    {
        strcpy(cwd, shell_wd);
    }

    strcpy(display_name, "<");
    strcat(display_name, user_name);
    strcat(display_name, "@");
    strcat(display_name, system_name);
    strcat(display_name, ":");
    strcat(display_name, cwd);
    strcat(display_name, ">");
    return display_name;
}