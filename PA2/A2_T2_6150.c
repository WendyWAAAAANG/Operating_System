// Date: 2023/04/24
// Author: Ruoxin WANG
// Student ID: 2030026150
// requirement: replace the code that uses C++ with the code for C, store the output into file.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>

#define TRUE 1
#define BUFFER_SIZE 10

/* functions for threads to call */
void *producer(void *param); //for producer
void *consumer(void *param); //for consumer

//using three semaphores
sem_t empty;
sem_t full;
sem_t mutex;
// create a semaphore for file IO.
sem_t f_mutex;

int shared_item = 0;
int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
pid_t pid;

int main(int argc, char *argv[]) {
    int i, scope;
    pthread_t producerID, consumerID; /* the thread identifier */

    pthread_attr_t attr; /* set of attributes for the thread */

    int n = 10;
    pid = getpid();

    //create semaphores, and initialize
    sem_init(&empty, 0, n);//n
    sem_init(&full, 0, 0);//0
    sem_init(&mutex, 0, 1);//1
    sem_init(&f_mutex, 0, 1);//1

    /* get the default attributes */
    pthread_attr_init(&attr);

    /* create the threads */
    pthread_create(&producerID, &attr, producer, &producerID);
    pthread_create(&consumerID, &attr, consumer, &consumerID);

    /*Now join on each thread*/
    pthread_join(producerID, NULL);
    pthread_join(consumerID, NULL);

    return 0;
}

/*The thread will begin control in this function.*/
void *producer(void *param) {

	int id = *(int*)param;
	printf("producer Thread ID = %d\n", id);
	
	
	sem_wait(&f_mutex);
	FILE *fptr;
	fptr = fopen("A2_T2_6150.txt", "a");
	fprintf(fptr,"producer Thread ID = %d\n", id);
	fclose(fptr);
	sem_post(&f_mutex);
	

	do {
		sem_wait(&empty);
		sem_wait(&mutex);
		shared_item++;
		if (shared_item > 20)
			break;

		buffer[in] = shared_item;
		in = (in + 1) % BUFFER_SIZE;
		sem_post(&mutex);
		sem_post(&full);
		
		
		printf("Producer create an item %d\n", shared_item);

		sem_wait(&f_mutex);
		FILE *fptr;
		fptr = fopen("A2_T2_6150.txt", "a");
		//fprintf(fptr,"producer Thread ID = %d\n", id);
		fprintf(fptr,"Producer create an item %d\n", shared_item);
		fclose(fptr);
		sem_post(&f_mutex);

	} while (TRUE);

	kill(pid, SIGINT); //send a signal to kernel to terminate the process
	pthread_exit(0);
}

void *consumer(void *param) {

	int id = *(int*)param;
	printf("consumer Thread ID = %d\n", id);
	
	
	sem_wait(&f_mutex);
	FILE *fptr;
	fptr = fopen("A2_T2_6150.txt", "a");
	fprintf(fptr,"consumer Thread ID = %d\n", id);
	fclose(fptr);
	sem_post(&f_mutex);
	

	do {
		sem_wait(&full);
		sem_wait(&mutex);

		int item = buffer[out];
		out = (out + 1) % BUFFER_SIZE;

		sem_post(&mutex);
		sem_post(&empty);
		
		
		printf("\tConsumer process an item %d\n", item);

		sem_wait(&f_mutex);
		FILE *fptr;
		fptr = fopen("A2_T2_6150.txt", "a");
		//fprintf(fptr,"consumer Thread ID = %d\n", id);
		fprintf(fptr,"\tConsumer process an item %d\n", item);
		fclose(fptr);
		sem_post(&f_mutex);

	} while (TRUE);

	pthread_exit(0);
}
