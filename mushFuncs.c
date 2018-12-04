//CREATED BY YUSUF BAHADUR AND ANDREW YAN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>

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
    return s;
}
