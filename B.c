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

int proc(int *fir_p, char *name) {
    close(fir_p[0]);    
        
    int sum = 0;
    FILE *file = fopen(name, "r");
    if(file != NULL) {
        int now = 0;

        while (fscanf(file, "%d", &now) == 1) {
            sum += now;//(long long) now;
        }
        fclose(file);
    }
    write(fir_p[1], &sum, sizeof(sum));
    close(fir_p[1]);
    return 0;
}


int main(int argc, char *argv[]) {
    int fir_p[2];
    int sec_p[2];
    pipe(sec_p);
    pipe(fir_p);
  
    if (fork() == 0) {
        proc(fir_p, argv[1]); 
        exit(0);
    }

    if (fork() == 0) {
        proc(sec_p, argv[2]);
        exit(0);
    }

    close(fir_p[1]);
    close(sec_p[1]);

    int sum1 = 0;
    int sum2 = 0;
    
    read(fir_p[0], &sum1, sizeof(sum1));
    read(sec_p[0], &sum2, sizeof(sum2));
    close(fir_p[0]);
    close(sec_p[0]);
    int big_sum = (long long) sum2 + sum1;

    printf("%d\n%d\n%d\n", sum2, sum1, big_sum);

    wait(NULL);
    wait(NULL);

    return 0;
}
