#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>

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
    int newInt = 7; // The new integer to write to the table

    shmem->counter = 0; // Initialize the counter to 0 because there are no items on the table

    while(true) { // Repeat forever
        while(shmem->counter == MAX_ITEMS); // Wait while the table is full
        sem_wait(sem); // Call wait on the semaphore to enter critical state
        shmem->table[shmem->counter] = newInt; // Put the new integer in the correct table index
        sem_post(sem); // Call post on the semaphore to exit critical state
        shmem->counter++; // Add 1 to the counter because we just put an item on the table
        usleep(1000000); // Wait 1 second
    }
    return 0;
} 
