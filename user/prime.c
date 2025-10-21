#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"


void checker(int* parent_p) {
    int prime, n;
    close(parent_p[1]); // close it to receive end of file signal
    if(read(parent_p[0], &prime, sizeof(int)) <= 0)
        return; // no number
    printf("prime %d\n", prime);

    int p[2];
    pipe(p); // p[0] for read, p[1] for write
    

    int pid = fork();
    if(pid == 0) {
        checker(p);
    } else if(pid > 0) {
        close(p[0]); // no need for reading, release fd slot;
        while(read(parent_p[0], &n, sizeof(int)) > 0) { // read from left neighbor
            if(n % prime == 0) // prime divide n
                continue;
            // send to right neighbor
            write(p[1], &n, sizeof(int));
        }
        close(parent_p[0]);
        close(p[1]);
        wait(0);
    } else {
        fprintf(2, "Error fork\n");
        exit(1);
    }
    exit(0);
}

int main(int argc, char* argv[]) {

    int p[2];
    pipe(p);
    int pid = fork();
    if(pid == 0) {
        checker(p);
    } else if(pid > 0) {
        for(int i = 2;i <= 35; i ++) {
            write(p[1], &i, sizeof(int));
        }
        close(p[0]);
        close(p[1]);
        wait(0);
    }
    exit(0);
}