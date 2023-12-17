#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>

#define MQ_MSGSIZE 10000

int main()
{
	// The message queue descriptor and the return value
	mqd_t myQueue, retVal;
	
	// The message queue attributes
	mq_attr attr;
	
	// The message buffer
	char messageBuff[MQ_MSGSIZE];
	
	unsigned int priority;	
	
	// Initialize the attributes
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 8192;
	attr.mq_curmsgs = 0;
	
	// Let's create the queue
	myQueue = mq_open("/myqueue", O_RDWR, 0744, 0);
	
	// Sanity checks
	if(myQueue < 0)
	{
		perror("mq_open");
		//exit(-1);
	}

	// NOTE: the MQ_MSGSIZE must be greater than
	// the mq_msgsize field of the attribute struct
	// used to create the queue	
	retVal = mq_receive(myQueue, messageBuff,MQ_MSGSIZE, NULL);
	
	// Sanity checks
	if(retVal == -1)
	{
		perror("mq_receive");
		exit(-1);
	}
	
	fprintf(stderr, "%s\n", messageBuff);
	
	// Close the queue
	mq_close(myQueue);
		
	return 0;
}
