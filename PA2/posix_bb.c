#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <deque>
#include <unistd.h>
#include <signal.h>

#define TRUE 1

/* functions for threads to call */
void *producer(void *param); //for producer
void *consumer(void *param); //for consumer

//using three semaphores
sem_t empty;
sem_t full;
sem_t mutex;
int shared_item = 0;
std::deque<int> myboundedbuffer;
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
	do {
		sem_wait(&empty);
		sem_wait(&mutex);
		shared_item++;
		if (shared_item > 20)
			break;
		printf("Producer create an item %d\n", shared_item);
		myboundedbuffer.push_back(shared_item);
		sem_post(&mutex);
		sem_post(&full);
	} while (TRUE);
	
	kill(pid, SIGINT); //send a signal to kernel to terminate the process
	pthread_exit(0);
}

void *consumer(void *param) {
	int id = *(int*)param;
	printf("consumer Thread ID = %d\n", id);
	do {
		sem_wait(&full);
		sem_wait(&mutex);
		printf("\tConsumer process an item %d\n",
		myboundedbuffer.front()) ;
		myboundedbuffer.pop_front();
		sem_post(&mutex);
		sem_post(&empty);
	} while (TRUE);
	pthread_exit(0);
}
