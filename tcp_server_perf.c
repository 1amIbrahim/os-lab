// TCP Socket Server Performance Test
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define NUM_MESSAGES 1000000
#define MSG_SIZE 16
#define PORT 8080

int main() {
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == -1) {
        perror("socket");
        return 1;
    }
    
    // Allow socket reuse
    int opt = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        return 1;
    }
    
    if (listen(server, 5) == -1) {
        perror("listen");
        return 1;
    }

    printf("TCP Server: Waiting for client connection on port %d...\n", PORT);
    int client = accept(server, NULL, NULL);
    if (client == -1) {
        perror("accept");
        return 1;
    }
    
    char buf[MSG_SIZE];
    printf("TCP Server: Starting to receive %d messages...\n", NUM_MESSAGES);
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    for (int i = 0; i < NUM_MESSAGES; i++) {
        if (read(client, buf, MSG_SIZE) == -1) {
            perror("read");
            break;
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("TCP Socket Server Performance:\n");
    printf("Messages received: %d\n", NUM_MESSAGES);
    printf("Time: %.6f seconds\n", time_taken);
    printf("Messages/sec: %.2f\n", NUM_MESSAGES / time_taken);
    printf("Avg time per message: %.2f microseconds\n", 
           (time_taken * 1000000) / NUM_MESSAGES);
    
    close(client);
    close(server);
    return 0;
}