// Named Pipe (FIFO) Writer Performance Test
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

#define NUM_MESSAGES 1000000
#define MSG_SIZE 16
#define FIFO_NAME "/tmp/perf_fifo"

int main() {
    char msg[MSG_SIZE] = "test_message";
    
    // Create FIFO if it doesn't exist
    if (mkfifo(FIFO_NAME, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        return 1;
    }
    
    printf("Writer: Opening FIFO for writing...\n");
    int fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    
    printf("Writer: Starting to send %d messages...\n", NUM_MESSAGES);
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    for (int i = 0; i < NUM_MESSAGES; i++) {
        if (write(fd, msg, MSG_SIZE) == -1) {
            perror("write");
            break;
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    close(fd);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("FIFO Writer Performance:\n");
    printf("Messages sent: %d\n", NUM_MESSAGES);
    printf("Time: %.6f seconds\n", time_taken);
    printf("Messages/sec: %.2f\n", NUM_MESSAGES / time_taken);
    
    return 0;
}