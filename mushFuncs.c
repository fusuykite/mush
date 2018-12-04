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

/*function to redirect based off of token_list*/
/*
void redirect(char cmd_line_cpy_y1, int stage, int pipe_flag){
	char *token;
	int less_than = 0;
	int more_than = 0;
	int status;

	char *first;	

	token = strtok(token_list[stage], " ");
	first = token;
	
	while (token != NULL) {
		
		if strcmp(token, '<') {
			less_than = 1;
			close(0);
			
		}
		
		else if (strcmp(token, '>')) {
			more_than = 1;
			close(1);
		}
	
		else if (less_than == 1) {	
			open(token, "r");
		}

		else if (more_than == 1) {
			open(token, "w");
		}
	
		token = strtok(NULL, " ");
	}
	pid_t pid = fork();

	if(pid == 0)	{	
		execl(*first, *first, NULL);		
		perror("Execl failed");
		exit(1);

	}

	else {
		waitpid(pid, &status, 0);

      if ( WIFEXITED(status)) {
        int exit_status = WEXITSTATUS(status);

         if(exit_status){
            perror("redirrect child process returned with error\n");
				exit(1);
         }
      }
	}

}*/	
