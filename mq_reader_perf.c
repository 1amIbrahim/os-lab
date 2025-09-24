// POSIX Message Queue Reader Performance Test
#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

#define NUM_MESSAGES 1000000
#define MSG_SIZE 16
#define MQ_NAME "/myq_perf"

int main() {
    // Wait a bit for writer to set up
    usleep(100000); // 100ms
    
    mqd_t mq = mq_open(MQ_NAME, O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return 1;
    }
    
    char buf[MSG_SIZE];
    printf("MQ Reader: Starting to receive %d messages...\n", NUM_MESSAGES);
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    int received = 0;
    for (int i = 0; i < NUM_MESSAGES; i++) {
        if (mq_receive(mq, buf, MSG_SIZE, NULL) == -1) {
            if (errno == EAGAIN) {
                // No message available, wait a bit and retry
                usleep(1);
                i--; // Retry this message
                continue;
            }
            perror("mq_receive");
            break;
        }
        
        received++;
        if (received % 100000 == 0) {
            printf("Reader: Received %d messages\n", received);
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("Message Queue Reader Performance:\n");
    printf("Messages received: %d\n", received);
    printf("Time: %.6f seconds\n", time_taken);
    printf("Messages/sec: %.2f\n", received / time_taken);
    printf("Avg time per message: %.2f microseconds\n", 
           (time_taken * 1000000) / received);
    
    mq_close(mq);
    mq_unlink(MQ_NAME); // Clean up
    
    return 0;
}