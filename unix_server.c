// UNIX Domain Socket Server
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define SOCK_PATH "unix_sock"

int main() {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path)-1);

    unlink(SOCK_PATH);
    bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(fd, 5);

    int client = accept(fd, NULL, NULL);
    char buf[50];
    read(client, buf, sizeof(buf));
    printf("Server got: %s\n", buf);
    close(client);
    close(fd);
    unlink(SOCK_PATH);
    return 0;
}
