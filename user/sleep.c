#include "kernel/types.h"
#include "user.h"

int main(int argc, char* argv[]) {
    if(argc < 2) {
        fprintf(2, "Usage: sleep n\n");
        exit(1);
    }

    int n = atoi(argv[1]);
    if(n > 0)
        sleep(n);
    else {
        fprintf(2, "Error n\n");
        exit(-1);
    }
    exit(0);
}