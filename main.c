#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include <signal.h>
#include <string.h>
#include <stdbool.h>


int SharedVariable = 0;

// added pointer to take input from main
void *SimpleThread(void *a) {
    int *p;
    p = a;
    int num, val = 0;

    for(num = 0; num < 20; num++) {
        if (rand() > (RAND_MAX / 2))
            usleep(500);

        val = SharedVariable;
        printf("*** thread %d sees value %d\n", *p, val);
        SharedVariable = val + 1;
    }
    val = SharedVariable;
    printf("Thread %d sees final value %d\n", *p, val);
}

int main(int argc, char *argv[])
{
    pthread_t *newThread; // create pointer for pointing to thread

    // check number of inputs
    if (argc != 2){
        printf("Invalid number of arguments\n a.out <n>\n");
        return 0;
    }
    else {
        bool inCheck = true; // boolean check for valid input 

        // foor loop that goes until end of string input
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
            // convert command line string of threads to int
            int n = atoi(argv[1]);
            // check thread state of sleep or awake
            newThread = (pthread_t*)malloc(sizeof(pthread_t) * atoi(argv[1])); // allocate size for number of awake threads
            // set pointer to size of input threads
            int *j = (int *)malloc(sizeof(int)*n);

            // for loop to count through each thread from input
            for (int i = 0; i<n; i++){
                j[i] = i;
            }

            // for loop to create new threads
            for (int i = 0; i<n; i++){
                pthread_create(&newThread[i], NULL, SimpleThread, &j[i]); // NULL so order doesn't matter which thread is being output
            }                                                             // so long as it's awake to show the current number

            // loop to wait for the thread to finish running before continuing 
            for (int i = 0; i<n; i++){
                pthread_join(newThread[i], NULL);
            }
        }
    }
    exit(0); // close program
}