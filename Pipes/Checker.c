#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>

/*
Checks to see if the dividend (second argument) is divisible by the divisor (first argument)
prints the output to the console.
returns 1 if success or -1 if fail
*/

int main(int argc, char **argv)
{
	//get PID and print it to make sure it matches. 
	pid_t pid = getpid();
	printf("Checker process [%d]: starting.\n", pid);
	
	//addition of HW 3
	int buffer = atoi(argv[0]);
	int sharedMemId = atoi(argv[0]);
	//read buffer as pipe to get shared memory
	read(buffer, &sharedMemId, sizeof(sharedMemId)); //sharedMemId should now be a pointer to shared memory location
	close(buffer);//close the read end of the pipe
	printf("Checker process [%d]: read %lu bytes containing shm ID %d\n",pid,sizeof(sharedMemId),sharedMemId);
	//make a shared memory pointer
	int *sharedPointer = (int*)shmat(sharedMemId, NULL, 0);

	//parse rest of arguments
	int divisor = atoi(argv[1]);
	int dividend = atoi(argv[2]);
	
	//using exit to hold the boolean result (0 for false and 1 for true)
	int exit = 0;
	//using result to hold the result of the modulo
	int result = dividend % divisor;
	//print the respective output
	if(result == 0)
	{
		printf("Checker process [%d]: %d *IS* divisible by %d.\n", pid, dividend, divisor); 
		exit = 1;
	}
	else
	{
		printf("Checker process [%d]: %d *IS NOT* divisible by %d.\n", pid, dividend, divisor); 
		exit = 0;
	}

	//complete the operation
	printf("Checker process [%d]: wrote result (%d) to shared memory.\n", pid, exit); //ammended for HW3
	*sharedPointer = exit; //added for HW3
	shmdt(sharedPointer); //Added for HW3 - detatch
	return 0;
}