#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	
	// The message queue descriptor and the return value
	mqd_t myQueue, retVal;
	
	// The message queue attributes
	mq_attr attr;
	
	// Initialize the attributes
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 8192;
	attr.mq_curmsgs = 0;
		
	// Sanity checks
	if(argc != 2)
	{
		fprintf(stderr, "USAGE: %s <message to send>\n");
		exit(-1);
	}
		
	// Let's create the queue
	myQueue = mq_open("/myqueue", O_CREAT | O_RDWR, 0744, &attr);
	
	// Sanity checks
	if(myQueue < 0)
	{
		perror("mq_open");
		exit(-1);
	}
	
	// Send the message
	retVal = mq_send(myQueue, argv[1], strlen(argv[1]) + 1, 1);
	
	// Send sanity checks
	if(retVal != 0)
	{
		perror("mq_send");
		exit(-1);
	}

	// Close the message queue
	mq_close(myQueue);
		
	return 0;
}
