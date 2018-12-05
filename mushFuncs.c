//CREATED BY YUSUF BAHADUR AND ANDREW YAN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include "mush.h"

void interrupt_handler(int signum) {
    printf("Can't kill me i'm a bad bitch\n");
    fflush(stdout);
}



int make_stages(char ***stages, char *cmd_line_cpy_y1, int **size_of){
    int s = 0;
    int a = 0;
    int i = 0;
    char *token;
	
    /*for(i = 0; i < 100; i++){
        stages[i] = '\0';
    } */
	
    token = strtok(cmd_line_cpy_y1, " ");
	stages[0] = (char **)calloc(100, sizeof(char *));
    size_of[0] = (int*)calloc(1, sizeof(int));

    while(token != NULL) {

        if (strcmp(token, "|") == 0) {
            *(size_of[s]) = a;  
            ++s;
            stages[s] = (char **)calloc(100, sizeof(char));
            size_of[s] = (int *)calloc(1, sizeof(int));  
            /*for(i = 0; i < 100; i++){
                stages[s][i] = '\0';
            }*/
            a = 0;
        }

        else {
            
            stages[s][a] = malloc(strlen(token) + 1);
            strcpy((stages[s][a]), token);
            /*printf("Stage: %d, Argument: %d, %s\n",s, a, (stages[s][a]));*/
            ++a;
        }
        token = strtok(NULL, " ");
    }
    
    *(size_of[s]) = a;
    ++s;
    return s;
}



int redirect_and_pipe(char **stages, int **size_of, int *pipes, int c_stages, 
    int *read_pipe) {
    /*Read is the past read call*/
    
    int i = 0;                                                                  
    int fdin;  
    int fdout;
    int pip[2];
    int write;
    int ret = 0;
    if(c_stages == 0 || c_stages == *pipes) {
        for(i = 0; i < size_of[c_stages]; i++) {
            if((strcmp(stages[i], ">")) == 0) {
                if((fdout = open(stages[i+1], STDOUT_FILENO)) == -1) {
                    return -1;
                }
                dup2(fdout, STDOUT_FILENO);;
                close(fdout);
            }
            
            if((strcmp(stages[i], "<")) == 0) {
                if((fdin = open(stages[i+1], STDIN_FILENO)) == -1) {
                    *read_pipe = fdin;
                    return -1;
                }
                dup2(fdin, STDIN_FILENO);
                close(fdin);
            }
        }
    }
    
    /*if last stage only set up input pipe and execl*/
    if(c_stages == (*pipes)) {
        if(!(*read_pipe == 0)) {
            dup2(*read_pipe, 0);
        }
        
        ret = execl(stages[0], stages, NULL);
        return ret;
    }
    /*if not last stage*/
    else{
        pipe(pip);

        write = pip[1];

        ret = forker(write, read_pipe, stages);

        *read_pipe = pip[0];
    
        close(write);
    
        return ret;
    }
}

int forker(int write, int *read_pipe, char **stages) {

    pid_t pid = fork();

    if(pid == 0) {
        if(!(*read_pipe == STDIN_FILENO)){
            dup2(*read_pipe, STDIN_FILENO);
            close(*read_pipe);
        }
    
        if(!(write == STDOUT_FILENO)) {
            dup2(write, STDOUT_FILENO);
            close(write);
        }
    
        return execl(stages[0], stages, NULL);
    }

    return pid;
}
        


