// Nathan Fleet
// Operating Systems
// 10/31/22

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>

struct data {
    sem_t mutex;
    int table[2];
};

int main(int argc, char *argv[]) {
    // file descriptor for shared memory
    int shm_fd;
    // number of items to be produced / consumed
    int items=15;
    
    // open shared memory block
    shm_fd = shm_open("table", O_RDWR, 0666);
    
    // map shared memory block
    struct data *memory = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    while(shm_fd < 0) {
        shm_fd = shm_open("table", O_RDWR, 0);
    }

    int i = 0;
    // while there are still items to consume
    while(items > 0){
        // wait until critical section becomes unlocked
        sem_wait(&memory->mutex);
        // indicate that the consumer is ready to consume an item
        printf("Consumer is consuming an item.\n");
        // if the current slot is NOT empty
        if(memory->table[i] > -1) {
            // indicate which value is contained within the slot, and clear it
            printf("Slot %d contains value: %d. Emptying the slot.\n", i, memory->table[i]);
            // set the new value to -1 to show that it is empty
            memory->table[i] = -1;
            // increment index
            i++;
        // else if the current slot IS empty
        } else {
            // indicate slot is empty
            printf("Slot %d holds no value.\n", i);
        }

        // revert index to 0 if value  is greater than 1
        if(i > 1) { i = 0; };

        // decrement the number of items than need to be consumed
        items--;
        // post mutex indicating critical section is now open
        sem_post(&memory->mutex);
        // short pause
        sleep(rand()%2+1);
    }
   
    // close shared memory
    munmap(memory, sizeof(int));
    close(shm_fd);
    shm_unlink("table");

    return 0;
}