
/* Code for Mutex */

gcc filename.c -lpthread


// From OS Point of view.

 int gtthread_mutex_init(gtthread_mutex_t *mutex) {
	if(mutex->lock == 1) {
		return -1;
	}
	// Set lock to 0 and parent to -1 as per man aq1
	mutex->lock = 0;
	mutex->parent_tid = -1;
	return 0;
}

int gtthread_mutex_lock(gtthread_mutex_t *mutex) {
	gtthread_t *thread = TAILQ_FIRST(&queue_head);
	//If the mutex lock is already acquired by the thread
	if(mutex->parent_tid == thread->tid) {
		return -1;
	}
	
	// Spinlock, keep yielding till lock is acquired with correct parent
	//Stopping condition => mutex->lock = 0 or mutex->parent_tid = thread->tid
	while(mutex->lock !=0 && mutex->parent_tid != thread->tid) 
	{
		gtthread_yield();
	}
	
	// Houston, we have a lock
	mutex->lock = 1;
	mutex->parent_tid = thread->tid;
	return 0;
}

int gtthread_mutex_unlock(gtthread_mutex_t *mutex) 
{
	gtthread_t *thread = TAILQ_FIRST(&queue_head);
	if(mutex->lock == 1 && mutex->parent_tid == thread->tid) {
		// Set lock to 0 and parent to -1 as per man page
		mutex->lock = 0;
		mutex->parent_tid = -1;
		return 0;
	} 
	else 
	{
		return -1;
	}
}


// Our swapping/scheduling function
void gtthread_next() {
	// Swap context from the head of the queue to the next
	gtthread_t *head_thread = TAILQ_FIRST(&queue_head);
	gtthread_t *next = TAILQ_NEXT(head_thread, entries);
	swapcontext(head_thread->context, next->context);

	// Move the head to the tail of the queue
	TAILQ_REMOVE(&queue_head, head_thread, entries);
	TAILQ_INSERT_TAIL(&queue_head, head_thread, entries);
	if(DEBUG_MESSAGES) {
		// The following is very spammy, but demonstrates round-robin scheduling
		// of threads very well. Uncomment to enable:
		//printf("Swapped thread id %d with %d\n", head_thread->tid, next->tid);
	}
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------

// From firmware point of view

//Basic periodic sceduler

uint32_t Counter;
#define NUM 10
void (*PeriodTask1)(void);

void scheduler()
{
	if(++counter == NUM)
	{
		(*PeriodTask1)();
		Counter = 0;
	}
	RunPt = RunPt->next;  // Round Robin
}

//Periodic threads
void (*PerThread0_Pt)(void);
void (*PerThread1_Pt)(void);
int32_t PerThread0_Period, PerThread1_Period;

int OS_AddPeriodicEventThreads(void(*thread1)(void), uint32_t period1, void(*thread2)(void), uint32_t period2)
{
	PerThread0_Pt = thread1;
	PerThread1_Pt = thread2;		
	PerThread0_Period = period1;
	PerThread1_Period = period2;
	return 1;
}

void Scheduler(void){ // every time slice
// ROUND ROBIN
	static int32_t Counter = 0;
	//Assigning couter to least common multiple of Periods mentioned.
	Counter = (Counter + 1)%(PerThread0_Period * PerThread1_Period);
	// run any periodic event threads if needed
	if ((Counter % PerThread0_Period) == 0)
		(*PerThread0_Pt)();	//Run periodic thread, every PerThread0_Period ms
	if ((Counter % PerThread1_Period) == 0)
		(*PerThread1_Pt)();	//Run periodic thread1, every PerThread1_Period ms
  //implement round robin scheduler, update RunPt
	RunPt = RunPt->next;  // Round Robin
}



***************************************************************************************************************************

//for regular threads
struct tcb{
	  int32_t *sp;      // pointer to stack (valid for threads not running
	  struct tcb *next; // linked-list pointer
	  int32_t *blocked;	// pointer to blocked semaphore, nonzero if blocked on this semaphore
	  int32_t sleep;	// time to sleep, nonzero if this thread is sleeping
	  uint32_t priority; // priority of the thread, 0 - highest priority, 254 - lowest
};
	
typedef struct tcb tcbType;
tcbType tcbs[NUMTHREADS];
tcbType *RunPt;
int32_t Stacks[NUMTHREADS][STACKSIZE];
	
	//initialize TCB circular list
	tcbs[0].next = &tcbs[1];	//main thread 0 points to main thread 1
	tcbs[1].next = &tcbs[2];	//main thread 1 points to main thread 2
	tcbs[2].next = &tcbs[3];	//main thread 2 points to main thread 3	
	tcbs[3].next = &tcbs[4];	//main thread 3 points to main thread 4
	tcbs[4].next = &tcbs[5];	//main thread 4 points to main thread 5
	tcbs[5].next = &tcbs[6];	//main thread 5 points to main thread 6
	tcbs[6].next = &tcbs[7];	//main thread 6 points to main thread 7
	tcbs[7].next = &tcbs[0];	//main thread 7 points to main thread 0
	//initialize threads as not blocked									
	for(i=0; i< NUMTHREADS; i++)
	{tcbs[i].blocked = 0;}
	
	// initialize RunPt
	RunPt = &tcbs[0];
	
	//Setting stack for each thread
	......
	
	//initialize priority for each thread
	
SysTick_Handler                ; 1) Saves R0-R3,R12,LR,PC,PSR
    CPSID   I                  ; 2) Prevent interrupt during switch
    PUSH    {R4-R11}           ; 3) Save remaining regs r4-11
    LDR     R0, =RunPt         ; 4) R0=pointer to RunPt, old thread
    LDR     R1, [R0]           ;    R1 = RunPt
    STR     SP, [R1]           ; 5) Save SP into TCB
;    LDR     R1, [R1,#4]        ; 6) R1 = RunPt->next
;    STR     R1, [R0]           ;    RunPt = R1
    PUSH    {R0,LR}
    BL      Scheduler
    POP     {R0,LR}
    LDR     R1, [R0]           ; 6) R1 = RunPt, new thread
    LDR     SP, [R1]           ; 7) new thread SP; SP = RunPt->sp;
    POP     {R4-R11}           ; 8) restore regs r4-11
    CPSIE   I                  ; 9) tasks run with interrupts enabled
    BX      LR                 ; 10) restore R0-R3,R12,LR,PC,PSR

StartOS
    LDR     R0, =RunPt         ; currently running thread
    LDR     R2, [R0]           ; R2 = value of RunPt
    LDR     SP, [R2]           ; new thread SP; SP = RunPt->stackPointer;
    POP     {R4-R11}           ; restore regs r4-11
    POP     {R0-R3}            ; restore regs r0-3
    POP     {R12}
    ADD     SP,SP,#4           ; discard LR from initial stack
    POP     {LR}               ; start location
    ADD     SP,SP,#4           ; discard PSR
    CPSIE   I                  ; Enable interrupts at processor level
    BX      LR                 ; start first thread

    ALIGN
    END

	
void SetInitialStack(int i){
  // ****IMPLEMENT THIS**** 
  // **Same as Lab 2 and Lab 3****
  tcbs[i].sp = &Stacks[i][STACKSIZE-16]; // thread stack pointer
  Stacks[i][STACKSIZE-1] = 0x01000000; // Thumb bit 
  Stacks[i][STACKSIZE-3] = 0x14141414; // R14 
  Stacks[i][STACKSIZE-4] = 0x12121212; // R12 
  Stacks[i][STACKSIZE-5] = 0x03030303; // R3 
  Stacks[i][STACKSIZE-6] = 0x02020202; // R2 
  Stacks[i][STACKSIZE-7] = 0x01010101; // R1 
  Stacks[i][STACKSIZE-8] = 0x00000000; // R0 
  Stacks[i][STACKSIZE-9] = 0x11111111; // R11 
  Stacks[i][STACKSIZE-10] = 0x10101010; // R10 
  Stacks[i][STACKSIZE-11] = 0x09090909; // R9 
  Stacks[i][STACKSIZE-12] = 0x08080808; // R8 
  Stacks[i][STACKSIZE-13] = 0x07070707; // R7 
  Stacks[i][STACKSIZE-14] = 0x06060606; // R6 
  Stacks[i][STACKSIZE-15] = 0x05050505; // R5 
  Stacks[i][STACKSIZE-16] = 0x04040404; // R4 
 
}


	
int OS_AddThreads(void(*thread0)(void), uint32_t p0,
                  void(*thread1)(void), uint32_t p1,
                  void(*thread2)(void), uint32_t p2,
                  void(*thread3)(void), uint32_t p3,
                  void(*thread4)(void), uint32_t p4,
                  void(*thread5)(void), uint32_t p5,
                  void(*thread6)(void), uint32_t p6,
                  void(*thread7)(void), uint32_t p7)
{
// **similar to Lab 3. initialize priority field****
	int32_t status;
	status = StartCritical();
										
	tcbs[0].next = &tcbs[1]; // 0 points to 1
	tcbs[1].next = &tcbs[2]; // 1 points to 2
	tcbs[2].next = &tcbs[3]; // 2 points to 3
	tcbs[3].next = &tcbs[4]; // 3 points to 4
	tcbs[4].next = &tcbs[5]; // 4 points to 5
	tcbs[5].next = &tcbs[6]; // 5 points to 6
	tcbs[6].next = &tcbs[7]; // 6 points to 7
	tcbs[7].next = &tcbs[0]; // 7 points to 0		
										
	SetInitialStack(0); 
	Stacks[0][STACKSIZE-2] = (int32_t)(thread0); // PC to address of thread
	tcbs[0].blocked = 0; 
	tcbs[0].sleep = 0; 
	tcbs[0].priority = p0;
										
	SetInitialStack(1); Stacks[1][STACKSIZE-2] = (int32_t)(thread1); // PC
	tcbs[1].blocked = 0; tcbs[1].sleep = 0; tcbs[1].priority = p1;  
										
	SetInitialStack(2); Stacks[2][STACKSIZE-2] = (int32_t)(thread2); // PC
	tcbs[2].blocked = 0; tcbs[2].sleep = 0; tcbs[2].priority = p2;  	
										
	SetInitialStack(3); Stacks[3][STACKSIZE-2] = (int32_t)(thread3); // PC	
	tcbs[3].blocked = 0; tcbs[3].sleep = 0; tcbs[3].priority = p3;  	
	
	SetInitialStack(4); Stacks[4][STACKSIZE-2] = (int32_t)(thread4); // PC
	tcbs[4].blocked = 0; tcbs[4].sleep = 0; tcbs[4].priority = p4;  
	
	SetInitialStack(5); Stacks[5][STACKSIZE-2] = (int32_t)(thread5); // PC
	tcbs[5].blocked = 0; tcbs[5].sleep = 0; tcbs[5].priority = p5;
 	
	SetInitialStack(6); Stacks[6][STACKSIZE-2] = (int32_t)(thread6); // PC
	tcbs[6].blocked = 0; tcbs[6].sleep = 0; tcbs[6].priority = p6;  
	
	SetInitialStack(7); Stacks[7][STACKSIZE-2] = (int32_t)(thread7); // PC
	tcbs[7].blocked = 0; tcbs[7].sleep = 0; tcbs[7].priority = p7;  		
										
	RunPt = &tcbs[0];       // thread 0 will run first
	
	EndCritical(status);
 
  return 1;               // successful
}	


// preemptive priority scheduler 
void Scheduler(void){      // every time slice
	// look at all threads in TCB list choose
	// highest priority thread not blocked and not sleeping 
	// If there are multiple highest priority (not blocked, not sleeping) run these round robin
	// ROUND ROBIN, skip blocked and sleeping threads
	uint32_t maxprio = 255;
	tcbType *tempPt;
	tcbType *bestPt;
	tempPt = RunPt;
	
	do {
		tempPt = tempPt->next;
		if(((tempPt->priority) < maxprio) && (tempPt->blocked == 0) && (tempPt->sleep == 0)) 
		{ 
			//If priority is higher and not blocked and not sleeping
			maxprio = tempPt->priority;
			bestPt = tempPt;
		}
		
	} while (RunPt != tempPt); //search through all linked list
	
	RunPt = bestPt; //move to next suitable thread
}


	
void OS_InitSemaphore(int32_t *semaPt, int32_t value){
	*semaPt = value; 
}

void OS_Wait(int32_t *semaPt){
	DisableInterrupts();
	*semaPt = *semaPt - 1;
	if((*semaPt) < 0) {
		RunPt->blocked = semaPt; // reason it is blocked
		EnableInterrupts();
		OS_Suspend(); // run thread switcher
	}
	EnableInterrupts();  
}

void OS_Signal(int32_t *semaPt){
	tcbType *pt;
	DisableInterrupts();
	*semaPt = *semaPt + 1;
	if((*semaPt) <= 0){
		pt = RunPt->next;
		while(pt->blocked != semaPt){
			pt = pt->next; // search for a thread blocked on this semaphore
		}
		pt->blocked = 0; // wakeup this one
	}
	EnableInterrupts();  
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

//FIFO for single producer and single consumer 

#define FSIZE 10    // can be any size //FIFO SIZE
uint32_t PutI;      // index of where to put next
uint32_t GetI;      // index of where to get next
uint32_t Fifo[FSIZE];
int32_t CurrentSize;// 0 means FIFO empty, FSIZE means full
uint32_t LostData;  // number of lost pieces of data

void OS_FIFO_Init(void){ //Init the FIFO with indexes and CurrentSize and LostData set to 0
	PutI = 0;
	GetI = 0;
	OS_InitSemaphore(&CurrentSize,0);
}

int OS_FIFO_Put(uint32_t data){
	if(CurrentSize == FSIZE) 
	{ //FIFO is full
		LostData++;
		return -1; //Error
	}
	else 
	{
		Fifo[PutI] = data;	//store data in FIFO at put index
		PutI = (PutI + 1)%FSIZE; //Increment Put index and wrap around if necessary
		OS_Signal(&CurrentSize);
		return 0;	//Success
	}
}

uint32_t OS_FIFO_Get(void)
{
	uint32_t data;
	OS_Wait(&CurrentSize);	//Wait till there is data in FIFO, block if empty
	data = Fifo[GetI];	//Get stored data from Fifo
	GetI = (GetI + 1) % FSIZE;	//Incremet Get index and wrap around
    return data;
}



// FIFO for many producers and many consumers with bounded buffer
#define FSIZE 10    // can be any size //FIFO SIZE
uint32_t volatile *PutI;      // index of where to put next
uint32_t volatile *GetI;      // index of where to get next
uint32_t static Fifo[FSIZE];
int32_t CurrentSize;// 0 means FIFO empty
int32_t RoomLeft // 0 means FIFO full
int32_t FIFOmutex // Exclusive access to FIFO
uint32_t LostData;  // number of lost pieces of data

void OS_FIFO_Init(void)
{ 
	//Init the FIFO with indexes and CurrentSize and LostData set to 0
	PutI = GetPt = &Fifo[0];
	OS_InitSemaphore(&CurrentSize,0);
	OS_InitSemaphore(&Roomleft,FIFOSIZE);
	OS_InitSemaphore(&FIFOMutex,1);
}

int OS_FIFO_Put(uint32_t data){
	OS_Wait(&Roomleft);
	OS_Wait(&FIFOmutex);
	*PutI = data;	//store data in FIFO at put index
	PutI++;
	if(PutI == &Fifo[FSIZE]) // If full move back to 0
		PutI= &Fifo[0];
	OS_signal(&FIFOmutex);
	OS_Signal(&CurrentSize);
	return 0;	//Success
}

uint32_t OS_FIFO_Get(void)
{
	uint32_t data;
	OS_Wait(&CurrentSize);
	OS_Wait(&FIFOmutex);
	data = *GetI;	//Get stored data from Fifo
	GetI++;
	if(GetI == &Fifo[FSIZE]) //if empty move back to 0
		GetI= &Fifo[0];
	OS_signal(&FIFOmutex);
	OS_Signal(&RoomLeft);
	return data;
}


// FIFO for one producer and many consumers with bounded buffer


#define FSIZE 10    // can be any size //FIFO SIZE
uint32_t volatile *PutI;      // index of where to put next
uint32_t volatile *GetI;      // index of where to get next
uint32_t static Fifo[FSIZE];
int32_t CurrentSize;// 0 means FIFO empty
int32_t FIFOmutex // Exclusive access to FIFO
uint32_t LostData;  // number of lost pieces of data

void OS_FIFO_Init(void){ //Init the FIFO with indexes and CurrentSize and LostData set to 0
	PutI = GetPt = &Fifo[0];
	OS_InitSemaphore(&CurrentSize,0);
	OS_InitSemaphore(&FIFOMutex,1);
	LostData = 0 ;
}

int OS_FIFO_Put(uint32_t data)
{
	if(CurrentSize == FSIZE) 
	{ //FIFO is full
		LostData++;
		return -1; //Error
	}
	OS_Wait(&FIFOmutex);
	*PutI = data;
	PutI++;
	if(PutI == &Fifo[FSIZE])
		PutI= &Fifo[0];
	OS_Wait(&FIFOmutex);
	OS_Signal(&CurrentSize);
	return 0;	//Success
	}
}

uint32_t OS_FIFO_Get(void){uint32_t data;
	OS_Wait(&CurrentSize);
	OS_Wait(&FIFOmutex);
	data = *GetI;	//Get stored data from Fifo
	GetI++;
	if(GetI == &Fifo[FSIZE])
		GetI= &Fifo[0];
	OS_signal(&FIFOmutex);
	return data;
}



--------------------------------------------------------------------------------------------------------------------------------------

//Producer consumer bounded buffer 

CODE:
/* buffer.h */
typedef int buffer_item;
#define BUFFER_SIZE 5

/* main.c */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"

#define RAND_DIVISOR 100000000
#define TRUE 1

/* The mutex lock */
pthread_mutex_t mutex;

/* the semaphores */
sem_t full, empty;

/* the buffer */
buffer_item buffer[BUFFER_SIZE];

/* buffer counter */
int counter;

pthread_t tid;       //Thread ID
pthread_attr_t attr; //Set of thread attributes

void *producer(void *param); /* the producer thread */
void *consumer(void *param); /* the consumer thread */

void initializeData() {

   /* Create the mutex lock */
   pthread_mutex_init(&mutex, NULL);

   /* Create the full semaphore and initialize to 0 */
   sem_init(&full, 0, 0);
   /* Create the empty semaphore and initialize to BUFFER_SIZE */
   sem_init(&empty, 0, BUFFER_SIZE);

   /* Get the default attributes */
   pthread_attr_init(&attr);

   /* init buffer */
   counter = 0;
}

/* Producer Thread */
void *producer(void *param) {
   buffer_item item;

   while(TRUE) {
      /* sleep for a random period of time */
      int rNum = rand() / RAND_DIVISOR;
      sleep(rNum);

      /* generate a random number */
      item = rand();

      /* acquire the empty lock */
      sem_wait(&empty);
      /* acquire the mutex lock */
      pthread_mutex_lock(&mutex);

      if(insert_item(item)) {
         fprintf(stderr, " Producer report error condition\n");
      }
      else {
         printf("producer produced %d\n", item);
      }
      /* release the mutex lock */
      pthread_mutex_unlock(&mutex);
      /* signal full */
      sem_post(&full);
   }
}

/* Consumer Thread */
void *consumer(void *param) {
   buffer_item item;

   while(TRUE) {
      /* sleep for a random period of time */
      int rNum = rand() / RAND_DIVISOR;
      sleep(rNum);

      /* aquire the full lock */
      sem_wait(&full);
      /* aquire the mutex lock */
      pthread_mutex_lock(&mutex);
      if(remove_item(&item)) {
         fprintf(stderr, "Consumer report error condition\n");
      }
      else {
         printf("consumer consumed %d\n", item);
      }
      /* release the mutex lock */
      pthread_mutex_unlock(&mutex);
      /* signal empty */
      sem_post(&empty);
   }
}

/* Add an item to the buffer */
int insert_item(buffer_item item) {
   /* When the buffer is not full add the item
      and increment the counter*/
   if(counter < BUFFER_SIZE) {
      buffer[counter] = item;
      counter++;
      return 0;
   }
   else { /* Error the buffer is full */
      return -1;
   }
}

/* Remove an item from the buffer */
int remove_item(buffer_item *item) {
   /* When the buffer is not empty remove the item
      and decrement the counter */
   if(counter > 0) {
      *item = buffer[(counter-1)];
      counter--;
      return 0;
   }
   else { /* Error buffer empty */
      return -1;
   }
}

int main(int argc, char *argv[]) {
   /* Loop counter */
   int i;

   /* Verify the correct number of arguments were passed in */
   if(argc != 4) {
      fprintf(stderr, "USAGE:./main.out <INT> <INT> <INT>\n");
   }

   int mainSleepTime = atoi(argv[1]); /* Time in seconds for main to sleep */
   int numProd = atoi(argv[2]); /* Number of producer threads */
   int numCons = atoi(argv[3]); /* Number of consumer threads */

   /* Initialize the app */
   initializeData();

   /* Create the producer threads */
   for(i = 0; i < numProd; i++) {
      /* Create the thread */
      pthread_create(&tid,&attr,producer,NULL);
    }

   /* Create the consumer threads */
   for(i = 0; i < numCons; i++) {
      /* Create the thread */
      pthread_create(&tid,&attr,consumer,NULL);
   }

   /* Sleep for the specified amount of time in milliseconds */
   sleep(mainSleepTime);

   /* Exit the program */
   printf("Exit the program\n");
   exit(0);
}
OUTPUT:
lee@isis:~/programming/c/producer-consumer$ ./pc.out 10 10 10
producer produced 35005211
consumer consumed 35005211
producer produced 1726956429
consumer consumed 1726956429
producer produced 278722862
consumer consumed 278722862
producer produced 468703135
producer produced 1801979802
producer produced 635723058
producer produced 1125898167
consumer consumed 1125898167
Exit the program

--------------------------------------------------------------------------------------------------------------------------------------------
//Readers and writers problem

#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

sem_t readCountAccess;
sem_t databaseAccess;
int readCount=0;

void *Reader(void *arg);
void *Writer(void *arg);

int main()
{
	int i=0,NumberofReaderThread=0,NumberofWriterThread;

	sem_init(&readCountAccess,0,1);
	sem_init(&databaseAccess,0,1);

	pthread_t Readers_thr[100],Writer_thr[100];
	printf(“\nEnter number of Readers thread(MAX 10)”);
	scanf(“%d”,&NumberofReaderThread);
	printf(“\nEnter number of Writers thread(MAX 10)”);
	scanf(“%d”,&NumberofWriterThread);

	for(i=0;i<numberofreaderthread;i++)
	pthread_create(&Readers_thr[i],NULL,Reader,(void *)i);

	for(i=0;i<numberofwriterthread;i++)
	pthread_create(&Writer_thr[i],NULL,Writer,(void *)i);

	for(i=0;i<NumberofWriterThread;i++)
	pthread_join(Writer_thr[i],NULL);

	for(i=0;i<NumberofReaderThread;i++)
	pthread_join(Readers_thr[i],NULL);

	sem_destroy(&databaseAccess);
	sem_destroy(&readCountAccess);
	return 0;
}

void * Writer(void *arg)
{
	sleep(1);
	int temp=(int)arg;
	printf(“\nWriter %d is trying to enter into database for modifying the data”,temp);
	sem_wait(&databaseAccess);
	printf(“\nWriter %d is writting into the database”,temp);
	printf(“\nWriter %d is leaving the database”);
	sem_post(&databaseAccess);
}

void *Reader(void *arg)
{
	sleep(1);
	int temp=(int)arg;
	printf(“\nReader %d is trying to enter into the Database for reading the data”,temp);
	sem_wait(&readCountAccess);
	readCount++;
	if(readCount==1)
	{
	sem_wait(&databaseAccess);
	printf(“\nReader %d is reading the database”,temp);
	}
	sem_post(&readCountAccess);
	
	sem_wait(&readCountAccess);
	readCount–-;
	if(readCount==0)
	{
	printf(“\nReader %d is leaving the database”,temp);
	sem_post(&databaseAccess);
	}
	sem_post(&readCountAccess);
}

OUTPUT
Enter number of Readers thread(MAX 10)2
Enter number of Writers thread(MAX 10)1

Reader 0 is trying to enter into the Database for reading the data
Reader 0 is reading the database
Reader 0 is leaving the database
Reader 1 is trying to enter into the Database for reading the data
Reader 1 is reading the database
Reader 1 is leaving the database
Writer 0 is trying to enter into database for modifying the data
Writer 0 is writing into the database
Writer 0 is leaving the database

-----------------------------------------------------------------------------------------------------------------------------------------------

Linux: Process Memory Sharing in [C] // Shared Memory

This code specifically prints out the Fibonacci sequence.
The parent process creates and attaches a shared memory segment, forks a child process (remember the child process shares the resources of the parent) , waits for the child to finish, prints out the contents of the shared memory, and finally detaches and removes the shared memory segment.
The child process runs through the Fibonacci algorithm, inserts the resultant into the next available space in shared memory and prints out the resultant to standard output.
This example accepts input from the command line with basic error checking. You know, make sure two args have been provided, they are between the min and max limits of the program; the BASICS.
CODE:

#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_SEQUENCE 10 // Max values to store in shared memory
#define MIN_SEQUENCE 2  // Min value the user can enter
//shared memory:
// 1) holds an array of numbers
// 2) holds how many numbers are in the array
typedef struct 
{
	int fib_seq[MAX_SEQUENCE];
	int sequence_size;
} shared_data;


//MAIN function
int main(int argc, char *argv[]) 
{
	pid_t pid;    //process ID
	int segment_id; //Shared Memory ID
	shared_data *mem; //Shared Memory Pointer
	
	
	//check to validate atleast two arguments
	if(argc != 2) 
	{
	printf(“USAGE ERROR: [0-9]\n”);
	exit(0);
	}
	//validate the input is not larger then the MAX
	if(atoi(argv[1]) > MAX_SEQUENCE) 
	{
	printf(“Max Input Size: %d\n”, MAX_SEQUENCE);
	exit(0);
	}
	//validate the input is not smaller then the MIN
	if(atoi(argv[1]) < MIN_SEQUENCE) 
	{
	printf(“Min Input Size: %d\n”, MIN_SEQUENCE);
	exit(0);
	}
	// 1) create a new shared memory location ‘IPC_PRIVATE’
	// 2) the size of our shared memory structure ‘sizeof(shared_data)’
	// 3) Set Modes S_IRUSR and S_IWUSR so the owner can read and write to the shared memory ‘S_IRUSR|S_IWUSR’
	segment_id = shmget(IPC_PRIVATE, sizeof(shared_data), S_IRUSR|S_IWUSR);
	//attach the shared memory and get the pointer to the beginning location in memory
	mem = (shared_data *) shmat(segment_id,NULL,0);
	//set the size of the sequence to the argument that was passed in via command line
	mem->sequence_size = atoi(argv[1]);
	// fork a child process
	pid = fork();
	if(pid < 0) 
	{ /* error occured */
		fprintf(stderr, “Fork Failed\n”);
		return 1;
	}
	else if(pid == 0) 
	{ 
		/* child process */
		int counter = 0;
		printf(“Child Fibonacci Sequence: “);
		while(counter < mem->sequence_size) 
		{
			if(counter == 0)
			{
				//FIB of zero is always zero
				mem->fib_seq[counter] = 0;
			}
			else if(counter == 1)
			{
				//FIB of one is always one
				mem->fib_seq[counter] = 1;
			}
			else 
			{
				mem->fib_seq[counter] = mem->fib_seq[counter – 1] + mem->fib_seq[counter – 2];
		}
		printf(“%d “, mem->fib_seq[(counter)]);
		counter++;
		}
	}
	else 
	{ 
		/* parent process */
		/* parent will wait for the child process to complete */
		wait(NULL);
		//Print out shared memory
		int count = 0;
		printf(“\nParent Fibonacci Sequence: “);
		while(count < mem->sequence_size)
		{
			printf(“%d “, mem->fib_seq[count]);
			count++;
		}
		//detach shared memory
		shmdt(mem);
		//remove shared memory segment
		shmctl(segment_id,IPC_RMID,NULL);
		printf(“\nComplete\n”);
	}
	return 0;
}



output:

lee@isis:~$ ./a.out 0
Min Input Size: 2
lee@isis:~$ ./a.out 1
Min Input Size: 2
lee@isis:~$ ./a.out 2
Child Fibonacci Sequence: 0 1
Parent Fibonacci Sequence: 0 1
Complete
lee@isis:~$ ./a.out 3
Child Fibonacci Sequence: 0 1 1
Parent Fibonacci Sequence: 0 1 1
Complete
lee@isis:~$ ./a.out 4
Child Fibonacci Sequence: 0 1 1 2
Parent Fibonacci Sequence: 0 1 1 2
Complete
lee@isis:~$ ./a.out 5
Child Fibonacci Sequence: 0 1 1 2 3
Parent Fibonacci Sequence: 0 1 1 2 3
Complete



-------------------------------------------------------------------------------------------------------------------------------------------------------

Condition variables allow threads to synchronize to a value of a shared resource. Typically, condition variables are used as a notification system between threads. 

For example, you could have a counter that once reaching a certain count, you would like for a thread to activate. The thread (or threads) that activates once the counter reaches the limit would wait on the condition variable. Active threads signal on this condition variable to notify other threads waiting/sleeping on this condition variable; thus causing a waiting thread to wake. You can also use a broadcast mechanism if you want to signal all threads waiting on the condition variable to wakeup. Conceptually, this is modeled by the figure on the right with pseudocode. 

//Print even and odd numbers using threads

#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"

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
   if ( count % 2 != 0 ) 
   {
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

