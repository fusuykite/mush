//CREATED BY YUSUF BAHADUR AND ANDREW YAN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include "parse.h"
#include "mush.h"


int main(int argc, char *argv[]) {
    int mush = 1;
    char cmd_line[MAX_CMD_LEN] = { 0 }; /* Cmd line delimited by spaces */
    char cmd_line_cpy[MAX_CMD_LEN] = { 0 }; 
	char cmd_line_cpy_y1[MAX_CMD_LEN] = {0};
	char ***stages = (char ***)calloc(STAGES, sizeof(char **));
    char *arg_token[MAX_CMD_LEN];  
    char *pipe_token[MAX_CMD_PIPE];
    int *arg_count = calloc(1, sizeof(int));
    int *pipes = calloc(1, sizeof(int));
    struct sigaction sa_interrupt;
    *arg_count = 0;
    *pipes = 0;
    int num_stages = 0; /* # of stages */
    int x, i = 0;
    int **size_of = (int **)calloc(STAGES, sizeof(int *));



    /* Set up the signals */
    memset(&sa_interrupt, 0, sizeof(sa_interrupt));
    sa_interrupt.sa_handler = &interrupt_handler;

    while (mush) {
        /*
        if (sigaction(SIGINT, &sa_interrupt, NULL) == -1) {
            perror("Error: ");
            return 1;
        }*/
        printf("8-P ");
        
        /* Reads in the command line */
        if (read_cmd_line(cmd_line) == 0) {
            strcpy(cmd_line_cpy, cmd_line);
            
            /* Breaks up based on spaces to err check */
            if (token_args(arg_token, cmd_line, &arg_count, &pipes) == 0) {

                if (err_check_input(arg_token, *arg_count, *pipes) == 0) {
                    /* Break up based on stages */

                }
            }
            strcpy(cmd_line_cpy_y1, cmd_line_cpy);
            num_stages = make_stages(stages, cmd_line_cpy_y1, size_of);
    
            /*printf("Stage: %d Argument: %d | '%s\n'", i, x, stages[1][2]);
            printf("Stages: %d\n", num_stages);
            for(i=0; i < num_stages; i++){ 
                for(x=0; x < *(size_of[i]); x++){ 
                        printf("Stage: %d Argument: %d | '%s\n'", i, x, stages[i][x]); 
                }
            }
            */

        }

        /* Shell resets itself */
        *arg_count = 0;
        *pipes = 0;
        memset(arg_token, 0, MAX_CMD_LEN);
        memset(cmd_line, 0, MAX_CMD_LEN);
        memset(cmd_line_cpy, 0, MAX_CMD_LEN);
        fflush(stdout);
    }
    free(pipes);
    free(arg_count);
    return 0;
    
}
