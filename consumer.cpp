#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

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

    int newInt = 0;

    while(true) {
        while(shmem->counter == 0);
        sem_wait(sem);
        shmem->table[shmem->counter] = 0;
        sem_post(sem);
        shmem->counter--;
        usleep(1000000);
    }
    return 0; 
} 