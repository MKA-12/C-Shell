#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
char *pwd()
{
    size_t size_buf = 1000 * sizeof(char);
    char *cwd = (char *)malloc(size_buf);
    if (getcwd(cwd, size_buf) != NULL)
    {
        return cwd;
    }
    else
    {
        return NULL;
    }
}