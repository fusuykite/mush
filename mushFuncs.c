//CREATED BY YUSUF BAHADUR AND ANDREW YAN

#include "parse.h"
#include "mush.h"

/* Handles ^C sigint interrupt */
void interrupt_handler(int signum) {
    printf("Can't kill me i'm a bad bitch\n");
    fflush(stdout);
}

void pipes(char **token_list, int stage) {
}
	
/* Changing directory */
int change_directory(char *path) {
    DIR *d;
    if ((d = opendir(path)) == NULL) {
        perror(path);
        return 1;
    }
    else {
        chdir(path);
    }
    return 0;
}
int make_stages(char ***stages, char *cmd_line, int **size_of){
    int s = 0;
    int a = 0;
    int i = 0;
    char *token;
    
   
    token = strtok(cmd_line, " ");
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

/* Takes the strings for each stage and number of arguments */
int redirect(char **stages, int num_args, int pflag) {
    int i = 0;
    int fdin;
    int fdout;
    for (i = 0; i < num_args; i++) {

        /* Check if '>' or '<' is encountered */
        if (pflag == 0) {
            if (!(strcmp(stages[i], ">"))) {
                /* Call open on the arg after '>' */
                if ((fdout = open(stages[i + 1], STDOUT_FILENO)) == -1) {
                    return -1;
                } 
                dup2(fdout, STDOUT_FILENO);
                close(fdout);
            }
            else if (!(strcmp(stages[i], "<"))) {
                /* Call open on the arg after '<' */
                if ((fdin = open(stages[i + 1], STDIN_FILENO)) == -1) {
                
                    return -1;
                }
                dup2(fdin, STDIN_FILENO);
                close(fdin);
            }
        }
    }
    return 0;
}





