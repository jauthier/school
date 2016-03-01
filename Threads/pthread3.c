#include <stdio.h>       /* Input/Output */
#include <stdlib.h>      /* General Utilities */
#include <unistd.h>      /* Symbolic Constants */
#include <sys/types.h>   /* Primitive System Data Types */
#include <sys/wait.h>    /* Wait for Process Termination */
#include <errno.h>       /* Errors */
#include <strings.h>     /* Strings */
#include <pthread.h>     /* pthreads library */

#define NUM_THREADS     4

/*
 *   References:
 *   https://computing.llnl.gov/tutorials/pthreads/
 */

void *PrintHello(void *threadid)
{
   long tid;
   tid = (long)threadid;
   int i = 0;
   int result = 0;

   printf("Thread %ld starting...\n", tid);
   for (i=0; i<100*(tid+1); i++) {
      result = result + i;
   }
   printf("Thread %ld done. Result = %d\n",tid, result);
   sleep((tid+1)*5);
   pthread_exit((void*) tid);
}

int main (int argc, char *argv[])
{
   pthread_t threads[NUM_THREADS];
   pthread_attr_t attr;

   int rc;
   long t;
   void *status;
   int answer = 0;

   /* Initialize and set thread detached attribute */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   for ( t=0; t < NUM_THREADS/2; t++ ) {
      printf("In main: creating thread %ld\n", t);
      rc = pthread_create(&threads[t], &attr, PrintHello, (void *)t);
      if (rc) {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }

   /* Free attribute and wait for the other threads */
   pthread_attr_destroy(&attr);
   for ( t=0; t < NUM_THREADS/2; t++ ) {
      rc = pthread_join(threads[t], &status);
      if (rc) {
         printf("ERROR; return code from pthread_join() is %d\n", rc);
         exit(-1);
      }
      printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
      answer = answer + (int)status;
   }

   printf("Main: answer = %d\n",answer);

   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   for ( t=NUM_THREADS/2; t < NUM_THREADS; t++ ) {
      printf("In main: creating thread %ld\n", t);
      rc = pthread_create(&threads[t], &attr, PrintHello, (void *)t);
      if (rc) {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }

   /* Free attribute and wait for the other threads */
   pthread_attr_destroy(&attr);
   for ( t=NUM_THREADS/2; t < NUM_THREADS; t++ ) {
      rc = pthread_join(threads[t], &status);
      if (rc) {
         printf("ERROR; return code from pthread_join() is %d\n", rc);
         exit(-1);
      }
      printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
      answer = answer + (int)status;
   }

   printf("Main: answer = %d\n",answer);

   /* Last thing that main() should do */
   printf("Main: program completed. Exiting.\n");
   pthread_exit(NULL);

}

