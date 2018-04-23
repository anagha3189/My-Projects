#include <stdio.h>
#include <time.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  even  = PTHREAD_COND_INITIALIZER;
pthread_cond_t  odd   = PTHREAD_COND_INITIALIZER;

void *printfun1(void *pnt);
void *printfun2(void *pnt);

int main(void)
{
    pthread_t pthread1, pthread2;
    int ret1, ret2;

    ret1 = pthread_create(&pthread1, NULL, printfun1, NULL);

    if(ret1)
    {
        printf("thread creation failed");
    }
    ret2 = pthread_create(&pthread2, NULL, printfun2, NULL);
    if(ret2)
    {
        printf("thread creation failed");
    }

    pthread_join(pthread1, NULL);
    pthread_join(pthread2, NULL);
}

int counter = 0;

void *printfun1(void *ptr)
{
    while(counter < 50)
    {
        pthread_mutex_lock(&mutex);
        while ((counter & 1) == 1)
            pthread_cond_wait(&even, &mutex);

        printf("Even -> %d \n", counter);
        counter++;
        pthread_cond_signal(&odd);
        pthread_mutex_unlock(&mutex);

        //usleep( 1000000);
    }
    return NULL;
}

void *printfun2(void *ptr)
{
    while(counter < 50)
    {
        pthread_mutex_lock(&mutex);
        while ((counter & 1) == 0)
            pthread_cond_wait(&odd, &mutex);

        printf("Odd -> %d \n", counter);
        counter++;
        pthread_cond_signal(&even);
        pthread_mutex_unlock(&mutex);

        //usleep( 1000000);
    }
    return NULL;
}
