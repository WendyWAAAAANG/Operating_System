// Author: Ruoxin WANG
// Student ID: 2030026150
// Function Description: let user choose an algorithm and run exe files in task1 and task2.
// Date: 2023/04/03

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define SIZE 1024

int main() {
    
    while (1) {
        // output prompt for input.
   	char prompt[] = "Choose algorithm (rr for Round Robin, sjf for Shortest Job First, exit the system): ";
   	char choice[SIZE];
    	
    	// ask user to type a command.
	write(1, prompt, strlen(prompt));
	// Read from the standard input the command typed by the user (note
	// that read puts into the array buf the ’\n’ character typed
	// by the user when the user presses the Enter key on the keyboard):
	read(0, choice, SIZE);
	// Replace the Enter key typed by the user with ’\0’:
	for (int i = 0; i < SIZE; i++) {
		if (choice[i] == '\n' || choice[i] == '\r') {
			choice[i] = '\0';
		}
	}

	// check whether usr want to exit the program.
    	if(strcmp("exit", choice) == 0){
		// which means user has input "exit".
		return 0;
	}

	// otherwise, create child process to execute command.
	// create a new child prcess.
	pid_t pid = fork();
	// Exit status of child
	int status;
	if(pid < 0){
		// which means failed to create a child.
		char pmpt0[] = "Fork failed!";
		write(1, pmpt0, strlen(pmpt0));
		write(1, "\n", 1);
		return 1;
	} else if (pid == 0) {
		// here is child process.
		// which means usr X want to exit,
		// so we check which cmd usr input.
		if (!strcmp(choice, "sjf")) {
			// execute sjf scheduling algorithm.
	    		execlp("./sjf", choice, NULL);
	    		continue;
	    	} else if (!strcmp(choice, "rr")) {
	    		// execute rr scheduling algorithm.
	    		execlp("./rr", choice, NULL);
	    		continue;
	    	} else {
	    		// which means usr input some invalid string.
	    		// ask usr to do again.
	    		printf("\nPlease input a valid algorithm!\n");
	    		continue;
	    	}
	    	// Child process dies after failed exec.
		return 1;
	} else {
		// here is parent process, wait for child and for next input.
		wait(&status);
		continue;
	}
    }

    return 0;
}
