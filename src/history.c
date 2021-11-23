#include <stdio.h>
#include <sys/utsname.h> //display user
#include <string.h>      //display user
#include <stdlib.h>      //split and store
#include <unistd.h>      //split and store
#include <fcntl.h>       //ls
#include <dirent.h>      //ls
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h> // 6ques
void history_input(char *input)
{
    // FILE *his_file = fopen("history.txt", "a");
    // fprintf(his_file, input, 10000);
    // fwrite(input,1,10000,his_file);
    // fclose(his_file);
    // int fd = open("history.txt","a");
    // write();
}
void history(char **arguments, int NO_ARGS,FILE* file)
{
    // FILE *file = fopen("/history.txt", "r");
    int no_of_lines = 0;
    char ch = fgetc(file);
    while (ch != EOF)
    {
        if (ch == '\n')
        {
            no_of_lines++;
        }
        ch = fgetc(file);
    }
    fseek(file, 0, SEEK_SET);
    int counter = no_of_lines;
    int lines = no_of_lines;
    if (NO_ARGS == 1)
    {
        if (no_of_lines > 10)
            lines = 10;
        else
            lines = no_of_lines;
    }
    else
    {
        sscanf(arguments[1], "%d", &lines);
    }
    if (lines > no_of_lines)
    {
        lines = no_of_lines;
    }
    if (lines > 20)
    {
        if (no_of_lines > 20)
            lines = 20;
        else
        {
            lines = no_of_lines;
        }
    }
    int c = 0;
    char *buffer = (char *)malloc(10000 * sizeof(char));
    for (c = 0; c < (no_of_lines - lines); c++)
    {
        fgets(buffer, 1000, file);
    }
    while (c < no_of_lines)
    {
        fgets(buffer, 1000, file);
        printf("%s", buffer);
        c++;
    }
    free(buffer);
    fclose(file);
}