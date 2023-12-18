#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>

#define MAX_READ_SIZE 4096
#define MQ_NAME "/cpsc351queue"

int main(int argc, char** argv) {
    mqd_t myQueue;
    ssize_t retVal;
    char messageBuff[MAX_READ_SIZE];
    FILE *fileSend;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_READ_SIZE;
    attr.mq_curmsgs = 0;

    // Create the message queue (if it doesn't exist)
    myQueue = mq_open(MQ_NAME, O_CREAT | O_WRONLY, 0744, &attr);

    if (myQueue == (mqd_t)-1) {
        perror("mq_open");
        return -1; // Return -1 on error
    }

    // Open the file for sending
    fileSend = fopen(argv[1], "rb");

    if (fileSend == NULL) {
        perror("fopen");
        return -1; // Return -1 on error
    }

    // Read file
    while (!feof(fileSend)) {
        size_t bytesRead = fread(messageBuff, 1, MAX_READ_SIZE, fileSend);

        retVal = mq_send(myQueue, messageBuff, bytesRead, 1);

        if (retVal == -1) {
            perror("mq_send");
            return -1; // Return -1 on error
        }
    }

    // Send an empty message to indicate end of file
    retVal = mq_send(myQueue, "", 0, 1);

    if (retVal == -1) {
        perror("mq_send");
        return -1; // Return -1 on error
    }

    // Close file and the message queue
    fclose(fileSend);
    mq_close(myQueue);

    return 0;
}
