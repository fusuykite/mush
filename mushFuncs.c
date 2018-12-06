//CREATED BY YUSUF BAHADUR AND ANDREW YAN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include "mush.h"
#include <unistd.h>
#include <fcntl.h>


#define STDOUT_FILENO 1
#define STDIN_FILENO 0

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
    return 0;
}



int redirect_and_pipe(char **stages, int **size_of, int *pipes, int c_stages, 
    int *read_pipe) {
    /*Retad is the past read call*/
    
    int i = 0;                                                                  
    int fdin;  
    int fdout;
    int pip[2];
    int write;
    int ret = 0;
    int *out_flag = calloc(1, sizeof(int));  
    *out_flag = 0;
    int idx_ct = 0;
    int argc;
    char **argv = NULL;
    int num_arg;

    num_arg = *(size_of[c_stages]);
    argc = num_arg;

    printf("Argv Maker Beginning");
    fflush(stdout);

    for (i = 0; i < num_arg; i++) {
        if (!strcmp(stages[i], "<")) {
            argc = argc - 2;
        }
        else if (!strcmp(stages[i], ">")) {
            argc = argc - 2;
        }
    }

    argv = calloc(argc + 1, sizeof(*argv));

    if (argc == 1) {
        argv[0] = stages[argc - 1];
        argv[argc] = NULL;
    }
    else {
        for (i = 0; i < num_arg; i++) {
            if (strcmp(stages[i], ">") != 0 && strcmp(stages[i], "<") != 0) {
                argv[idx_ct] = stages[i];
                idx_ct++;
            }
            else {
                i++;
            }
        }
        argv[argc] = NULL;
    } 

   
    printf("Beginning of Redirect\n");
    fflush(stdout);
    
    if(c_stages == 0 || c_stages == *pipes) {

        for(i = 0; i < *(size_of[c_stages]); i++) {

            if((strcmp(stages[i], ">")) == 0) {

                if((fdout = open(stages[i+1], O_RDWR | O_CREAT | O_TRUNC
                )) == -1) {
                    return -1;
                }

                *out_flag = 1;
                /*dup2(fdout, STDOUT_FILENO);;
                close(fdout);*/
            }
            
            if((strcmp(stages[i], "<")) == 0) {

                if((fdin = open(stages[i+1], O_RDONLY)) == -1) {

                    return -1;
                }

                *read_pipe = fdin;
               /* dup2(fdin, STDIN_FILENO);
                close(fdin);*/
            }
        }
    }

    printf("After redirect\n");
    fflush(stdout);
    printf("# of pipes: %d\n", *pipes);
    fflush(stdout);

    /*if last stage only set up input pipe and execl*/

    if(c_stages == (*pipes)) {
        printf("inside compare last stage pipe\n");
        fflush(stdout);

        printf("Read Pipe: %d\n", *read_pipe);
        fflush(stdout);

        if((*read_pipe != STDIN_FILENO)) {
            printf("Inside compare last stage pipe read pipe comp\n");
            fflush(stdout);
            dup2(*read_pipe, 0);
            close(*read_pipe);
        }

        if(*out_flag == 1) {
            dup2(fdout, STDOUT_FILENO);
            close(fdout);
        }

        //printf("inside compare last stage pipe 2\n");
        //fflush(stdout);
        execvp(stages[0],argv);
    }

    else {
 
        pipe(pip);

        
        printf("Before forker\n");
        fflush(stdout);
        
        ret = forker(pip[1], read_pipe, stages, out_flag, fdout, argv);
        
        printf("After forker\n");
        fflush(stdout);
        *read_pipe = pip[0];
        
        printf("Read Pipe: %d\n", *read_pipe);
        fflush(stdout);
        printf("After setting read_pipe in redirect_pipe");
        fflush(stdout);

        close(write);
        printf("Before return to main\n");
        fflush(stdout);

        return ret;
    }
}


int forker(int write, int *read_pipe, char **stages, int *out_flag, int fdout, char **argv ) {

    pid_t pid = fork();
    
    printf("After Forking\n");
    fflush(stdout);

    if(pid == 0) {
        if(!(*read_pipe == STDIN_FILENO)){
            dup2(*read_pipe, STDIN_FILENO);
            close(*read_pipe);
        }
        if(*out_flag == 1){
            dup2(fdout, STDOUT_FILENO);
            close(fdout);
            *out_flag = 0;
        }
        else if(!(write == STDOUT_FILENO)) {
            dup2(write, STDOUT_FILENO);
            close(write);
        }
       
        printf("Before execl in fork\n");
        fflush(stdout);

        execvp(stages[0], argv);
        fprintf(stderr, "%s: ", stages[0]);
        perror(NULL);

    }
    
    return pid;
}
        


