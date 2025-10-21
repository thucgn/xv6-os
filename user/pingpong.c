#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"

int main(int argc, char* argv[]) {
    int p[2];
    char data;
    pipe(p);
    int pid;
    if((pid = fork()) == 0) { //child
        read(p[0], &data, 1);
        printf("<%d>:received ping\n", pid);
        data = 'B';
        write(p[1], &data, 1);
    } else if(pid > 0) {
        data = 'A';
        write(p[1], &data, 1);
        read(p[0], &data, 1);
        wait(0);
        printf("<%d>:received pong\n", getpid());
    } else { // pid < 0
        fprintf(2, "fork error\n");
        exit(1);
    }
    close(p[0]);
    close(p[1]);
    exit(0);
}