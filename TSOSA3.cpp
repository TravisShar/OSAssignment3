#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdint>
using namespace std;

sem_t milk_sem;
sem_t cheese_sem;
sem_t burger_sem;

int num_burgers;
int num_milk;
int num_cheese;
int burger_count = 0;
int milk_count = 0;
int cheese_count = 0;

void *milk_producer(void *param);
void *cheese_producer(void *param);
void *burger_producer(void *param);

int main(int argc, char *argv[]) 
{
    
    // check for proper usage
    /*if (argc != 2) 
    {
        fprintf(stderr, "usage: ./hw3 num_burgers\n");
        exit(1);
    }*/

    // get number of burgers from command line
    //num_burgers = atoi(argv[1]);
    printf("How Many Cheeseburgers? ");
    scanf("%i", &num_burgers);
    

    // compute number of milk bottles and cheese slices needed
    num_milk = 6 * num_burgers;
    num_cheese = 2 * num_burgers;

    // initialize semaphores
    sem_init(&milk_sem, 0, 0);
    sem_init(&cheese_sem, 0, 0);
    sem_init(&burger_sem, 0, 0);

    // create threads
    pthread_t tid1, tid2, tid3, tid4, tid5, tid6;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // create milk producer threads
    for (intptr_t i = 1; i <= 3; i++) 
    {
        pthread_create(&tid1, &attr, milk_producer, (void*)i);
    }

    // create cheese producer threads
    for (intptr_t i = 4; i <= 5; i++) 
    {
        pthread_create(&tid2, &attr, cheese_producer, (void*)i);
    }

    // create burger producer thread
    pthread_create(&tid3, &attr, burger_producer, (void*)6);

    // wait for threads to finish
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    // clean up
    sem_destroy(&milk_sem);
    sem_destroy(&cheese_sem);
    sem_destroy(&burger_sem);

    return 0;
}

// milk producer thread
void *milk_producer(void *param) 
{
    int id = (intptr_t)param; // thread ID

    // produce milk bottles
    for (int i = 0; i < num_milk / 3; i++) 
    {
        printf("Milk %dP: %d\n", id, i);
        sleep(rand() % 5);
    }

    // signal that milk has been produced
    sem_post(&milk_sem);

    pthread_exit(0);
    return 0;
}

// cheese producer thread
void *cheese_producer(void *param) 
{
    int id = (intptr_t)param; // thread ID

    // produce cheese slices
    for (int i = 0; i < num_cheese / 2; i++) 
    {
        printf("Cheese %dP: %d\n", id, i);
        sleep(rand() % 5);
    }

    // signal that cheese has been produced
    sem_post(&cheese_sem);

    pthread_exit(0);
    return 0;
}

// burger producer thread
void *burger_producer(void *param) 
{
    int id = (intptr_t)param; // thread ID

    // produce burgers
    while (burger_count < num_burgers) 
    {
        // wait for milk and cheese to be produced
        sem_wait(&milk_sem);
        sem_wait(&cheese_sem);

        // use milk and cheese to make a burger
        milk_count++;
        cheese_count++;
        burger_count++;

        printf("Burger %dB: %d, %d\n", id, milk_count, cheese_count);

        // signal that a burger has been produced
        sem_post(&burger_sem);
    }
    pthread_exit(0);
    return 0;
}