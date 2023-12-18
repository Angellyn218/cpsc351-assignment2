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
    mq_attr attr;
    char messageBuff[MQ_MSGSIZE];

    // Initialize message queue attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 4096;
    attr.mq_curmsgs = 0;

    // Create or open the queue
    myQueue = mq_open(MQ_NAME, O_RDWR, 0744, &attr);

    if (myQueue < 0) {
        perror("mq_open");
        exit(-1);
    }

    FILE *fileRecv = fopen(FILENAME, "a");
    if (fileRecv == NULL) {
        perror("fopen");
        exit(-1);
    }

    while (1) {
		retVal = mq_receive(myQueue, messageBuff, MQ_MSGSIZE, NULL);

		if (retVal == -1) {
			perror("mq_receive");
			exit(-1);
		}

		if (retVal > 0) {
			// Write the message content to the file
			fwrite(messageBuff, 1, retVal, fileRecv);
			printf("Received and wrote %d bytes to file.\n", retVal);
		} else {
			printf("Received empty message or error.\n");
			break;
			
		}
	}
	// Close the file and queue before exiting
	fclose(fileRecv);
	mq_close(myQueue);
	mq_unlink(MQ_NAME);
	exit(0);

    // This part of the code is unreachable
    return 0;
}
