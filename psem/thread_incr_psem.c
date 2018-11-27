#include <semaphore.h>
#include <pthread.h>
#include "tlpi_hdr.h"
#include <stdlib.h>

static int glob = 0;
static sem_t sem;

static void *                   /* Loop 'arg' times incrementing 'glob' */
threadFunc(void *arg)
{
    int loops = *((int *) arg);
    int loc, j;

    for (j = 0; j < loops; j++) {
        if (sem_wait(&sem) == -1)
            errExit("sem_wait");

        loc = glob;
        loc++;
        glob = loc;

        if (sem_post(&sem) == -1)
            errExit("sem_post");
    }

    return NULL;
}

int
main(int argc, char *argv[])
{

    //pthread_t t1, t2;
    int num_threads;
    char* p;
    long conv = strtol(argv[2], &p, 10);
    num_threads = conv;
    //printf("num_threads: %i\n", num_threads);
    pthread_t threads[num_threads];
    int loops, s;

    loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000;

    /* Initialize a semaphore with the value 1 */

    if (sem_init(&sem, 0, 1) == -1)
        errExit("sem_init");


    /* Create two threads that increment 'glob' */
    for(int i = 0; i < num_threads; ++i){
   	s = pthread_create(&threads[i], NULL, threadFunc, &loops);	
	if(s != 0)
		return -1;
    }


    /* s = pthread_create(&t1, NULL, threadFunc, &loops); */
    /* if (s != 0) */
    /*     errExitEN(s, "pthread_create"); */
    /* s = pthread_create(&t2, NULL, threadFunc, &loops); */
    /* if (s != 0) */
    /*     errExitEN(s, "pthread_create"); */

    /* Wait for threads to terminate */

    for(int i = 0; i < num_threads; ++i){
	s = pthread_join(threads[i], NULL);
	if(s != 0)
		return -1;
    }

    /* s = pthread_join(t1, NULL); */
    /* if (s != 0) */
    /*     errExitEN(s, "pthread_join"); */
    /* s = pthread_join(t2, NULL); */
    /* if (s != 0) */
    /*     errExitEN(s, "pthread_join"); */

    printf("glob = %d\n", glob);
    exit(EXIT_SUCCESS);
}
