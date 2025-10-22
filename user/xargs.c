#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"


int main(int argc, char* argv[]) {
    if(argc <= 1) {
        fprintf(2, "Usage: command | xargs command\n");
        exit(1);
    }

    // copy args to ustack, argv[1] is the target command
    char buf[512];
    char* ustack[MAXARG];
    for(int i = 1;i < argc; i ++) {
        ustack[i-1] = argv[i];
    }
    ustack[argc-1] = buf;
    ustack[argc] = 0;

    char c;
    int len = 0;
    int pid;
    while(read(0, &c, 1) != 0) {
        if(c == '\n' && len) { // exec command
            buf[len] = 0; // '\0' for the end of string
            len = 0; // reset len for newline
            if((pid=fork()) == 0) {
                exec(argv[1], ustack);
                exit(1);
            } else if (pid > 0) {
                wait(0);
            } else { // fork failed
                fprintf(2, "xargs: fork failed\n");
                exit(1);
            }
        } else {
            buf[len ++] = c;
        }

        if(len == 512) {
            fprintf(2, "xargs: too long input\n");
            break;
        }
    }
    exit(0);
}