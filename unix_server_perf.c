// UNIX Domain Socket Server Performance Test
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

    unlink(SOCK_PATH);
    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        return 1;
    }
    
    if (listen(fd, 5) == -1) {
        perror("listen");
        return 1;
    }

    printf("UNIX Server: Waiting for client connection...\n");
    int client = accept(fd, NULL, NULL);
    if (client == -1) {
        perror("accept");
        return 1;
    }
    
    char buf[MSG_SIZE];
    printf("UNIX Server: Starting to receive %d messages...\n", NUM_MESSAGES);
    
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
    
    printf("UNIX Socket Server Performance:\n");
    printf("Messages received: %d\n", NUM_MESSAGES);
    printf("Time: %.6f seconds\n", time_taken);
    printf("Messages/sec: %.2f\n", NUM_MESSAGES / time_taken);
    printf("Avg time per message: %.2f microseconds\n", 
           (time_taken * 1000000) / NUM_MESSAGES);
    
    close(client);
    close(fd);
    unlink(SOCK_PATH);
    return 0;
}