// Named Pipe (FIFO) Reader Performance Test
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#define NUM_MESSAGES 1000000
#define MSG_SIZE 16
#define FIFO_NAME "/tmp/perf_fifo"

int main() {
    char buf[MSG_SIZE];
    
    printf("Reader: Opening FIFO for reading...\n");
    int fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    
    printf("Reader: Starting to receive %d messages...\n", NUM_MESSAGES);
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    for (int i = 0; i < NUM_MESSAGES; i++) {
        if (read(fd, buf, MSG_SIZE) == -1) {
            perror("read");
            break;
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    close(fd);
    unlink(FIFO_NAME); // Clean up FIFO
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("FIFO Reader Performance:\n");
    printf("Messages received: %d\n", NUM_MESSAGES);
    printf("Time: %.6f seconds\n", time_taken);
    printf("Messages/sec: %.2f\n", NUM_MESSAGES / time_taken);
    printf("Avg time per message: %.2f microseconds\n", 
           (time_taken * 1000000) / NUM_MESSAGES);
    
    return 0;
}