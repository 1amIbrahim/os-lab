// TCP Socket Server
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int server = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(server, (struct sockaddr*)&addr, sizeof(addr));
    listen(server, 5);

    int client = accept(server, NULL, NULL);
    char buf[50];
    read(client, buf, sizeof(buf));
    printf("TCP server got: %s\n", buf);
    close(client);
    close(server);
    return 0;
}
