//CREATED BY YUSUF BAHADUR AND ANDREW YAN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>

void interrupt_handler(int signum) {
    printf("Can't kill me i'm a bad bitch\n");
    fflush(stdout);
}
