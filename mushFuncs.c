//CREATED BY YUSUF BAHADUR AND ANDREW YAN

#include "parse.h"
#include "mush.h"

/* Handles ^C sigint interrupt */
void interrupt_handler(int signum) {
    printf("Can't kill me i'm a bad bitch\n");
    fflush(stdout);
}

void pipes(char **token_list, int stage) {
}
	
/* Changing directory */
int change_directory(char *path) {
    DIR *d;
    if ((d = opendir(path)) == NULL) {
        perror(path);
        return 1;
    }
    else {
        chdir(path);
    }
    return 0;
}
