// Named Pipe (FIFO) Writer
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd = open("myfifo", O_WRONLY);
    char msg[] = "Hello FIFO";
    write(fd, msg, strlen(msg)+1);
    close(fd);
    return 0;
}
