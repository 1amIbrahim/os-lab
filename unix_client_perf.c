// UNIX Domain Socket Client Performance Test
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define SOCK_PATH "/tmp/unix_sock_perf"
#define NUM_MESSAGES 1000000
#define MSG_SIZE 16

int main() {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        return 1;
    }
    
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path)-1);

    printf("UNIX Client: Connecting to server...\n");
    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect");
        return 1;
    }

    char msg[MSG_SIZE] = "test_message";
    printf("UNIX Client: Starting to send %d messages...\n", NUM_MESSAGES);
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    for (int i = 0; i < NUM_MESSAGES; i++) {
        if (write(fd, msg, MSG_SIZE) == -1) {
            perror("write");
            break;
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("UNIX Socket Client Performance:\n");
    printf("Messages sent: %d\n", NUM_MESSAGES);
    printf("Time: %.6f seconds\n", time_taken);
    printf("Messages/sec: %.2f\n", NUM_MESSAGES / time_taken);
    
    close(fd);
    return 0;
}