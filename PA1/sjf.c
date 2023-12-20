// Author: Ruoxin WANG
// Student ID: 2030026150
// Function Description: implement shortest job first algorithm.
// Date: 2023/04/03

#include <stdio.h>

int main() {
    // get the total number of process.
    int processNum;
    printf("Enter the number of processes to schedule: ");
    scanf("%d", &processNum);

    // declare some array to store process and their time.
    int processID[processNum], burstTime[processNum], waitingTime[processNum], turnaroundTime[processNum];
    
    // using for loop to ask usr to input each burst time.
    for (int i = 0; i < processNum; i++) {
        printf("Enter burst time of process %d: ", i + 1);
        scanf("%d", &burstTime[i]);
        // increase the number of process.
        processID[i] = i + 1;
    }

    // Using bubble sort to order the process according to time.
    for (int i = 0; i < processNum; i++) {
        for (int j = 0; j < processNum - i - 1; j++) {
            if (burstTime[j] > burstTime[j + 1]) {
            	// exchange the info of burstTime.
                int temp1 = burstTime[j];
                burstTime[j] = burstTime[j + 1];
                burstTime[j + 1] = temp1;
                
		// as well as exchange the ID of process.
                int temp2 = processID[j];
                processID[j] = processID[j + 1];
                processID[j + 1] = temp2;
            }
        }
    }

    // initialize the first time of waiting time as 0.
    waitingTime[0] = 0;
    // declare variable to store waiting time and turnaround time.
    float totalWT = 0.0, totalTT = 0.0;
    float awt = 0.0, att = 0.0;
    // then for each process, calculate their waiting time.
    for (int i = 1; i < processNum; i++) {
        waitingTime[i] = waitingTime[i - 1] + burstTime[i - 1];
        totalWT += waitingTime[i];
    }
    // calculate turnaround time for each process.
    for (int i = 0; i < processNum; i++) {
        turnaroundTime[i] = waitingTime[i] + burstTime[i];
        totalTT += turnaroundTime[i];
    }

    // print out the sequence of schedule.
    printf("Schedule sequence: ");
    for (int i = 0; i < processNum; i++) {
        printf("%d ", processID[i]);
    }
    
    // calculate AWT and ATT, then print out the result.
    awt = totalWT / processNum;
    att = totalTT / processNum;
    printf("\n");
    printf("AWT: %f\n", awt);
    printf("ATT: %f\n", att);

    return 0;
}
