#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>

int SharedVariable = 0;
pthread_mutex_t lock;
pthread_barrier_t barr;

// added pointer to take input from main
void *SimpleThread(void *a) {
    int *p;
    p = a;
    int num, val = 0;

    for(num = 0; num < 20; num++) {
        // macro for compilation
#ifdef PTHREAD_SYNC
        pthread_mutex_lock(&lock); // -- (CRITICAL-SECTION)
        // (code only to be accessed by 1 thread @ a time, all other threads wait here until the mutex is "unlocked")
#endif

        val = SharedVariable;
        printf("*** thread %d sees value %d\n", *p, val);
        SharedVariable = val + 1;

#ifdef PTHREAD_SYNC
        pthread_mutex_unlock(&lock); // -- (end of CRITICAL-SECTION)
#endif
    }
    // "BARRIER": a point in the program where threads stop to wait
    //            until ALL threads have reached the barrier.
    // barrier so that all thread will read the shared variable value and display the final value.
    pthread_barrier_wait(&barr);
    val = SharedVariable;
    printf("Thread %d sees final value %d\n", *p, val);
}

int main(int argc, char *argv[])
{
    pthread_t *newThread; // create pointer for pointing to thread

    // check number of inputs
    if (argc != 2){ // "2": 1 for the program name + 1 for the argument passed (AKA the # of threads)
        printf("Invalid number of arguments\n");
        printf("a.out <n>\n");
        return 0;
    }
    else {
        bool inCheck = true; // boolean check for valid input 

        // for loop that goes until end of string input
        for (int i = 0; i<strlen(argv[1]); i++){
            // check for negative input or if input is greater than 9
            if (argv[1][i] < '0' || argv[1][i] > '9'){     // valid numbers are 0-9
                inCheck = false;
                break;
            }
        }
        // print for invalid input
        if (inCheck = false){
            printf("\nPlease provide a positive integer as an argument\n");
        }
        else {
            // convert command line string of # of threads to int
            int NUM_THREADS = atoi(argv[1]);
            // check thread state of sleep or awake // (?)
            newThread = (pthread_t*)malloc(sizeof(pthread_t) * atoi(argv[1])); // allocate size for number of awake threads
            // set pointer to size of input threads // (?)
            int *j = (int *)malloc(sizeof(int)*NUM_THREADS);

            // Initialize MUTEX.
            pthread_mutex_init(&lock, NULL);
            // Initialize BARRIER.
            pthread_barrier_init(&barr, NULL, NUM_THREADS);

            // 1. Assign each thread a unique ID. (?)
            for (int i = 0; i<NUM_THREADS; i++){
                j[i] = i;
            }

            // 2. Create new threads & have them begin their subroutine, "SimpleThread()".
            for (int i = 0; i<NUM_THREADS; i++){
                // "&newThread[i]" : Pointer to an int, the ID of the Thread (?)
                // "SimpleThread"  : the subroutine to be executed by the thread
                // "&j[i]"         : the subroutine's arg
                pthread_create(&newThread[i], NULL, SimpleThread, &j[i]); // NULL so order doesn't matter which thread is being output
            }                                                             // so long as it's awake to show the current number

            // 3. Wait for all threads to finish running before continuing.
            for (int i = 0; i<NUM_THREADS; i++){
                pthread_join(newThread[i], NULL);
            }
        }
    }
    exit(0); // close program
}