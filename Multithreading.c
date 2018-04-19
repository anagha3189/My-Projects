#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int average;
int size = 0;

void *calcAvg(void *arg);
int numbers[] = {0,1,2,3,4,5,6,7};

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;
void *print_even();
void *print_odd();
int count = 0;

// This is useful when the elements are not provided via array
#define COUNT_DONE 200


int main(int argc, char *argv[])
{
  if(argc < 1)
	exit(0);

  //initialize an array of the integers to be passed
  int numSize = atoi(argv[1]); 
  srand(time(NULL));
  int *nums = (int*)malloc((numSize - 1)*sizeof(int));
  int i ;
  for(i = 0; i < numSize ; i++)
  {
    nums[i] = (rand() % 100) + i ;
    printf("%d   " , nums[i]);
    size++;
  }
  printf("\n");
  //Thread Identifier 
  //pthread_t avgThread;
  //pthread_create(&avgThread, NULL, calcAvg, (void*)nums);
  pthread_t even_thread;
  pthread_t odd_thread;
  pthread_create(&even_thread, NULL, print_even , (void*)nums);
  pthread_create(&odd_thread, NULL, print_odd, (void*)nums);

  //pthread_join(avgThread, NULL);
  //printf("average = %d \n",average);
  pthread_join(even_thread, NULL); 
  pthread_join(odd_thread, NULL);
  pthread_mutex_destroy(&count_mutex);
  pthread_cond_destroy(&condition_var);
  free(nums);

  return 0;

}



void *calcAvg(void *arg)
{
  int *val_p = (int *) arg;
  int sum = 0;
  int i = 0;
  for( i = 0; i < size; i++){
    sum += val_p[i];
  }
  average = sum / (size);
  pthread_exit(0);
} 


// Print odd numbers
void *print_odd(void *arg)
{
  int *arr_p = (int *)arg;
  while(count < size)
  {
   	// Lock mutex and then wait for signal to relase mutex
   	pthread_mutex_lock( &count_mutex );
   	while( arr_p[count] % 2 != 0 ) 
   	{ 
     		pthread_cond_wait( &condition_var, &count_mutex );
   	}
   	count++;
	printf("Array value print_odd: %d\n",arr_p[count]);
       	pthread_mutex_unlock( &count_mutex );
   	pthread_cond_signal( &condition_var );
  }
  pthread_exit(0);
}



// print even numbers
void *print_even(void *arg)
{
  int *arr_p = (int*)arg;
  while(count < size)
  {
 	 // Lock mutex and then wait for signal to relase mutex
  	pthread_mutex_lock( &count_mutex );
  	while( arr_p[count] % 2 == 0 )
	{
    	    pthread_cond_wait( &condition_var, &count_mutex );
  	}
  	count++;
  	printf("Array Value print_even: %d\n", arr_p[count]);
	pthread_mutex_unlock(&count_mutex);
  	pthread_cond_signal( &condition_var );
  }
  pthread_exit(0);
}



