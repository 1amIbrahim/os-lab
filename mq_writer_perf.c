// POSIX Message Queue Writer Performance Test
#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#define NUM_MESSAGES 1000000
#define MSG_SIZE 16
#define MQ_NAME "/myq_perf"

int main() {
    // Clean up any existing message queue
    mq_unlink(MQ_NAME);
    
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;  // Max messages in queue
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_curmsgs = 0;
    
    mqd_t mq = mq_open(MQ_NAME, O_CREAT | O_WRONLY, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return 1;
    }
    
    char msg[MSG_SIZE] = "test_message";
    printf("MQ Writer: Starting to send %d messages...\n", NUM_MESSAGES);
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    for (int i = 0; i < NUM_MESSAGES; i++) {
        snprintf(msg, MSG_SIZE, "msg_%d", i);
        
        if (mq_send(mq, msg, strlen(msg)+1, 0) == -1) {
            if (errno == EAGAIN) {
                // Queue full, wait a bit and retry
                usleep(1);
                i--; // Retry this message
                continue;
            }
            perror("mq_send");
            break;
        }
        
        if (i % 100000 == 0) {
            printf("Writer: Sent %d messages\n", i);
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("Message Queue Writer Performance:\n");
    printf("Messages sent: %d\n", NUM_MESSAGES);
    printf("Time: %.6f seconds\n", time_taken);
    printf("Messages/sec: %.2f\n", NUM_MESSAGES / time_taken);
    
    mq_close(mq);
    return 0;
}