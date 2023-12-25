#include <stdio.h>
#include <stdlib.h>



void scheduling_method();
void normal_scheduling();
void first_come_first_served();
void shortest_job_first();
void priority();
void round_robin(int Q_value);
void preemptive_mode(); 

typedef struct{
    int brust_time;
    int arrival_time;
    int priority;
    int order;
} process;

char fcfs[] = "First come first serve";
char sjf[] = "Shortest-job-first";
char p[] = "Priority";
char rr[] = "Round-Robin";
char *method = fcfs;

int q_value;

int main(int argc, char **argv[]) {

    bool p_mode = false;
    while(1){
    printf("                    CPU Scheduler Simulator");
    printf("\n1) Scheduling Method (%s)\n2) Preemptive Mode (%s)\n3) Show Results\n4) End Program", method, p_mode? "On":"Off");
    int option;
    printf("\noption > ");
    scanf("%d", &option);
    switch (option){
        case 1:
            int c;
            printf("                    Scheduling Methods\n");
                printf("1) First come first serve\n2) Shortest-job-first\n3) Priority\n4) Round-Robin\n");
                printf("Option > ");
                scanf("%d", &c);
                if (c > 4 || c < 1) continue;
                if (c == 1) method = fcfs;
                if (c == 2) method = sjf;
                if (c == 3) method = p;
                if (c == 4) {
                    method = rr;
                    printf("Enter quantem value: ");
                    scanf("%d", &q_value);
                break;
        case 2:
            if (p_mode){
                p_mode = false;
            }else{
                p_mode = true;
            }
            break;
        case 3:
            if(method == fcfs){
                first_come_first_served();
            }else if(method == sjf){
                shortest_job_first();
            }else if(method == p){
                priority();
            }else if(method == rr){
                p_mode = false;
                round_robin();
            }
            break;
        case 4:
            exit(0);
            break;
        default:
            printf("invalid input, pick an option from 1-4\n");
            break;
    }}}

}

    void first_come_first_served() {

    }

    void shortest_job_first() {

    }

    void priority() {

    }

    void round_robin(int q_value) { 
        
    }

    void preemptive_mode(int num_of_jobs){
        int clock = 0, counter = 0; 
    }