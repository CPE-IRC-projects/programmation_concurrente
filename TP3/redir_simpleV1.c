#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
int main()
{
    int input = open("/etc/passwd",O_RDONLY);
    dup2(input, 0);
    execlp("wc","wc",NULL);
}