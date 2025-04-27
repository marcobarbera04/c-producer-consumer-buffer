#include "function.h"

sem_t start, produce, consume;
pthread_mutex_t producer_mutex, consumer_mutex;

int buffer[BUFFER_SIZE];    // shared buffer
int in, out;                // index to insert and consume  


void* thread_function(void* arg){
    thread_data* data = (thread_data*)arg;
    sem_wait(&start);

    if(data->thread_type == PRODUCER){
        sem_wait(&produce);     // wait until items can be produced
        pthread_mutex_lock(&producer_mutex);
        
        sleep(2);       // see how producing and consuming happens simultaneously

        if(in < BUFFER_SIZE){
            buffer[in] = rand() % 101;      // random number between 0 and 100
            printf("Thread %d produced item %d at index %d\n", data->thread_id, buffer[in], in);
            in++;
        }else{
            // buffer is full
        }

        pthread_mutex_unlock(&producer_mutex);
        sem_post(&consume);     // increment consume semaphore
    }
    else if(data->thread_type == CONSUMER){
        sem_wait(&consume);     // wait until items can be consumed
        pthread_mutex_lock(&consumer_mutex);

        sleep(2);       // see how producing and consuming happens simultaneously

        if(out < in){
            int item = buffer[out];     // consume the item
            printf("Thread %d consumed item %d at index %d\n", data->thread_id, item, out);
            out++;
        }else{
            // buffer is empy
        }

        pthread_mutex_unlock(&consumer_mutex);
        sem_post(&produce);     // increment produce semaphore
    }
    
    pthread_exit(NULL);
}