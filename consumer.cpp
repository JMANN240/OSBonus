#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#define MAX_ITEMS 2 // How many items are allowed on the table at one time

struct shmseg { // Creating the structure of the shared memory segment
   int counter; // Keeps track of how many items are on the table
   int table[MAX_ITEMS]; // Holds the items in shared memory
};

int main() 
{
    int shmid = shmget(0x1234,sizeof(shmseg),0666|IPC_CREAT); // Get a shared memory ID
    struct shmseg *shmem = (struct shmseg*) shmat(shmid,NULL,0);  // Get a pointer to the shared memory block
    sem_t* sem = sem_open("/sem", O_CREAT); // Create a semaphore called /sem

    while(true) { // Repeat forever
        while(shmem->counter == 0); // Wait while the table is empty
        sem_wait(sem); // Call wait on the semaphore to enter critical state
        shmem->table[shmem->counter] = 0; // Turn the table item back to 0 to consume it
        sem_post(sem); // Call post on the semaphore to exit critical state
        shmem->counter--; // Subtract 1 to the counter because we just took an item out of the table
        usleep(1000000); // Wait 1 second
    }
    return 0; 
} 
