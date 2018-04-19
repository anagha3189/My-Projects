#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/*int average;

int size = 0;


void *calcAvg(void *arg);
int main(int argc, char *argv[]){
  /* initialize an array of the integers to be passed */
  int *nums = (int*)malloc((argc - 1)*sizeof(int));
  int i = 1;
  for(i = 1; i < argc ; i++){
    nums[i-1] = atoi(argv[i]);
    size++;
  }

  /* Thread Identifier */
  pthread_t avgThread;

  pthread_create(&avgThread, NULL, calcAvg, (void*)nums);

  pthread_join(avgThread, NULL);
  printf("average = %d \n",average);
  free(nums);

}
void *calcAvg(void *arg){
  int *val_p = (int *) arg;
  int sum = 0;
  int i = 0;
  for( i = 0; i < size; i++){
    sum += val_p[i];
  }
  average = sum / (size);
  pthread_exit(0);
} */


pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;
void *functionCount1();
void *functionCount2();
int count = 0;
#define COUNT_DONE 200

void main()
{
 pthread_t thread1, thread2;
 pthread_create( &thread1, NULL, &functionCount1, NULL);
 pthread_create( &thread2, NULL, &functionCount2, NULL);
 pthread_join( thread1, NULL);
 pthread_join( thread2, NULL);
 exit(0);
}

// Print odd numbers
void *functionCount1()
{
  for(;;) {
   // Lock mutex and then wait for signal to relase mutex
   pthread_mutex_lock( &count_mutex );
   if ( count % 2 != 0 ) {
     pthread_cond_wait( &condition_var, &count_mutex );
   }
   count++;
   printf("Counter value functionCount1: %d\n",count);
   pthread_cond_signal( &condition_var );
   if ( count >= COUNT_DONE ) {
     pthread_mutex_unlock( &count_mutex );
     return(NULL);
   }
   pthread_mutex_unlock( &count_mutex );
 }
}

// print even numbers
void *functionCount2()
{
  for(;;) {
  // Lock mutex and then wait for signal to relase mutex
  pthread_mutex_lock( &count_mutex );
  if ( count % 2 == 0 ) {
    pthread_cond_wait( &condition_var, &count_mutex );
  }
  count++;
  printf("Counter value functionCount2: %d\n",count);
  pthread_cond_signal( &condition_var );
  if( count >= COUNT_DONE ) {
    pthread_mutex_unlock( &count_mutex );
    return(NULL);
  }
  pthread_mutex_unlock( &count_mutex );
 }
}

