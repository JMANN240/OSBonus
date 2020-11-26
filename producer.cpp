#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h> 
#include <cstring>
using namespace std; 

#define MAX_ITEMS 2

struct shmseg {
   int counter;
   int table[MAX_ITEMS];
};

int main() 
{
    int shmid = shmget(0x1234,sizeof(shmseg),0666|IPC_CREAT); 
    struct shmseg *shmem = (struct shmseg*) shmat(shmid,NULL,0); 
    sem_t* sem = sem_open("/sem", O_CREAT);
    int newInt = 7;

    shmem->counter = 0;

    while(true) {
        while(shmem->counter == MAX_ITEMS);
        sem_wait(sem);
        shmem->table[shmem->counter] = newInt;
        sem_post(sem);
        shmem->counter++;
        std::cout << shmem->counter << std::endl;
        usleep(1000000);
    }
    return 0; 
} 