//CREATED BY YUSUF BAHADUR AND ANDREW YAN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>

#define STAGES 10

void interrupt_handler(int signum);
int make_stages(char ***stages, char *cmd_line_cpy_y1, int **size_of);
int redirect_and_pipe(char **stages, int **size_of, int *pipes, int c_stages, 
    int *read_pipe);
int forker(int write, int *read_pipe, char **stages, int *out_flag, int fdout, char **argv, int *pipes, int c_stages );
