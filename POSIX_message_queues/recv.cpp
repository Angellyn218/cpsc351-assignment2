#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>

#define MQ_MSGSIZE 5000
#define MQ_NAME "/cpsc351queue"
#define FILENAME "file__recv"

int main() {
    mqd_t myQueue, retVal;
    char messageBuff[MQ_MSGSIZE];

    // Create or open the queue
    myQueue = mq_open(MQ_NAME, O_RDWR | O_CREAT, 0744, NULL);

    if (myQueue < 0) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    FILE *fileRecv = fopen(FILENAME, "a");
    if (fileRecv == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        retVal = mq_receive(myQueue, messageBuff, MQ_MSGSIZE, NULL);

        if (retVal == -1) {
            perror("mq_receive");
            break; // Exit loop on receive error
        }

        if (retVal > 0) {
            // Write the message content to the file
            fwrite(messageBuff, 1, retVal, fileRecv);
            printf("Received and wrote %d bytes to file.\n", retVal);
        } else {
            printf("Received empty message or error.\n");
            break; // Exit loop on empty message or error
        }
    }

    // Close the file and queue before exiting
    fclose(fileRecv);
    mq_close(myQueue);
    mq_unlink(MQ_NAME);

    return 0;
}


