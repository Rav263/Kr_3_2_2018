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

int proc(char *argv[], int beg) {
    int pid = fork();
    if (pid == 0) {
        execvp(argv[beg], &(argv[beg]));
        exit(1);
    }
    int status;
    wait(&status);

    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        return 1;
    }

    return 0;
}


int main(int argc, char *argv[]) {
    if (argc <= 2) {
        return 0;
    }
    int beg = 2;

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[1], argv[i]) == 0) {
            argv[i] = NULL;            

            int res = proc(argv, beg);
            if (res != 0) {
                return 1;
            }

            beg = i + 1;
        }
    }

    int res = proc(argv, beg);
    if (res != 0) {
        return 1;
    }
    return 0;
}
