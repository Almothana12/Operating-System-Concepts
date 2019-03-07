#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 11

typedef struct 
{
    int row;
    int column;
    int thread_num;
}parameters;

int solutions[NUM_THREADS]; /* for each thread, put 1 if sudoku part is correct, 0 if it is incorrect, -1 otherwise */
int sudoku[9][9]; /*the solution to be vaildated*/

void *runner(void *param); /* the thread */
void initialize_parameters(parameters* list[NUM_THREADS]); /* initialize the parameters to be sent to the threads */
void read_input(); /* read input from "input.txt" and write them in sudoku[9][9] */

int main()
{
    read_input();

    int i;
    for(i=0; i<NUM_THREADS; i++) /*initialize solutions[] */
    {
        solutions[i] = -1;
    }
    
    parameters* parameters_list[NUM_THREADS];
    initialize_parameters(parameters_list);
    
    pthread_t workers[NUM_THREADS]; /* the threads identifiers */
    pthread_attr_t attr; /* set of attributes for the thread */


    /* get the default attributes */
    pthread_attr_init(&attr);

    /* create the threads */
    for(i=0; i<NUM_THREADS; i++)
        pthread_create(&workers[i],&attr,runner,parameters_list[i]);

    /* wait for the threads to exit */
    for(i=0; i<NUM_THREADS; i++)
        pthread_join(workers[i],NULL);
    
    for(i=0; i<NUM_THREADS; i++) {
        if(solutions[i] == 0 || solutions[i] == -1){
            printf("The sudoku is not correct :(\n");
            return 0;
        }
    }
    
    printf("The sudoku is correct !\n");

    return 0;
}


void* runner(void* param) 
{
    parameters* input = (parameters*) param;
    int check[10]; /* tick every number you encounter with 1 */
    int i, j, c;
    for(j=0; j<10; j++) /* reset check[] to use on next row */
        check[j] = 0;
    if(input->column == -1) /* check 9 rows */
    {
        for(i=0; i<9; i++) {
            for(j=0; j<9; j++) {
                c = sudoku[i][j];
                if(check[c] == 1){ /* if number already encountered */
                    solutions[input->thread_num] = 0; /* report zero to solutions */
                    pthread_exit(0);
                }
                else { /* firt time to encounter the number in this row */
                    check[c] = 1; /* mark it encountered */
                }
                
            }
            for(j=0; j<10; j++) /* reset check[] to use on next row */
                check[j] = 0;
        }
        
        solutions[input->thread_num] = 1; /* report that all rows are correct */
        pthread_exit(0);
        
    } 
    else if(input->row == -1) /* check 9 columns */
    {
        for(i=0; i<9; i++) {
            for(j=0; j<9; j++) {
                c = sudoku[j][i];
                if(check[c] == 1){ /* if number already encountered */
                    solutions[input->thread_num] = 0; /* report zero to solutions */
                    pthread_exit(0);
                }
                else { /* firt time to encounter the number in this column */
                    check[c] = 1; /* mark it encountered */
                }
                
            }
            for(j=0; j<10; j++) /* reset check[] to use on next column */
                check[j] = 0;
        }
        
        solutions[input->thread_num] = 1; /* report that all columns are correct */
        pthread_exit(0);
    }
    else /*check 3x3 square */
    {
        /* define this thread's 3x3 section */
        int row = input->row;
        int col = input->column;
        int row_lim = row+3; /* row limit */
        int col_lim = col+3; /* column limit */
        
        for(i=row; i<row_lim; i++)
        {
            for(j=col; j<col_lim; j++)
            {
                c = sudoku[i][j];
                
                if(check[c] == 1){ /* if number already encountered */
                    solutions[input->thread_num] = 0; /* report zero to solutions */
                    pthread_exit(0);
                }
                else { /* firt time to encounter the number in this 3x3 square */
                    check[c] = 1; /* mark it encountered */
                }
            }
        }
        solutions[input->thread_num] = 1; /* report everything is correct on this part */
        pthread_exit(0);
    }
    

}

void read_input() /* this could be implemented better */
{
    FILE* file = fopen("input.txt", "r");
    if(!(file)){
        printf("ERROR. are you sure that you have a file named \"input.txt\"? \n");
        exit(1); /* termenite program */
    }
    char c;
    int i, j;    
    for(i=0; i<9; i++) 
    {
        for(j=0; j<9; j++) 
        {
            REPEAT:
            c = fgetc(file); /* read from file */
            if(c == EOF) /* if we reached end-of-file */
                return;
            if(c >= '0' && c <= '9') /* if c is a single digit */
                sudoku[i][j] = c - '0'; /* turn c into an int and assign it to sudoku */
            else /* c is a whitespace or a newline character */
                goto REPEAT;/* repeat loop without incrementing j */
        }

    }

}
/* initialize the parameters to be sent to the threads */
void initialize_parameters(parameters* list[NUM_THREADS])
{
    parameters* par1 = (parameters*) calloc(1, sizeof(parameters));
    par1->column = -1;
    par1->thread_num = 0;
    list[0] = par1;
    
    parameters* par2 = (parameters*) calloc(1, sizeof(parameters));
    par2->row = -1;
    par2->thread_num = 1;
    list[1] = par2;
    

    parameters* par = (parameters*) calloc(1, sizeof(parameters));
    par->row = 0;
    par->column = 0;
    par->thread_num = 2;
    list[2] = par;
    
    par = (parameters*) calloc(1, sizeof(parameters));
    par->row = 0;
    par->column = 3;
    par->thread_num = 3;
    list[3] = par;
    
    par = (parameters*) calloc(1, sizeof(parameters));
    par->row = 0;
    par->column = 6;
    par->thread_num = 4;
    list[4] = par;

    par = (parameters*) calloc(1, sizeof(parameters));
    par->row = 3;
    par->column = 0;
    par->thread_num = 5;
    list[5] = par;
    
    par = (parameters*) calloc(1, sizeof(parameters));
    par->row = 3;
    par->column = 3;
    par->thread_num = 6;
    list[6] = par;
    
    par = (parameters*) calloc(1, sizeof(parameters));
    par->row = 3;
    par->column = 6;
    par->thread_num = 7;
    list[7] = par;
    
    par = (parameters*) calloc(1, sizeof(parameters));
    par->row = 6;
    par->column = 0;
    par->thread_num = 8;
    list[8] = par;
    
    par = (parameters*) calloc(1, sizeof(parameters));
    par->row = 6;
    par->column = 3;
    par->thread_num = 9;
    list[9] = par;
    
    par = (parameters*) calloc(1, sizeof(parameters));
    par->row = 6;
    par->column = 6;
    par->thread_num = 10;
    list[10] = par;
    
}
