#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 10
#define constant 0x0A

pthread_t rand_8bit_thread, xor_thread;
pthread_mutex_t mutex;

int numArray[SIZE];
int i = 0;

void* rand_8bit_Num(void* arg){
    while(i < SIZE){

        int randomNumber = rand() % 256;    // random 8 bit number
        pthread_mutex_lock(&mutex);         // assign the array to the current thread

        if(i < SIZE){
            numArray[i++] = randomNumber;   // adds the number to the array
        }
        pthread_mutex_unlock(&mutex);       // unlocks the array for the other thread to use
        usleep(10000);                      // wait 10 ms before generating a new number
    }
    pthread_exit(NULL);
}

void* xor(void* arg){
    while(i < SIZE){
        pthread_mutex_lock(&mutex);         // lock the array on this thread

        if(i > 0){
            int previousNum = numArray[i - 1];      //get the number that was recently added to the array
            int result = previousNum ^ constant;    //apply xor
            char asciiResult = (char) result;       //cast to char
            printf("Numero %d Caracter: %c\n", previousNum, asciiResult);
        }

        pthread_mutex_unlock(&mutex);       //unlock the array when the xor has been applied
        usleep(10000);
    }
    pthread_exit(NULL);
}

int main(){

    //  start the mutex to control the access to the array
    pthread_mutex_init(&mutex, NULL);

    //  create a thread for each function
    pthread_create(&rand_8bit_thread, NULL, &rand_8bit_Num, NULL);
    pthread_create(&xor_thread, NULL, &xor, NULL);

    //  wait for the threads to end
    pthread_join(rand_8bit_thread, NULL);
    pthread_join(xor_thread, NULL);

    //  delete the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}