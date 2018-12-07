//CREATED BY YUSUF BAHADUR AND ANDREW YAN

#include "parse.h"
#include "mush.h"


int main(int argc, char *argv[]) {
    int i = 0; /* Testing */
    int mush = 1;
    char cmd_line[MAX_CMD_LEN] = { 0 }; /* Cmd line delimited by spaces */
    char cmd_line_cpy[MAX_CMD_LEN] = { 0 }; /* Original line */
    char *arg_token[MAX_CMD_LEN];  /* Delimited by spaces */
    
    char ***stages = (char ***)calloc(MAX_CMD_PIPE, sizeof(char **));
    int **stage_arg = (int **)calloc(MAX_CMD_PIPE, sizeof(int *));

    pid_t *kid_pids[MAX_CMD_PIPE] = { 0 };
    int *arg_count = calloc(1, sizeof(int));
    int *pipes = calloc(1, sizeof(int));
    struct sigaction sa_interrupt;

    int *pipe_in[1];
    int *pipe_out[1];

    *arg_count = 0;
    *pipes = 0;

    /* Set up the signals */
    memset(&sa_interrupt, 0, sizeof(sa_interrupt));
    sa_interrupt.sa_handler = &interrupt_handler;

    /* Signal handler */
    if (sigaction(SIGINT, &sa_interrupt, NULL) == -1) {
        perror("Error: ");
        return 1;
    }

    while (mush) {
        printf("8-P ");
        fflush(stdout);
        
        /* Reads in the command line */
        if (read_cmd_line(cmd_line) == 0) {
            strcpy(cmd_line_cpy, cmd_line);
            
            /* Breaks up based on spaces to err check */
            if (token_args(arg_token, cmd_line, &arg_count, &pipes) == 0) {

                if (err_check_input(arg_token, *arg_count, *pipes) == 0) {
                    make_stages(stages, cmd_line_cpy, stage_arg);
                    if (cd_checker(stages[0], *stage_arg[0], *pipes) == 0) {
                        for (i = 0; i <= *pipes; i++) {
                            if (fork_test(stages[i], *stage_arg[i], 
                            kid_pids, i, pipe_in, pipe_out, *pipes + 1) != 0) {
                                break;
                            }   
                        }
                        wait_kids(kid_pids, i);
                    }
                }
            }
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
