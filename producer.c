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

int main() {
    // shared memory file descriptor
    int shm_fd;
    // number of items to be produced / consumed
    int items=15;  

    // create shared memory block
    shm_fd = shm_open("table", O_CREAT | O_RDWR, 0666);

    // change the size of the shared memory block
    ftruncate(shm_fd, 1024);

    // map the shared memory block
    struct data *memory = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    
    // initialize each slot in table to -1
    for(int i = 0; i < 2; ++i) {
        memory->table[i] = -1;
    }

    // initialize mutex semaphore
    sem_init(&memory->mutex, 1, 1);

    // indicate that the producer is ready to begin producing
    printf("\nProducer is ready to produce items.\n");
    // index
    int i = 0;
    // while there are still items to produce
    while(items > 0){
        // wait for critical section to become unlocked
        sem_wait(&memory->mutex);
        // indicate that the producer is ready to produce an item
        printf("Producer is producing an item.\n");

        // if cell is NOT full, produce an item and put it in the slot
        if(memory->table[i] == -1) {
            // generate random data (integer 1-100)
            int t = rand()%100+1;
            // fill current cell with that data
            memory->table[i] = t;
            // indicate which cell was filled and witch which value
            printf("Slot %d now contains value %d.\n", i, t);
            // increment index
            i++;

        // if cell IS full, indicate cell is full and check again in the next section
        } else {
            // indicate that the cell is full
            printf("Current slot is filled.\n");
        }

        // revert index to 0 if value  is greater than 1
        if(i > 1) { i = 0; };

        // decrement the number of items to prod / con
        items--;
        // post mutex indicating that the critical section is now open
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