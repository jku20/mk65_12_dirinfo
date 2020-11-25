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

int file_size(struct dirent *de, char *cwd) {
    //introduces a hard limit of 511 to path length but that should be fine
    char path[512];
    if(0) {//de->d_type == DT_DIR) { currently like this untill I do (or don't do) recursive thing

    } else {
        int a = 0;
        while(a < 512 && cwd[a]) {
            path[a++] = cwd[a];
        }
        path[a++] = '/';
        int b = 0;
        while(a < 512 && de->d_name[b]) {
            path[a++] = de->d_name[b++];
        }
        path[a] = '\0';
        struct stat s;
        if(stat(path,&s) == -1) {
            printf("using path %s, error %d: %s\n",path, errno, strerror(errno));
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

int main(int argc, char **argv) {
    char to_analyze[255];
    if(argc <= 1) {
        printf("Enter a Directory:");
        fgets(to_analyze, sizeof(to_analyze), stdin);
        *strchr(to_analyze,'\n') = '\0';
    } else {
        strncpy(to_analyze, argv[1], sizeof(to_analyze)-1);
    }

    printf("Statistics For Directory: %s\n\n", to_analyze); 

    DIR *df;
    safe_open(to_analyze, &df);

    struct dirent *cur = readdir(df);
    long long total_size = 0;
    while(cur) {
        if(cur->d_type != DT_DIR) {
            cur = readdir(df);
            continue;
        }
        //total_size += file_size(cur); //not including directories for now
        print_file_info(cur);
        cur = readdir(df);
    }
    rewinddir(df);
    cur = readdir(df);
    while(cur) {
        if(cur->d_type == DT_DIR) {
            cur = readdir(df);
            continue;
        }
        total_size += file_size(cur,to_analyze);
        print_file_info(cur);
        cur = readdir(df);
    }
    printf("\ntotal size: %lld bytes\n", total_size);

    closedir(df);

    return 0;
}

