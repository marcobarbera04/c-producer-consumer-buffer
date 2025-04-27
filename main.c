#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "function.h"

#define MAX_THREAD 8

extern sem_t start, produce, consume;
extern pthread_mutex_t producer_mutex, consumer_mutex;

extern int buffer[BUFFER_SIZE];

int main(){
    srand(time(NULL));  // generate seed for random numbers

    // init the buffer to -1
    for(int i = 0; i < BUFFER_SIZE; i++){
        buffer[i] = -1;
    }

    pthread_t threads[MAX_THREAD];
    thread_data threads_datas[MAX_THREAD];

    sem_init(&start, 0, 0);                 // semaphore that will start all threads at the same time
    sem_init(&produce, 0, BUFFER_SIZE);     // semaphore that counts how many items have can be produced
    sem_init(&consume, 0, 0);               // semaphore that counts how many items can be consumed
    pthread_mutex_init(&producer_mutex, 0); // mutex to handle producers
    pthread_mutex_init(&consumer_mutex, 0); // mutex to handle consumers

    // creating all thread 
    for(int i = 0; i < MAX_THREAD; i++){
        threads_datas[i].thread_id = i;
        if(i % 2 == 0){
            threads_datas[i].thread_type = PRODUCER;    // thread with even id are producer
        }
        else{
            threads_datas[i].thread_type = CONSUMER;    // thread with odd id are consumer
        }
        pthread_create(&threads[i], NULL, thread_function, &threads_datas[i]);
    }
    
    // increment the semaphore to max threads number
    for(int i = 0; i < MAX_THREAD; i++){
        sem_post(&start);
    }

    // join all thread
    for(int i = 0; i < MAX_THREAD; i++){
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&start);
    sem_destroy(&produce);
    sem_destroy(&consume);
    pthread_mutex_destroy(&producer_mutex);
    pthread_mutex_destroy(&consumer_mutex);

    for(int i = 0; i < BUFFER_SIZE; i++){
        printf("[%d]%d\n", i, buffer[i]);
    }

    return 0;
}