/**
 * From the book:
 * 
 * After reading user input, the steps are:
 * (1) fork a child process
 * (2) the child process will invoke execvp()
 * (3) if command included &, parent will invoke wait()
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "history.c"

#define TRUE 1
#define FALSE 0

void read_command(char[MAX_LINE]);
void process_command(char[MAX_LINE], char* [MAX_LINE/2 + 1], int*);
void execute_command(char* [MAX_LINE/2 + 1], int);

int main(void) 
{
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	char command[MAX_LINE];     /* the command that the user will enter */
    int should_run = TRUE;
	int should_wait = FALSE; /* will be be TRUE if user input is "(command) &" */ 

    History* history = (History*) calloc(1, sizeof(History)); /* holds last 10 commands */
    
    while(should_run) 
    {
        read_command(command);

        if(strcmp(command, "history") == 0) /* if input is "history" */
        {
            display_history(history);
        }
        else if(strcmp(command, "exit") == 0)
        {
            return 0;
        }
        else
        {
            add_to_history(history, command); /* add command to history */
            process_command(command, args, &should_wait); /* process the command to seperate strings */
            execute_command(args, should_wait);
        }
    }
	return -1;
}


void read_command(char command [MAX_LINE])
{
    printf("\nosh> ");
    scanf(" %[^\n]",command); /* idk what " %[^\n]" does but it works :D */
    fflush(stdout);
}

void process_command(char command [MAX_LINE], char* args [MAX_LINE/2 + 1], int* should_wait )
{
    char* temp = (char*) malloc(MAX_LINE*sizeof(char));
    int j=0; /* counter of temp */
    int i;
    int upper=0;
    /* split command into words: */
    for(i=0; command[i]!='\0'; i++) /* iterate command char by char */
    { 
        if((int)command[i] == ' ') /* when you encounter a space, put the word into args */
        { 
            args[upper++] = temp;
            temp = (char*) malloc(MAX_LINE*sizeof(char)); /* reset temp */
            j=0; /* reset temp counter */
        }
        else 
        {
            temp[j++] = command[i];
        }
    }
    
    if (temp[0] == '&')
        *should_wait = TRUE;
    else
        args[upper++] = temp; /* put the last word into args */
        
    args[upper] = NULL; /* is this really necessary ?? */
}

void execute_command(char* args[MAX_LINE/2 + 1], int should_wait)
{
    pid_t pid = fork();

    if(pid < 0) 
    {
        printf("ERROR FORKING CHILD");
        return;
    }
    else if (pid == 0) /* child process */
    { 
        execvp(args[0], args);
    }
    else /* parent */
    {
        if(should_wait) /* program doesn't work too well when not invoking wait(), so I will ignore should_wait for now... */
            wait(NULL);
        else
            wait(NULL);
    }
}


