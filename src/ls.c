#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>
#include <fcntl.h>
#include <dirent.h>
extern int alphasort();
int file_select(const struct dirent *file)
{
    return (file[0].d_name[0] != '.');
}
int file_no_select(const struct dirent *file)
{
    return (file[0].d_name[0] == '.');
}
void ls(char **arguments, char *tilda, int NO_ARGS)
{
    int flag_count = 0;
    int dir_count = 0;
    char **flags_list = (char **)malloc(1000 * sizeof(char *));
    char **dir_list = (char **)malloc(1000 * sizeof(char *));
    for (int i = 0; i < 1000; i++)
    {
        flags_list[i] = (char *)malloc(1000 * sizeof(char));
        dir_list[i] = (char *)malloc(1000 * sizeof(char));
    }
    for (int i = 1; i < NO_ARGS; i++)
    {
        if (arguments[i][0] == '-')
        {
            strcpy(flags_list[flag_count++], arguments[i]);
        }
        else
        {
            strcpy(dir_list[dir_count++], arguments[i]);
        }
    }
    if (dir_count == 0)
    {
        strcpy(dir_list[dir_count++], ".");
    }
    int l_flag = 0;
    int a_flag = 0;
    for (int i = 0; i < flag_count; i++)
    {
        if (strcmp("-l", flags_list[i]) == 0)
        {
            l_flag = 1;
        }
        else if (strcmp("-la", flags_list[i]) == 0)
        {
            l_flag = 1;
            a_flag = 1;
        }
        else if (strcmp("-al", flags_list[i]) == 0)
        {
            l_flag = 1;
            a_flag = 1;
        }
        else if (strcmp("-a", flags_list[i]) == 0)
        {
            a_flag = 1;
        }
        else
        {
            printf("ls: invalid option -- '%s'\n", flags_list[i]);
            return;
        }
    }
    for (int i = 0; i < dir_count; i++)
    {
        struct dirent **file_list;
        struct dirent **hidden_file_list;
        char *path_given = (char *)malloc(1000 * sizeof(char));
        // strcpy(path_given, dir_list[i]);
        if (dir_list[i][0] == '~')
        {
            dir_list[i] = dir_list[i] + 1;
            strcpy(path_given, tilda);
            strcat(path_given, dir_list[i]);
        }
        else
        {
            strcpy(path_given, dir_list[i]);
        }
        printf("%s\n", path_given);
        int file_no = scandir(path_given, &file_list, file_select, alphasort);
        int hidden_file_no = scandir(path_given, &hidden_file_list, file_no_select, alphasort);
        if (file_no == -1 || hidden_file_no == -1)
        {
            perror("");
            continue;
        }

        if (l_flag == 0)
        {
            if (a_flag == 1)
            {

                for (int j = 0; j < hidden_file_no; j++)
                {
                    printf("%s\n", hidden_file_list[j][0].d_name);
                }
            }
            for (int j = 0; j < file_no; j++)
            {
                printf("%s\n", file_list[j][0].d_name);
            }
        }
        else
        {
            if (a_flag == 1)
            {
                for (int j = 0; j < hidden_file_no; j++)
                {

                    char *path_to_file_hid = (char *)malloc(1000 * sizeof(char));
                    strcpy(path_to_file_hid, path_given);
                    struct stat curr_file_hid;
                    strcat(path_to_file_hid, "/");

                    strcat(path_to_file_hid, hidden_file_list[j]->d_name);
                    stat(path_to_file_hid, &curr_file_hid);

                    char *pass_hide = (char *)malloc(1000 * sizeof(char));
                    strcpy(pass_hide, getpwuid(curr_file_hid.st_uid)->pw_name);
                    char *grp_hide = (char *)malloc(1000 * sizeof(char));
                    strcpy(grp_hide, getgrgid(curr_file_hid.st_gid)->gr_name);

                    char *time_print_hid = (char *)calloc(1000, sizeof(char));
                    strftime(time_print_hid, 1000, "%c", localtime(&curr_file_hid.st_mtime));

                    printf(((S_ISDIR(curr_file_hid.st_mode)) ? "d" : "-"));
                    printf((curr_file_hid.st_mode & S_IRUSR) ? "r" : "-");
                    printf((curr_file_hid.st_mode & S_IWUSR) ? "w" : "-");
                    printf((curr_file_hid.st_mode & S_IXUSR) ? "x" : "-");
                    printf((curr_file_hid.st_mode & S_IRGRP) ? "r" : "-");
                    printf((curr_file_hid.st_mode & S_IWGRP) ? "w" : "-");
                    printf((curr_file_hid.st_mode & S_IXGRP) ? "x" : "-");
                    printf((curr_file_hid.st_mode & S_IROTH) ? "r" : "-");
                    printf((curr_file_hid.st_mode & S_IWOTH) ? "w" : "-");
                    printf((curr_file_hid.st_mode & S_IXOTH) ? "x" : "-");
                    printf("%5ld %10s %10s %12ld %20s %s", curr_file_hid.st_nlink, pass_hide, grp_hide, curr_file_hid.st_size, time_print_hid, hidden_file_list[j]->d_name);
                    printf("\n");
                }
            }
            for (int j = 0; j < file_no; j++)
            {

                char *path_to_file = (char *)malloc(1000 * sizeof(char));
                strcpy(path_to_file, path_given);
                struct stat curr_file;
                strcat(path_to_file, "/");

                strcat(path_to_file, file_list[j]->d_name);
                stat(path_to_file, &curr_file);

                char *pass = (char *)malloc(1000 * sizeof(char));
                strcpy(pass, getpwuid(curr_file.st_uid)->pw_name);
                char *grp = (char *)malloc(1000 * sizeof(char));
                strcpy(grp, getgrgid(curr_file.st_gid)->gr_name);

                char *time_print = (char *)calloc(1000, sizeof(char));
                strftime(time_print, 1000, "%c", localtime(&curr_file.st_mtime));
                printf(((S_ISDIR(curr_file.st_mode)) ? "d" : "-"));
                printf((curr_file.st_mode & S_IRUSR) ? "r" : "-");
                printf((curr_file.st_mode & S_IWUSR) ? "w" : "-");
                printf((curr_file.st_mode & S_IXUSR) ? "x" : "-");
                printf((curr_file.st_mode & S_IRGRP) ? "r" : "-");
                printf((curr_file.st_mode & S_IWGRP) ? "w" : "-");
                printf((curr_file.st_mode & S_IXGRP) ? "x" : "-");
                printf((curr_file.st_mode & S_IROTH) ? "r" : "-");
                printf((curr_file.st_mode & S_IWOTH) ? "w" : "-");
                printf((curr_file.st_mode & S_IXOTH) ? "x" : "-");

                printf("%5ld %10s %10s %12ld %20s %s", curr_file.st_nlink, pass, grp, curr_file.st_size, time_print, file_list[j]->d_name);
                printf("\n");
            }
        }
        if (i < dir_count - 1)
        {
            printf("\n\n");
        }
    }
}