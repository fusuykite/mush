#ifndef __MYMUSH__
#define __MYMUSH__

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
#include "parse.h"

/* Interrupt handler for ^C */
void interrupt_handler(int signum);

/* Takes in the stage with the CD as the argument */
int change_directory(char *cd_stage);

/* takes in the cmd line and formats it */
int make_stages(char ***stages, char *cmd_line, int **size_of);

int cd_checker(char **stages, int num_arg, int num_pipe);

int wait_kids(int **kid_pids, int num_kids);

int redirect_and_pipe(char **stages, int **size_of, int *pipes, int c_stages, 
int *read_pipe, int **kid_pids);

int forker(int write, int *read_pipe, char **stages, int *out_flag, 
int fdout, char **argv, int *pipes, 
int c_stages, int **kid_pids);

void freetp(char ***stages, int **size_of, int *pipes, int *read_pipe, int ff);

void freetp2(char ***stages, int **size_of, int *pipes);

#endif /* __MYMUSH__ */

