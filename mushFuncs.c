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

void pipes(char **token_list, int stage){
	
	



/*function to redirect based off of token_list*/

void redirect(char **token_list, int stage, int pipe_flag){
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
		execl(*first, *first, NULL);			/*potentially change the arguments */
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

}	
