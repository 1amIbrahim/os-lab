// Anonymous Pipes Performance Test
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#define NUM_MESSAGES 1000000
#define MSG_SIZE 16

int main() {
    int fd[2];
    char buf[MSG_SIZE];
    char msg[MSG_SIZE] = "test_message";
    
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    if (fork() == 0) { 
        // Child - Writer
        close(fd[0]); // Close read end
        
        for (int i = 0; i < NUM_MESSAGES; i++) {
            if (write(fd[1], msg, MSG_SIZE) == -1) {
                perror("write");
                break;
            }
        }
        close(fd[1]);
        return 0;
        
    } else { 
        // Parent - Reader
        close(fd[1]); // Close write end
        
        for (int i = 0; i < NUM_MESSAGES; i++) {
            if (read(fd[0], buf, MSG_SIZE) == -1) {
                perror("read");
                break;
            }
        }
        close(fd[0]);
        
        wait(NULL); // Wait for child to finish
        
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        double time_taken = (end.tv_sec - start.tv_sec) + 
                           (end.tv_nsec - start.tv_nsec) / 1e9;
        
        printf("Anonymous Pipes Performance:\n");
        printf("Messages: %d\n", NUM_MESSAGES);
        printf("Time: %.6f seconds\n", time_taken);
        printf("Messages/sec: %.2f\n", NUM_MESSAGES / time_taken);
        printf("Avg time per message: %.2f microseconds\n", 
               (time_taken * 1000000) / NUM_MESSAGES);
    }
    return 0;
}