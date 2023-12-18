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
    mqd_t myQueue, retVal;
    FILE *fileSend;
    char messageBuff[MAX_READ_SIZE];

    // The message queue attributes
    struct mq_attr attr;

    // Initialize the attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 4096;
    attr.mq_curmsgs = 0;

    // Sanity checks for command-line arguments
    if (argc != 2) {
        fprintf(stderr, "USAGE: %s <file name>\n", argv[0]);
        exit(-1);
    }

    // Open the file for reading
    fileSend = fopen(argv[1], "rb");
    if (fileSend == NULL) {
        perror("fopen");
        exit(-1);
    }

    // Let's create the queue
    myQueue = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0744, &attr);
    if (myQueue < 0) {
        perror("mq_open");
        exit(-1);
    }

    // Read file and send content through the message queue
    size_t bytesRead;
    while ((bytesRead = fread(messageBuff, 1, MAX_READ_SIZE, fileSend)) > 0) {
        // Send the bytes read through the message queue
        retVal = mq_send(myQueue, messageBuff, bytesRead, 1);

        // Send file sanity check
        if (retVal == -1) {
            perror("mq_send");
            exit(-1);
        }
    }

    // Close the file and message queue after sending data
    fclose(fileSend);
    mq_close(myQueue);

    return 0;
}
