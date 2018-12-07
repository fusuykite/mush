//CREATED BY YUSUF BAHADUR AND ANDREW YAN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include "parse.h"
#include "mush.h"
#include <sys/stat.h> 
#include <fcntl.h>

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
    int current_stage = 0;
    int *read_pipe = calloc(1, sizeof(int));
    *read_pipe = 0;
    int allow = 1;
    int fd = 0;
    

    /* Set up the signals */
    memset(&sa_interrupt, 0, sizeof(sa_interrupt));
    sa_interrupt.sa_handler = &interrupt_handler;

    while (mush) {
        current_stage = 0;
        *read_pipe = 0;
        allow = 1;
        *pipes = 0;
        *arg_count = 0;
        num_stages = 0;
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
                    make_stages(stages, cmd_line_cpy, size_of);

                }
                while(current_stage <= *pipes && allow != -1) {        

                    //printf("Current Stage in Main %d and Last read pipe: %d\n", current_stage, *read_pipe);
                    //fflush(stdout);

                    allow = redirect_and_pipe(stages[current_stage], size_of,
                             pipes, current_stage, read_pipe);
                    
                    
                    //printf("Before looping to next current stage in main %d\n\n", current_stage);
                    //fflush(stdout);
                    current_stage ++;
                }

               // printf("OUT OF STAGE \n");
                //fflush(stdout);

            }
    
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
