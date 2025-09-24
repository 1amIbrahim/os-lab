// POSIX Message Queue Reader
#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
    mqd_t mq = mq_open("/myq", O_RDONLY);
    char buf[50];
    mq_receive(mq, buf, sizeof(buf), NULL);
    printf("Reader got: %s\n", buf);
    mq_close(mq);
    return 0;
}
