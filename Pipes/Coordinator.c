//Sky Vercauteren
//June 2025
//Operating Systems

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>


/** 
* a function for whatever work the parent should do - (would be where you put concurrent tasks for while wwaiting)
*/
/*
* Changed for HW3 - waits for the child, attaches to memory, reads the result from memory, destroys shared memory.
*/
int parent_wait(pid_t pid, int sharedId, int dividend, int divisor){
	//wait for the child to complete the check so that only one process is executing at a time
	printf("Coordinator: waiting on child process ID %d...\n",pid); //print waiting
	int status;
   	waitpid(pid,&status,0); //wait

	// HW3 additions 
	int * outPointer = (int*)shmat(sharedId, NULL, 0); //attach to shared memory
	//read from shred memory
	if(*outPointer == 0){
		printf("Coordinator: result %d read from shared memory: %d is not divisible by %d.\n", *outPointer, dividend, divisor); //print child exit status
	}
	else if(*outPointer == 1){
		printf("Coordinator: result %d read from shared memory: %d is divisible by %d.\n", *outPointer, dividend, divisor); //print child exit status
	}
	//destroy shared memory
	shmctl(*outPointer, IPC_RMID, NULL);
	
	return status;
}


/**
 * a function for whatever work the child should do - (where we call exec())
 */
int child_work(char* buffer, char* divisor, char* dividend)
{
	//load checker.c
	//only change for HW3 is including the buffer (holding the pipe)
	char *args[] = {"./checker", buffer, divisor, dividend, NULL};
	execlp(args[0], args[1], args[2], args[3], args[4]);
	return 0;
}


/**
 * function containing the logic for forking, and executing the child and parent behaviors
 */
int coordinate(char* divisor, char* dividend, int i, int pids[][2])
{
	//HW3 Addition

	//make some shared memory
	//shmget(key = private/unique, size of 4 ints, create with permissions flag)
	int sharedID = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);

	//make a pipe
	int tube[2];
	pipe(tube);

	//fork a process
	pid_t pid = fork();
	if(pid == -1) //if fork fails
	{
		//handle errors
		printf("Coordinator: failed to fork()\n");
		return -1;
	}else if(pid > 0) //parent behavior
	{
		printf("Coordinator: forked process with ID %d.\n",pid); //print pid

		//HW 3 addition
		close(tube[0]); //write only pipe
		write(tube[1], &sharedID, sizeof(sharedID)); //write shared memory ID to pipe
		close(tube[1]);
		
		//output for grader
		printf("Coordinator: wrote shm ID %d to pipe (%lu bytes)\n",sharedID,sizeof(sharedID));
		//storing the thread id and memory location to print out later.
		pids[i][0] = pid;
		pids[i][1] = sharedID;

		//finish parent behavior, next thread.
		return 0;
	}
	else //child behavior
	{
		//HW3 Addition
		close(tube[1]); //read only pipe
		char buffer[8];
		sprintf(buffer, "%d", tube[0]); //put the read end of the pipe into a buffer to pass it as arg.

		child_work(buffer, divisor, dividend);
		return -1; //return to main loop
	}
}


/**
 * Main driver for coordinator, calls checker with forked processes
 */
int main(int argc, char **argv) 
{
	//should receive 5 arguments.
	if (argc != 6)
    {
        printf("Incorrect number of arguments: %d\n",argc);
        return -1;
    }else{
		//HW3 Addition
		int pids[4][2]; // keep track of the threads and their shm

		//parse the first argument as the divisor
		char* divisor = argv[1];
		//enter a loop to create 4 child threads
		for(int i=1; i <5; i++)
		{
			//parse the next argument for checking
			char* dividend = argv[i+1];
			
			//do all the forking behavior
			int thread = coordinate(divisor, dividend, (i-1), pids);
			if(thread == -1){return -1;}//when child processes return here, stop them.
		}

		//New for HW3 - WAIT FOR CHILDREN!
		for(int i=0; i<4; i++)
		{
			parent_wait(pids[i][0], pids[i][1], atoi(argv[i+2]), atoi(divisor));
		}

		printf("Coordinator: exiting.\n");
		//complete task
		return 0;
	}
}
