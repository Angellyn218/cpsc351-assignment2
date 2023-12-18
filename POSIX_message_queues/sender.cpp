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

    // Open the message queue
    myQueue = mq_open(MQ_NAME, O_WRONLY);

    if (myQueue == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    // Open the file for sending
    fileSend = fopen(argv[1], "rb");

    if (fileSend == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Read file
    while (!feof(fileSend)) {
        size_t bytesRead = fread(messageBuff, 1, MAX_READ_SIZE, fileSend);

        retVal = mq_send(myQueue, messageBuff, bytesRead, 1);

        if (retVal == -1) {
            perror("mq_send");
            exit(EXIT_FAILURE);
        }
    }

    // Send an empty message to indicate end of file
    retVal = mq_send(myQueue, "", 0, 1);

    if (retVal == -1) {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }

    // Close file and the message queue
    fclose(fileSend);
    mq_close(myQueue);

    return 0;
}

