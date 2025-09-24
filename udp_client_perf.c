// UDP Socket Client Performance Test
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char msg[MSG_SIZE] = "test_message";
    printf("UDP Client: Starting to send %d messages to port %d...\n", NUM_MESSAGES, PORT);
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    for (int i = 0; i < NUM_MESSAGES; i++) {
        if (sendto(fd, msg, MSG_SIZE, 0, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
            perror("sendto");
            break;
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("UDP Socket Client Performance:\n");
    printf("Messages sent: %d\n", NUM_MESSAGES);
    printf("Time: %.6f seconds\n", time_taken);
    printf("Messages/sec: %.2f\n", NUM_MESSAGES / time_taken);
    
    close(fd);
    return 0;
}