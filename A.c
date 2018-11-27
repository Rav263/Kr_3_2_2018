#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <time.h>

int proc(void)
{
    int pid = fork();
    if (!pid) {
        write(1, "1\n", 2);
    }
    return pid;
}


int main(int argc, char *argv[]) {
    int a = (proc() || proc()) && (proc() || proc());
    for(; wait(NULL) != -1;) {
        if (a != 0) a+= 1;
    }

    //printf("%d!!!!!!\n", a);

    return 0;
}
