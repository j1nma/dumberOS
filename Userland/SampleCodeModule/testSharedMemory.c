#include "tests.h"

#define SHARED_ID 1234

void errorShared(int error) {
	switch (error) {
		case 1: {
			printf("Error: Shared memory not created.\n");
			break;
		}
		case 2: {
			printf("Error: Shared memory not retrived from second process.\n");
			break;
		}
		case 3: {
			printf("Error: Shared memory not retrived from main process.\n");
			break;
		}
		case 4: {
			printf("Error: Second process didn't write on shared memory.\n");
			break;
		}
		case 5: {
			printf("Error: Created successful on the same SHARED_ID without freeing first.\n");
			break;
		}
		case 6: {
			printf("Error: Failed to create Shared memory on same SHARED_ID after freeing.\n");
			break;
		}
		default: {
			printf("Error: %d\n", error);
			break;
		}
	}
}

void sharingProcess() {
	printf("Hi, im: %d.\n", getPid());

	void * sh;
	if (getSharedMemory(SHARED_ID, &sh) == 0) {
		errorShared(2);
		LoopNop();
	}

	printf("Got shared memory, my pid: %d.\n", getPid());

	((char *)sh)[0] = 'A';

	printf("Wrote in shared memory. Waiting to die...\n");
	LoopNop();
}

void testSharedMemory() {
	printf("Shared memory test...\n");
	if (createSharedMemory(SHARED_ID, 1) == 0) {
		errorShared(1);
		return;
	}


	int pid = createProcess(&sharingProcess);
	printf("Created sharing process pid: %d\n", pid);


	void * shared;
	if (getSharedMemory(SHARED_ID, &shared) == 0) {
		errorShared(3);
		freeSharedMemory(SHARED_ID);
		killProcess(pid);
		return;
	}

	printf("Got shared memory, my pid: %d.\n", getPid());

	printf("Waiting for other process to write in Shared memory.\n");

	int loops = 0;
	while (((char *)shared)[0] != 'A' && loops < 5000000)
		loops++;




	if (((char *)shared)[0] == 'A') {
		printf("Other process wrote as expected: %c.\n", ((char *)shared)[0]);
	}else {
		errorShared(4);
		freeSharedMemory(SHARED_ID);
		killProcess(pid);
		return;
	}

	killProcess(pid);

	printf("Trying to create Shared memory with same id...\n");
	if (createSharedMemory(SHARED_ID, 1) == 0) {
		printf("Creating failed successfully.\n");
	}else{
		errorShared(5);
		freeSharedMemory(SHARED_ID);
		return;
	}

	printf("Freeing Shared memory...\n");
	freeSharedMemory(SHARED_ID);


	printf("Trying to create Shared memory with same id...\n");
	if (createSharedMemory(SHARED_ID, 1) != 0) {
		printf("Creating successful.\n");
	}else{
		errorShared(6);
		freeSharedMemory(SHARED_ID);
		return;
	}

	printf("Freeing Shared memory...\n");
	freeSharedMemory(SHARED_ID);

	printf("Done!\n");
	

}