#define MAX_LINE    80 /* 80 chars per line, per command */

int global_index = 1;

typedef struct {
    char cmnd [MAX_LINE];
    int index;
} Cmnd_struct;

typedef struct {
    Cmnd_struct list [10]; /* stores last 10 commands */
    int size;
} History;

void add_to_history(History* history, char command [MAX_LINE]) 
{
    if((int)command[0] ==  '!')
    {
        if(history->size == 0)
        {
            printf("No commands in history.\n");
        }
        else if((int)command[1] ==  '!') /* command is "!!" */ 
        {
            strcpy(command, history->list[ (history->size) - 1].cmnd) ; /* command = last command in history */
        }
        else /* command is "!n", where n is an int */
        {
            int n = atoi(&command[1]); /* n = command[1] NOTE: I don't know how but this code actually works even with n>9 !!! (more than one digit) */
            int i;
            for(i=0; i<10; i++)
            {
                if(n == history->list[i].index)
                {
                    strcpy(command, history->list[i].cmnd); /* command = list[i] */
                    add_to_history(history, command); /* put the command to the top of history (so it will be called when next input = !! )*/
                    printf("\nhistory> %s\n", history->list[i].cmnd);
                    return;
                }
            }
            printf("No such command in history.\n");
        }
        return;
    }
    
    Cmnd_struct* temp = (Cmnd_struct*) calloc(1, sizeof(Cmnd_struct));
    temp->index = global_index++;
    strcpy(temp->cmnd, command); /* temp->cmnd = command */
    
    if(history->size < 10) 
    {
        history->list[(history->size)++] = *temp;
    }
    else /* history list is full */
    {
        int i;
        for(i=0; i<10; i++)
        {
            history->list[i] = history->list[i+1];  /* shift left */
        }
        history->list[9] = *temp; /* insert the newest command */
    }
}



void display_history(History* history)
{
    if(history->size == 0)
    {
        printf("No commands in history.\n");
    }
    else
    {
        int i;
        for(i=history->size-1; i>=0; i--)
        {
            printf("%d %s\n", history->list[i].index, history->list[i].cmnd);
        }
    }

}
 
