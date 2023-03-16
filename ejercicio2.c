#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS 2
#define ITERATIONS 10   //  amount of iterations each thread will go through

int index = 0;              //  index
int turn = 0;           //  indicate which thread turn is

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *threadCount(void *arg) {
    int id = *(int*)arg;

    for (int k = 0; k < ITERATIONS; k++) {
        pthread_mutex_lock(&mutex);     //  locks the mutex on the current thread
        
        //  checks if its the turn of the thread to increase the index
        //  if its not, the mutex is realeased and assigned to the other thread 
        while (turn != id) {             
            pthread_mutex_unlock(&mutex);
            usleep(5000);
            pthread_mutex_lock(&mutex);
        }

        index++;    //  increase the counter
        printf("Thread %d increasing the index to %d\n", id, index); // prints which thread increased the counter
        turn = !id; // assigns the turn to the other thread once the counter was increased
        pthread_mutex_unlock(&mutex);   //unlocks the mutex
        printf("Thread %d finished increasing the index\n", id);    // indicates the thread has finished its turn

        
    }
    

    pthread_exit(NULL);
}

int main() {

    pthread_t threadArray[THREADS];
    int ids[THREADS] = {0, 1};  // ids to identify the threads


    // creates N threads to increase the counter
    for (int i = 0; i < THREADS; i++) {
        pthread_create(&threadArray[i], NULL, threadCount, &ids[i]);
    }

    //  joins the threads
    for (int j = 0; j < THREADS; j++) {
        pthread_join(threadArray[j], NULL);
    }

    //  deletes the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
