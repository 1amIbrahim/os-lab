// UDP Socket Server Performance Test
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define NUM_MESSAGES 1000000
#define MSG_SIZE 16
#define PORT 9090

int main() {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("socket");
        return 1;
    }
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        return 1;
    }

    char buf[MSG_SIZE];
    printf("UDP Server: Starting to receive %d messages on port %d...\n", NUM_MESSAGES, PORT);
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    for (int i = 0; i < NUM_MESSAGES; i++) {
        if (recvfrom(fd, buf, MSG_SIZE, 0, NULL, NULL) == -1) {
            perror("recvfrom");
            break;
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("UDP Socket Server Performance:\n");
    printf("Messages received: %d\n", NUM_MESSAGES);
    printf("Time: %.6f seconds\n", time_taken);
    printf("Messages/sec: %.2f\n", NUM_MESSAGES / time_taken);
    printf("Avg time per message: %.2f microseconds\n", 
           (time_taken * 1000000) / NUM_MESSAGES);
    
    close(fd);
    return 0;
}