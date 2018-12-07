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
	
    token = strtok(cmd_line_cpy_y1, " ");
	stages[0] = (char **)calloc(100, sizeof(char *));
    size_of[0] = (int*)calloc(1, sizeof(int));

    while(token != NULL) {

        if (strcmp(token, "|") == 0) {
            *(size_of[s]) = a;  
            ++s;
            stages[s] = (char **)calloc(100, sizeof(char));
            size_of[s] = (int *)calloc(1, sizeof(int));  
            a = 0;
        }

        else {
            
            stages[s][a] = malloc(strlen(token) + 1);
            strcpy((stages[s][a]), token);
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
    
    int i = 0;    
    int fd = 0;                                                               
    int fdout;
    int pip[2];
    
    int ret = 0;
    int *out_flag = calloc(1, sizeof(int));  
    *out_flag = 0;
    int idx_ct = 0;
    int argc;
    char **argv = NULL;
    int num_arg;
    int saved_stdin;
    int saved_stdout;

    /*save the original fd*/
    saved_stdin = dup(0);
    saved_stdout = dup(1);
    
    /*calculate the number of args for given stage */
    num_arg = *(size_of[c_stages]);
    argc = num_arg;

    /*CODE TO SET UP ARGS WITHOUT > < "*/
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


    /*CODE TO SET UP REDIRECTION */    
    if(c_stages == 0 || c_stages == *pipes) {

        for(i = 0; i < num_arg; i++) {

            if((strcmp(stages[i], ">")) == 0) {

                if((fdout = open(stages[i+1], O_RDWR | O_CREAT | O_TRUNC, 0666
                )) == -1) {
                    fprintf(stderr, "%s: ", stages[i + 1]);
                    perror(NULL);
                    return -1;
                }

                *out_flag = 1;
            }
            
            if((strcmp(stages[i], "<")) == 0) {

                if((*read_pipe = open(stages[i+1], O_RDONLY, 0666)) == -1) {
                    fprintf(stderr, "%s: ", stages[i + 1]);
                    perror(NULL);
                    return -1;
                }

            }
        }
    }

    /*CODE TO BEGIN PIPING*/
    if(c_stages <= (*pipes)){
 
        if(pipe(pip) == -1) {
            perror("Pipe Error");
            return -1;
        }
        
        /*function to start forking*/        
        
        ret = forker(pip[1], read_pipe, stages, out_flag, fdout, argv, 
                    pipes, c_stages);

        /*close all extra file_descriptors*/
        close(pip[1]);
        close(fdout);
        close(*read_pipe);

        /*restore original file descriptors*/        
        dup2(saved_stdin, 0);                                               
        dup2(saved_stdout, 1); 
        
        /*close saved input output*/
        close(saved_stdin);
        close(saved_stdout);

        if(ret == -1) {
            return -1;
        }
    
        /*if not last pipe set *read_pipe to current pipe*/

        if(c_stages == *pipes) {
            return 1;
        }
    
        else { 
            *read_pipe = pip[0];
            return 1;
        }

    }
    /*error hit*/
    else {
        return -1;
    }
}


int forker(int write, int *read_pipe, char **stages, int *out_flag, int fdout, 
            char **argv, int *pipes, int c_stages ) {

    pid_t pid = fork();
    
    if(pid == -1) {
        perror("Fork Error");
        return -1;
    }
    
    /*if child*/
    else if(pid == 0) {
        
        if(!(*read_pipe == STDIN_FILENO)) {
            dup2(*read_pipe, STDIN_FILENO);
            close(*read_pipe);
        }
        if(*out_flag == 1) {
            dup2(fdout, STDOUT_FILENO);
            close(fdout);
            *out_flag = 0;
        }
        if(c_stages != (*pipes)) {
            if(!(write == STDOUT_FILENO)) {
                dup2(write, STDOUT_FILENO);
                close(write);
            }
        }
       
        execvp(stages[0], argv);

        printf("%s: ", argv[0]);
        fflush(stdout);
        perror(NULL);
    }
    
    return pid;
}
        


