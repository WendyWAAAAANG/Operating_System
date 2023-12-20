// Author: Ruoxin WANG
// Student ID: 2030026150
// Function Description: implement round robin algorithm.
// Date: 2023/04/03

#include <stdio.h>

int main() {
    // get the total number of process.
    int processNum;
    printf("Enter the number of processes to schedule: ");
    scanf("%d", &processNum);

    // declare some array to store process and their time.
    int burstTime[processNum], waitingTime[processNum], turnaroundTime[processNum], remainTime[processNum];

    for (int i = 0; i < processNum; i++) {
        printf("Enter burst time of process %d: ", i + 1);
        scanf("%d", &burstTime[i]);
        remainTime[i] = burstTime[i];
    }

    // ask usr to input time quantum.
    int timeQuantum = 0;
    printf("Enter the length of the time quantum q: ");
    scanf("%d", &timeQuantum);

    int currTime = 0;
    //printf("\n");
    printf("Schedule sequence: ");
    while (1) {
    	// use done to represent whether current task is finished.
        int done = 1;
        for (int i = 0; i < processNum; i++) {
            if (remainTime[i] > 0) {
            	// which means the process has not been done completely.
                done = 0;
                //printf("%d ", i + 1);
                if (remainTime[i] <= timeQuantum) {
                    // which means the process can be finished after this turn.
                    // print out current process ID.
                    for (int j = 0; j < remainTime[i]; j++) {
                        printf("%d ", i + 1);
                    }
                    // calculate remaining time, waiting time, burst time.
                    currTime += remainTime[i];
                    waitingTime[i] = currTime - burstTime[i];
                    remainTime[i] = 0;
                    
                } else {
                    // which means the process cannot be finished after this turn.
                    // print out current process ID.
                    for (int j = 0; j < timeQuantum; j++) {
                        printf("%d ", i + 1);
                    }
                    // calculate remaining time, burst time.
                    currTime += timeQuantum;
                    remainTime[i] -= timeQuantum;
                }
            }
        }
        // check whether the task is already done.
        if (done == 1) break;
    }

    // declare variable to store waiting time and turnaround time.
    float totalWT = 0.0, totalTT = 0.0;
    float awt = 0.0, att = 0.0;
    // calculate total waiting tiem and turnaround time.
    for (int i = 0; i < processNum; i++) {
        turnaroundTime[i] = waitingTime[i] + burstTime[i];
        totalWT += waitingTime[i];
        totalTT += turnaroundTime[i];
    }

    // calculate AWT and ATT, then print out the result.
    awt = totalWT / processNum;
    att = totalTT / processNum;
    printf("\n");
    printf("AWT: %f\n", awt);
    printf("ATT: %f\n", att);

    return 0;
}
