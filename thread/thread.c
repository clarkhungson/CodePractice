#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
pthread_t tid1, tid2;

void * working_1(void *arg){
	for (int i = 0; i < 5; i++){
		printf("Thread 1: %d \n", (int)arg);
		sleep(1);
	}
}


void * working_2(void *arg){
	for (int i = 0; i < 10; i++){
		printf("Thread 2: %d \n", (int)arg);
		sleep(1);
	}
}

int main(void)
{
	int err1, err2;

	int arg1 = 10, arg2 = 20;
	// Create thread 1, 2
	err1 = pthread_create(&(tid1), NULL, &working_1, (void *) arg1);
	if (err1 != 0){
		printf("Fail to create thread 1\n");
	}
	err2 = pthread_create(&(tid2), NULL, &working_2, (void *) arg2);
	if (err2 != 0){
		printf("Fail to create thread 1\n");
	}
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	//sleep(25);
	return 0;
}