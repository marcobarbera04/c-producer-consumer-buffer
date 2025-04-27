#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 8

extern sem_t start, produce, consume;
extern pthread_mutex_t producer_mutex, consumer_mutex;

extern int buffer[BUFFER_SIZE];

typedef enum{
    PRODUCER = 0,
    CONSUMER = 1
}THREAD_TYPE;

typedef struct thread_data{
    THREAD_TYPE thread_type;
    int thread_id;
}thread_data;


void* thread_function(void* arg);

#endif