#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int average;
int size = 0;

void *calcAvg(void *arg);
int numbers[] = {0,1,2,3,4,5,6,7};

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t even = PTHREAD_COND_INITIALIZER;
pthread_cond_t odd = PTHREAD_COND_INITIALIZER;

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
  int *nums = (int*)malloc((numSize)*sizeof(int));
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
  pthread_cond_destroy(&even);
  pthread_cond_destroy(&odd);
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


// Print even numbers
void *print_even(void *arg)
{
  int *arr_p = (int *)arg;
  while(count < size)
  {
   	// Lock mutex and then wait for signal to relase mutex
   	pthread_mutex_lock( &count_mutex );
<<<<<<< HEAD
	//Keep looping till the condition is odd 
   	while( arr_p[count] % 2 == 1 )  
     		pthread_cond_wait( &even, &count_mutex); //Waiting till even condition becomes true
	printf("Array value print_even: %d\n",arr_p[count]);
        count++;
   	pthread_cond_signal( &odd);
=======
	//Keep looping till the condition becomes even
   	while( arr_p[count] % 2 == 1 ) 
     		pthread_cond_wait( &even, &count_mutex ); 
	printf("Array value print_even: %d\n",arr_p[count]);
        count++;
   	pthread_cond_signal( &odd );
>>>>>>> baeb3d035e0740884b60547fe278b5f6a0611291
	pthread_mutex_unlock(&count_mutex);
  }
  pthread_exit(0);
}



// print odd numbers
void *print_odd(void *arg)
{
  int *arr_p = (int*)arg;
  while(count < size)
  {
 	 // Lock mutex and then wait for signal to relase mutex
  	pthread_mutex_lock( &count_mutex );
<<<<<<< HEAD
	// Keep looping till condition is even
  	while( arr_p[count] % 2 == 0 )
    	    	pthread_cond_wait( &odd, &count_mutex ); //waiting till odd becomes true		
=======
	//Keep looping till the condition becomes odd
  	while( arr_p[count] % 2 == 0 )
    	    	pthread_cond_wait( &odd, &count_mutex ); 	
>>>>>>> baeb3d035e0740884b60547fe278b5f6a0611291
	printf("Array Value print_odd: %d\n", arr_p[count]);
	count++;
  	pthread_cond_signal(&even);
	pthread_mutex_unlock(&count_mutex);
  }
  pthread_exit(0);
}



