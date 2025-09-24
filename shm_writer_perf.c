// POSIX Shared Memory + Semaphore Writer Performance Test
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>

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
    // Clean up any existing shared memory/semaphore
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_NAME);
    
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }
    
    if (ftruncate(fd, sizeof(struct shared_data)) == -1) {
        perror("ftruncate");
        return 1;
    }
    
    struct shared_data *ptr = (struct shared_data *)mmap(0, sizeof(struct shared_data), 
                                                         PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    sem_t *sem_write = sem_open(SEM_NAME, O_CREAT, 0666, 1); // Writer can start
    if (sem_write == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }

    // Initialize shared data
    ptr->count = 0;
    ptr->done = 0;
    strcpy(ptr->message, "test_message");

    printf("SHM Writer: Starting to send %d messages...\n", NUM_MESSAGES);
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    for (int i = 0; i < NUM_MESSAGES; i++) {
        sem_wait(sem_write);  // Wait for reader to be ready
        
        snprintf(ptr->message, MSG_SIZE, "msg_%d", i);
        ptr->count = i;
        
        sem_post(sem_write);  // Signal that message is ready
        
        if (i % 100000 == 0) {
            printf("Writer: Sent %d messages\n", i);
        }
    }
    
    // Signal completion
    sem_wait(sem_write);
    ptr->done = 1;
    sem_post(sem_write);
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("Shared Memory Writer Performance:\n");
    printf("Messages sent: %d\n", NUM_MESSAGES);
    printf("Time: %.6f seconds\n", time_taken);
    printf("Messages/sec: %.2f\n", NUM_MESSAGES / time_taken);

    munmap(ptr, sizeof(struct shared_data));
    close(fd);
    sem_close(sem_write);
    
    return 0;
}