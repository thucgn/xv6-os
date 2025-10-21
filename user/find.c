#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"


void find(char* dname, char* target) {
    if(dname == 0)
        return;

    int fd;
    struct dirent de;
    struct stat st;
    
    if((fd = open(dname, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", dname);
        return;
    }

    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", dname);
        return;
    }

    if(st.type == T_DEVICE || st.type == T_FILE) {
        fprintf(2, "find: should be directory %s\n", dname);
        return;
    }

    char buf[512];
    if(strlen(dname) + 1 + DIRSIZ + 1 > sizeof(buf)) {
        printf("find: path too long\n");
        return;
    }

    strcpy(buf, dname);
    char* p = &buf[strlen(dname)];
    *p++ = '/';

    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        if(de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0) {
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        if(st.type == T_DEVICE || st.type == T_FILE) {
            if(strcmp(de.name, target) == 0) 
                printf("%s\n", buf);
            continue;
        }

        if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;
        find(buf, target);
    }
    close(fd);
}

int main(int argc, char* argv[]) {
    if(argc < 3) {
        fprintf(2, "Usage: find dir name");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}