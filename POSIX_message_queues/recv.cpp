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
	// creates necessary variables
    mqd_t myQueue;
	ssize_t retVal;
    char messageBuff[MQ_MSGSIZE];
	FILE *fileRecv;

    // Create or open the queue
    myQueue = mq_open(MQ_NAME, O_RDWR | O_CREAT, 0744, NULL);

	// checks if there is an error opening queue
    if (myQueue == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

	// create file
    fileRecv = fopen(FILENAME, "a");

	// checks for errors in opening file
    if (fileRecv == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

	// loop through  message queue
    do {
        retVal = mq_receive(myQueue, messageBuff, MQ_MSGSIZE, NULL);

        if (retVal == -1) {
            perror("mq_receive");
            break; // Exit loop on receive error
        }

        if (retVal > 0) {
            // Write the message content to the file
            fwrite(messageBuff, 1, retVal, fileRecv);
            printf("Received and wrote %zd bytes to file.\n", retVal);
        }

    } while (retVal > 0); // Continue until an empty message or error is received

    // Close the file and queue before exiting
    fclose(fileRecv);
    mq_close(myQueue);
    mq_unlink(MQ_NAME);

    return 0;
}


