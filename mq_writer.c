// POSIX Message Queue Writer
#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main() {
    mqd_t mq = mq_open("/myq", O_CREAT | O_WRONLY, 0666, NULL);
    char msg[] = "Hello MQ";
    mq_send(mq, msg, strlen(msg)+1, 0);
    mq_close(mq);
    return 0;
}
