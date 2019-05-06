#include <stdio.h>
#include <pthread.h>

#define NUM_STUDENTS 5        /* the number of students */
#define NUM_WAITING_CHAIRS 3  /* the number of waiting chairs */

int waiting_chair[NUM_WAITING_CHAIRS];
int wc_status;  /* number of unavilable chairs */

void* student(void*);  /* the student thread */
void* ta(void*);       /* the TA thread */

int main(void)
{

	pthread_t student_id[NUM_STUDENTS]; /* the students identifiers */
	pthread_t ta_id;                    /* the TA id */

	pthread_attr_t attr; /* set of attributes for the thread */
        pthread_attr_init(&attr); /* get the default attributes */

	int i;
	for(i=0; i<NUM_STUDENTS; i++)
		pthread_create(&student_id[i], &attr, student, NULL);  /* create the students thread */

	pthread_create(&ta_id, &attr, ta, NULL);  /* create the TA thread */

}


void* student(void* param)
{


	return NULL;
}

void* ta(void* param)
{

	while(1) {
		if(wc_status == 0 )
			nap();
	}

	return NULL;
}
