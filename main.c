#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>

void safe_open(char *d, DIR **df) {
    *df = opendir(d);
    if(!*df) {
        printf("error %d: %s\n",errno, strerror(errno));
        exit(-1);
    }
}

int file_size(struct dirent *de) {
    if(0) {//de->d_type == DT_DIR) { currently like this untill I do (or don't do) recursive thing

    } else {
        struct stat s;
        if(stat(de->d_name,&s) == -1) {
            printf("error %d: %s\n", errno, strerror(errno));
            exit(-1);
        }
        return s.st_size;
    }
}

void print_file_info(struct dirent *de) {
    if(de->d_type == DT_DIR) {
        printf("Directory    %s\n",de->d_name);
    } else {
        printf("Regular File %s\n",de->d_name);
    }
}
int main() {
    DIR *df;
    safe_open(".", &df);

    struct dirent *cur = readdir(df);
    long long total_size = 0;
    while(cur) {
        print_file_info(cur);
        total_size += file_size(cur);
        cur = readdir(df);
    }
    printf("\ntotal size: %lld bytes\n", total_size);

    closedir(df);

    return 0;
}

