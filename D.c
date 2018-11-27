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
#include <math.h>

void proc(double dx, double x0, unsigned int n, int num, int p, int *fds) {
    close(fds[0]);


    for (int i = 0; i < n; i++) {
        double now_point = x0 + dx * num + dx * p * i;

        double now = cos(now_point);


        write(fds[1], &now, sizeof(now));
    }
    close(fds[1]);
}

int main(int argc, char *argv[]) {
    if (argc < 6) {
        return 1;
    }
    double dx = atof(argv[1]);
    unsigned int p = atoi(argv[2]);
    double x0 = atof(argv[4]);
    unsigned int n = atoi(argv[5]);

    int fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (fd < 0) {
        return 1;
    }

    FILE *file = fdopen(fd, "w");

    if (file == NULL) {
        return 1;
    }

    int *fds = calloc(p * 2, sizeof(*fds));

    for (int i = 0; i < p * 2; i += 2) {
        if (pipe (&(fds[i])) < 0) {
            return 1;
        }
    }

    for (int i = 0; i < p; i++) {
        if (fork() == 0) {
            proc(dx, x0, n, i, p, &(fds[i * 2]));
            exit(0);
        } else {
            close(fds[i * 2 + 1]);
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p * 2; j += 2) {
            double now;

            read(fds[j], &now, sizeof(now));

            fprintf(file, "%.10e\n", now);
        }
    }

    for (int i = 0; i < p * 2; i += 2) {
        close(fds[i]);
    }

    fclose(file);
    close(fd);
    free(fds);

    return 0;
}
