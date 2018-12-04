//CREATED BY YUSUF BAHADUR AND ANDREW YAN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>

/* Interrupt handler for ^C */
void interrupt_handler(int signum);

void pipes(char **token_list, int stage);

/* Takes in the stage with the CD as the argument */
int change_directory(char *cd_stage);
