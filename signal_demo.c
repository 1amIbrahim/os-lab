// Signals Demo
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    printf("Caught signal %d\n", sig);
}

int main() {
    signal(SIGUSR1, handler);
    printf("PID: %d\n", getpid());
    while (1) pause();
    return 0;
}
