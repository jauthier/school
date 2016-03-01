#include <stdio.h>       /* Input/Output */
#include <stdlib.h>      /* General Utilities */
#include <unistd.h>      /* Symbolic Constants */
#include <sys/types.h>   /* Primitive System Data Types */
#include <sys/wait.h>    /* Wait for Process Termination */
#include <errno.h>       /* Errors */
#include <strings.h>     /* Strings */
#include <pthread.h>     /* pthreads library */

#define NUM_THREADS     5
/*   
 *   Share me 
 */
int flag = 1;

/*
 *   References:
 *   https://computing.llnl.gov/tutorials/pthreads/
 */

void *PrintHello(void *threadid)
{
   long tid;
   tid = (long)threadid;
   int randomTime;
   time_t t;

   srand((unsigned)time(&t) * tid);
   randomTime = rand() % 10;
   printf("Thread %ld - flag = %d. Sleeping for %d\n", tid,flag,randomTime);
   sleep(randomTime);
   flag++;
   printf("Thread %ld - flag = %d.\n", tid,flag);
   pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
   pthread_t threads[NUM_THREADS];
   pthread_attr_t attr;
   int rc;
   long t;
   void *status;

   /* Initialize and set thread detached attribute */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   printf("In main: flag =  %d\n",flag);

   for(t=0; t<NUM_THREADS; t++){
      rc = pthread_create(&threads[t], &attr, PrintHello, (void *)t);
      if (rc) {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }

   /* Free attribute and wait for the other threads */
   pthread_attr_destroy(&attr);
   for ( t=0; t < NUM_THREADS; t++ ) {
      rc = pthread_join(threads[t], &status);
      if (rc) {
         printf("ERROR; return code from pthread_join() is %d\n", rc);
         exit(-1);
      }
   }
   printf("In main: flag = %d\n",flag);
   /* Last thing that main() should do */
   pthread_exit(NULL);

}

