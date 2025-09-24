// POSIX Shared Memory + Semaphore Reader Performance Test
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NUM_MESSAGES 1000000
#define MSG_SIZE 16
#define SHM_NAME "/myshm_perf"
#define SEM_NAME "/mysem_perf"

struct shared_data {
    char message[MSG_SIZE];
    int count;
    int done;
};

int main() {
    // Wait a bit for writer to set up
    usleep(100000); // 100ms
    
    int fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }
    
    struct shared_data *ptr = (struct shared_data *)mmap(0, sizeof(struct shared_data), 
                                                         PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    sem_t *sem_write = sem_open(SEM_NAME, 0);
    if (sem_write == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }

    printf("SHM Reader: Starting to receive %d messages...\n", NUM_MESSAGES);
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    int received = 0;
    while (!ptr->done && received < NUM_MESSAGES) {
        sem_wait(sem_write);  // Wait for message
        
        if (!ptr->done) {
            received++;
            if (received % 100000 == 0) {
                printf("Reader: Received %d messages\n", received);
            }
        }
        
        sem_post(sem_write);  // Signal that message was read
        
        if (ptr->done) break;
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("Shared Memory Reader Performance:\n");
    printf("Messages received: %d\n", received);
    printf("Time: %.6f seconds\n", time_taken);
    printf("Messages/sec: %.2f\n", received / time_taken);
    printf("Avg time per message: %.2f microseconds\n", 
           (time_taken * 1000000) / received);

    munmap(ptr, sizeof(struct shared_data));
    close(fd);
    sem_close(sem_write);
    
    // Clean up
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_NAME);
    
    return 0;
}