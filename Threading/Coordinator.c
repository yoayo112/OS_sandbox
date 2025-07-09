//Sky Vercauteren
//June 2025
//Operating Systems

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


/** 
* a function for whatever work the parent should do - (would be where you put concurrent tasks for while wwaiting)
*/
int parent_work(pid_t pid){
	printf("Coordinator: forked process with ID %d.\n",pid); //print pid
	//wait for the child to complete the check so that only one process is executing at a time
	printf("Coordinator: waiting for process [%d].\n",pid); //print waiting
	int status;
   	waitpid(pid,&status,0); //wait
	printf("Coordinator: child process %d returned %d.\n", pid, WEXITSTATUS(status)); //print child exit status
	return status;
}


/**
 * a function for whatever work the child should do - (where we call exec())
 */
int child_work(char* divisor, char* dividend)
{
	//load the Checker program
	char *args[] = {"./checker", divisor, dividend, NULL};
	execlp(args[0], args[1], args[2], args[3]);
	return 0;
}


/**
 * function containing the logic for forking, and executing the child and parent behaviors
 */
int coordinate(char* divisor, char* dividend)
{
	//fork a new process
	pid_t pid = fork();
	if(pid == -1) //if fork fails
	{
		//handle errors
		printf("Coordinator: failed to fork()\n");
		return -1;
	}else if(pid > 0) //parent behavior
	{
		parent_work(pid);
		return 0;
	}
	else //child behavior
	{
		child_work(divisor, dividend);
		return -1;
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
		//parse the first argument as the divisor
		char* divisor = argv[1];
		//enter a loop to create 4 child threads
		for(int i=1; i <5; i++)
		{
			//parse the next argument for checking
			char* dividend = argv[i+1];
			
			//do all the forking behavior
			int thread = coordinate(divisor, dividend);
			if(thread == -1){return -1;}//when child processes return here, stop them.
		}
		printf("Coordinator: exiting.\n");
		//complete task
		return 0;
	}
}
