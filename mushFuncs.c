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
                if ((fdout = open(stages[i + 1], O_RDWR | 
                    O_CREAT | O_TRUNC)) == -1) {
                    fprintf(stderr, "%s: ", stages[i + 1]);
                    perror(NULL);
                    return 1;
                } 
                dup2(fdout, STDOUT_FILENO);
                close(fdout);
            }
            else if (!(strcmp(stages[i], "<"))) {

                /* Call open on the arg after '<' */
                if ((fdin = open(stages[i + 1], O_RDONLY)) == -1) {
                    fprintf(stderr, "%s: ", stages[i + 1]);
                    perror(NULL);
                    return 1;
                }
                dup2(fdin, STDIN_FILENO);
                close(fdin);
            }
        }
    }
    return 0;
}

/* Checks if the first argument is cd 
 * returns 1 on error of if the directory is changed
 * returns 0 if first argument is not cd */

int cd_checker(char **stages, int num_arg, int num_pipe) {
    int i = 0;
    
    if (!strcmp(stages[0], "cd")) {
        if (num_pipe != 0) {
            fprintf(stderr, "%s: too many arguments.\n", stages[0]);
            return 1;
        }
        if (num_arg > 2) {
            fprintf(stderr, "%s: too many arguments.\n", stages[0]);
            return 1;
        }
        else if (num_arg == 1) {
            fprintf(stderr, "%s: missing argument.\n", stages[0]);
            return 1;
        }
        else {
            change_directory(stages[1]);
            return 1;
        }
    }
    return 0;
}

int fork_test(char **stages, int num_arg) {
    int i = 0;
    int idx_ct = 0;
    int argc = num_arg;
    int wait_int;
    pid_t fork_result;
    char *cmd = stages[0];
    /*char *argv[num_arg + 1]; */
    char **argv = NULL;
    
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
    
    if ((fork_result = fork()) == -1) {
        fprintf(stderr, "%s", "Fork has failed\n");
        exit(EXIT_FAILURE);
    }

    else if (fork_result == 0) {
        execvp(cmd, argv);
        fprintf(stderr, "%s: ", stages[0]);
        perror(NULL);
        return 1;
    }

    else {
        wait(&wait_int);
        if (wait_int != 0) {
            fprintf(stderr, "%s\n", "Process unsucessful");
            return 1;
        }
        else {
            printf("Proccess successful\n");
            fflush(stdout);
            return 0;
        }
    }

    free(argv);
    return 0;
}
    









