#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>

#define MAX_READ_SIZE 4096
#define MQ_NAME "/cpsc351queue"

int main(int argc, char** argv)
{
	
	// The message queue descriptor and the return value
	mqd_t myQueue, retVal;

	// Define Send File and Message Buff
	FILE *fileSend;
    char messageBuff[MAX_READ_SIZE];
	
	// The message queue attributes
	mq_attr attr;
	
	// Initialize the attributes
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 4096;
	attr.mq_curmsgs = 0;
		
	// Sanity checks
	if(argc != 2)
	{
		// fprintf(stderr, "USAGE: %s <message to send>\n");
		exit(-1);
	}
		
	// Let's create the queue
	myQueue = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0744, &attr);
	
	// Sanity checks
	if(myQueue < 0)
	{
		perror("mq_open");
		exit(-1);
	}

	// Open the send file
    fileSend = fopen(argv[1], "rb");
    if (fileSend == NULL) {
        perror("fopen");
        exit(-1);
    }

	// Read file 
	while (!feof(fileSend)) {
        // Read at most MAX_READ_SIZE bytes from the file
        size_t bytesRead = fread(messageBuff, 1, MAX_READ_SIZE, fileSend);

        // Send the bytes read through the message queue
        retVal = mq_send(myQueue, messageBuff, bytesRead, 1);

		// Send file sanity check
        if (retVal == -1) {
            perror("mq_send");
            exit(-1);
        }
    }

	// Empty message for indication of end of file 
	retVal = mq_send(myQueue, "", 0, 2);
	
	// Send file sanity checks
	if(retVal == -1)
	{
		perror("mq_send");
		exit(-1);
	}

	// Close file and the message queue
	fclose(fileSend);
	mq_close(myQueue);
		
	return 0;
}
