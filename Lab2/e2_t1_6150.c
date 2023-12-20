// Author: Ruoxin WANG
// Student ID: 2030026150
// Task Requirements: use system calls instead of C standard library funcitons.

#include <unistd.h>
#include <string.h>
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
	
	char output_prompt[] = "Executing command: ";
	// output the user typed command:
	write(1, output_prompt, strlen(output_prompt));
	write(1, buf, strlen(buf));
	write(1, "\n", 1);
	return 0;
}
