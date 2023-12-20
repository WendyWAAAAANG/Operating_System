// Date: 2023/04/24
// Author: Ruoxin WANG
// Student ID: 2030026150
// requirement: Further modify the code from task 2
// 1.	The program create two producers and two consumers and one reader.
// 2.	Producers cannot write to buffer at the same time
// 3.	Consumers cannot consume at the same time
// 4.	Producer and consumer cannot write and consume at the same time
// 5.	Consumers and reader cannot consume and reader at the same time
// 6.	Reader will read the first item in the current queue

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>

#define TRUE 1
#define BUFFER_SIZE 10

/* functions for threads to call */
void *producer(void *param); //for producer
void *consumer(void *param); //for consumer
void *reader(void *param); //for reader

//using three semaphores 
sem_t empty;
sem_t full;
sem_t mutex;
sem_t f_mutex;
sem_t r_mutex;

int shared_item = 0;
int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
pid_t pid;

int main(int argc, char *argv[]) {
    int i, scope;
    pthread_t producerID1, producerID2, consumerID1, consumerID2, readerID;

    /* the thread identifier */
    pthread_attr_t attr;

    /* set of attributes for the thread */
    int n = 10;
    pid = getpid();

    //create semaphores, and initialize
    sem_init(&empty, 0, n); //n
    sem_init(&full, 0, 0); //0
    sem_init(&mutex, 0, 1); //1
    sem_init(&f_mutex, 0, 1); //1
    sem_init(&r_mutex, 0, 1); //1

    /* get the default attributes */
    pthread_attr_init(&attr);

    /* create the threads */
    pthread_create(&producerID1, &attr, producer, &producerID1);
    pthread_create(&producerID2, &attr, producer, &producerID2);
    pthread_create(&consumerID1, &attr, consumer, &consumerID1);
    pthread_create(&consumerID2, &attr, consumer, &consumerID2);
    pthread_create(&readerID,&attr,reader,&readerID);

    /*Now join on each thread*/
    pthread_join(producerID1, NULL);
    pthread_join(producerID2,NULL);
    pthread_join(consumerID1,NULL);
    pthread_join(consumerID2,NULL);
    pthread_join(readerID,NULL);
    
    return 0;
}

/*The thread will begin control in this function.*/
void *producer(void *param) {

	int id = *(int*)param;
	printf("Producer Thread ID = %d\n", id);
	
	sem_wait(&f_mutex);
	FILE *fptr;
	fptr = fopen("A2_T3_6150.txt", "a");
	fprintf(fptr,"Producer Thread ID = %d\n", id);
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
		
		printf("Producer %d is putting the data %d at the position %d\n", id, shared_item, in);

		sem_wait(&f_mutex);
		FILE *fptr;
		fptr = fopen("A2_T3_6150.txt", "a");
		//fprintf(fptr,"producer Thread ID = %d\n", id);
		fprintf(fptr,"Producer %d is putting the data %d at the position %d\n", id, shared_item, in);
		fclose(fptr);
		sem_post(&f_mutex);
	} while (TRUE);

	kill(pid, SIGINT); //send a signal to kernel to terminate the process
	pthread_exit(0);
}

void *consumer(void *param) {

	int id = *(int*)param;
	printf("Consumer Thread ID = %d\n", id);
	
	sem_wait(&f_mutex);
	FILE *fptr;
	fptr = fopen("A2_T3_6150.txt", "a");
	fprintf(fptr,"Consumer Thread ID = %d\n", id);
	fclose(fptr);
	sem_post(&f_mutex);
	
	do {
		sem_wait(&full);
		sem_wait(&mutex);

		int item = buffer[out];
		out = (out + 1) % BUFFER_SIZE;

		sem_post(&mutex);
		sem_post(&empty);
		
		printf("\tConsumer %d is consuming data %d at the position %d\n", id, item, out);

		sem_wait(&f_mutex);
		FILE *fptr;
		fptr = fopen("A2_T3_6150.txt", "a");
		//fprintf(fptr,"consumer Thread ID = %d\n", id);
		fprintf(fptr,"\tConsumer %d is consuming data %d at the position %d\n", id, item, out);
		fclose(fptr);
		sem_post(&f_mutex);

	} while (TRUE);

	pthread_exit(0);
}

void *reader(void *param){
	int id=*(int*)param;
	printf("Reader Thread ID = %d\n", id);
	
	sem_wait(&f_mutex);
	FILE *fptr;
	fptr=fopen("A2_T3_6150.txt","a");
	fprintf(fptr,"Reader Thread ID = %d\n", id);
	fclose(fptr);
	sem_post(&f_mutex);

	do{
		sem_wait(&full);
		sem_wait(&mutex);
		sem_wait(&r_mutex);
		sem_wait(&f_mutex);
		
		FILE *fptr;
		fptr=fopen("A2_T3_6150.txt","a");
		//fprintf(fptr,"Reader Thread ID = %d\n",id);

		if(buffer[out]!=0){
			printf("\t\tReader is reading data %d at the position %d\n",buffer[out], out);
			fprintf(fptr,"\t\tReader is reading data %d at the position %d\n",buffer[out], out);
		}
		fclose(fptr);
		sem_post(&f_mutex);
		sem_post(&r_mutex);
		
		sem_post(&mutex);
		sem_post(&empty);

		//sleep(1);

	}while(TRUE);

	pthread_exit(0);

}
