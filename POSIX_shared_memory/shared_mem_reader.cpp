#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int main()
{
	// The size of the future message queue (in bytes)
	const int SHARED_MEM_SIZE = 4096;	
	
	// The buffer used to store the message copied
	// copied from the shared memory
	char buff[100];
	
	// The variable to hold the shared memory id
	int shmid = -1;

	// Allocate the shared memory object
	// @param "/mysharedmem" the path that will contain the file
	// that abstracts the the shared memory object
	// @param O_RDONLY - access the memory that has already
	// been allocated and for read-only access.  Please note
	// Unlike in the sender, we are not creating the shared
	// memory segment if it does not exist.
	// 0666 - the rwx permissions in octal format
	// The return value will be the shared memory id 
	shmid = shm_open("/mysharedmem", O_RDONLY, 0666);

	fprintf(stderr, "shmid = %d\n", shmid);
	
	// The shared memory pointer;
	void* sharedMemPtr = NULL;
	
	// Make sure the shared memory allocation succeeded
	if(shmid < 0)
	{
		perror("shm_open");
		exit(1);
	}


	 // Get the shared memory pointer
	 // @param 0 - let OS pick the address
	 // @paam SHARED_MEM_SIZE - the size of the memory
	 // @param PROT_WRITE - the permissions enforced on the memory
	 // accessible through this pointer. These permissions must
	 // not conflict with the permissions specified in shm_open().
	 // The permissions can be moe restrictive than the shm_open()
	 // permissions, but not less. The possble permissions are
	 // PROT_NONE - the memory may not be accessed
	 // PROT_READ - the memory can be read (the perission below; note:
	 // in the sender it was PROT_WRITE.
	 // PROT_WRITE - the memory can be written
	 // PROT_EXEC - the memory can be executed
	 // We can pass multiple permissions as e.g., PROT_READ | PROT_WRITE
	 // If we want the memory to be both readable and written
	 // @param flags - Basically, necessary to enable proper memory
	 // sharing; we will revising this later.  For now, please always use
	 // MAP_SHARED
	 // @param shmid - the id of the shared memory returned by shm_open()
	 // @param offset - At what offset of the shared memory should the pointer
	 // point to? 0 means to the beginning.
	 // @return - on error, will return MAP_FAILED
	 sharedMemPtr = mmap(0, SHARED_MEM_SIZE, PROT_READ, MAP_SHARED, shmid, 0);
	
	 // Did the mmap() fail?
	 if(sharedMemPtr == MAP_FAILED)
	 {
		perror("mmap");
		exit(1);
	 }
	
	 // We can now copy the data into memory in the same way as we copy
	 // to any array...for example, let's copy string "Hello world".
	 // If you are not familiar with, strncpy(), it copies strings between
	 // arrays.  Below will keep copying characters from "Hello World" to
	 // to the memory pointed to sharedMemPtr until either the NULL char
	 // is encountered in the source or 12 chars have been copied
	 strncpy(buff, (char*)sharedMemPtr, 100);
	
	fprintf(stderr, "Got: %s\n", buff);
	
	return 0;
}
