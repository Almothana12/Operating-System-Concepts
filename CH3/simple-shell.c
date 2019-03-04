#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define MAX_LINE    80 /* 80 chars per line, per command */
#define TRUE 1
#define FALSE 0

int main(void)
{
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
    int should_run = TRUE;
	int should_wait = FALSE; /* will be be TRUE if user input is "(command) &" */ 
    
    while(should_run) {
        printf("osh>");
        
        char command[MAX_LINE];
        scanf(" %[^\n]",command);
        fflush(stdout);
        
        
        
        char* temp = (char*) malloc(MAX_LINE*sizeof(char*));
        int j=0; // counter of temp
        int i;
        int upper=0;
        for(i=0; command[i]!='\0'; i++) {
            if((int)command[i] == ' ') {
                args[upper] = temp;
                upper++;
                temp = (char*) malloc(MAX_LINE*sizeof(char*));
                j=0;
            }
            else {
                temp[j++] = command[i];
            }
        }
        if (temp[0] == '&')
            should_wait = TRUE;
        args[upper++] = temp;
        args[upper] = NULL;
        
        /*
        for(i=0; args[i] != NULL; i++){
            printf("args[%d] = %s\n", i, args[i]);
        }
        */

        pid_t pid;
        pid = fork();
        
        if(pid < 0) {
            printf("ERROR FORKING CHILD");
            return 1;
        }
        else if (pid == 0) { /* child process */
            execvp(args[0], args);
        }
        else {               /* parent */
            if(should_wait)
                wait(NULL);
        }

    }
        
        /**
         * After reading user input, the steps are:
         * (1) fork a child process
         * (2) the child process will invoke execvp()
         * (3) if command included &, parent will invoke wait()
         */
    
    
    
	return 0;
}
