//Sky Vercauteren
//June 2025
//Operating Systems

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

/*
Checks to see if the dividend (second argument) is divisible by the divisor (first argument)
prints the output to the console.
returns 1 if success or -1 if fail
*/

int main(int argc, char **argv)
{
	//get PID and print it to make sure it matches. 
	pid_t pid = getpid();
	printf("Checker process [%d]: Starting.\n", pid);
	
	//parse arguments
	int divisor = atoi(argv[0]);
	int dividend = atoi(argv[1]);
	
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
	printf("Checker process [%d]: Returning %d.\n", pid, exit);
	return exit;
}