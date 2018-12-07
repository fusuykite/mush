//CREATED BY YUSUF BAHADUR AND ANDREW YAN
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

void pipes(char **token_list, int stage);

/* Takes in the stage with the CD as the argument */
int change_directory(char *cd_stage);

/* takes in the cmd line and formats it */
int make_stages(char ***stages, char *cmd_line, int **size_of);

int redirect(char **stages, int num_arg, int pflag);

int cd_checker(char **stages, int num_arg, int num_pipe);

int fork_test(char **stages, int num_arg, int **kid_pids, int cur_stage,
int **in_pipe, int **out_pipe, int num_stages);

int wait_kids(int **kid_pids, int num_kids);

int pipe_test(char **stages, int num_arg);
#endif /* __MYMUSH__ */
