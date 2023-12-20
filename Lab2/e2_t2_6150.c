// Author: Ruoxin WANG
// Student ID: 2030026150
// Task Requirements: creation of a child process and execute it.

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 1024
int main(void){
	char prompt[] = "Type a command: ";
	char buf[SIZE];

	// ask user to type a command.
	write(1, prompt, strlen(prompt));

	// Read from the standard input the command typed by the user (note
	// that read puts into the array buf the ’\n’ character typed
	// by the user when the user presses the Enter key on the keyboard):
	read(0, buf, SIZE);
	
	// Replace the Enter key typed by the user with ’\0’:
	for(int i = 0; i < SIZE; i++) {
		if(buf[i] == '\n' || buf[i] == '\r') {
			buf[i] = '\0';
			break;
		}
	}
	
	// char output_prompt[] = "Executing command: ";
	// output the user typed command:
	// write(1, output_prompt, strlen(output_prompt));
	// write(1, buf, strlen(buf));
	// write(1, "\n", 1);
	// create a new child prcess.
	pid_t pid = fork();
	// Exit status of child
	int status;
	if(pid < 0){
		char pmpt0[] = "Parent: fork failed.";
		write(1, pmpt0, strlen(pmpt0));
		write(1, "\n", 1);
		return 1;
	} else if(pid == 0) {
		// Code only executed by the new child process.
		char pmpt1[] = "Child now running the same program as parent, doing exec.";
		write(1, pmpt1, strlen(pmpt1));
		write(1, "\n", 1);
		execlp(buf, buf, NULL);
		// If exec succeeded, the new child now starts executing the code of
		// the main function of the xcalc program. The new child normally
		// never executes the code below, unless exec failed.
		char pmpt2[] = "Child: exec failed, die.";
		write(1, pmpt2, strlen(pmpt2));
		write(1, "\n", 1);
		return 1; // Child process dies after failed exec.
	 } else {
		// Code only executed by the parent process.
		char pmpt3[] = "Parent: now sleeping and waiting for child end.";
		write(1, pmpt3, strlen(pmpt3));
		write(1, "\n", 1);
		// waiting child.
		wait(&status);
		char pmpt4[] = "Parent: finished waiting for child, child is dead.";
		write(1, pmpt4, strlen(pmpt4));
		write(1, "\n", 1);
		//char pmpt5[] = "Parent: child exit status is: ";
		//write(1, pmpt5, strlen(pmpt5));
		//write(1, status, sizeof(int));
		//write(1, "\n", 1);
		return 0; // Parent process dies.
	}
	return 0;
}
