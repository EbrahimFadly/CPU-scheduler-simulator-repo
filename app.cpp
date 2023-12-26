#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>


void scheduling_method();
void normal_scheduling();
void first_come_first_served();
void shortest_job_first();
void priority_sch();
void round_robin(int Q_value);
void preemptive_mode(); 


// implementing Link list for organizing the jobs queue,
typedef struct process{
    int brust_time;
    int arrival_time;
    int priority; // lower numbers are prioritized
    int order; // order of the process according to the input file
    int waiting_time;
    process *next;
} Process;

int order = 1;

process *createProcess(int brust, int arrival, int priority){
    process *node = (process*) malloc(sizeof(process));
    node->brust_time = brust;
    node->arrival_time = arrival;
    node->priority = priority;
    node->order = order;
    order++;
    node->next = NULL; 
    return node;
}

void insert_process(process **header, int brust, int arrival, int priority) { // taking values from input file, sorted accoeding to arrival time
    Process *process = createProcess(brust, arrival, priority);
    if (*header == NULL) {
        *header = process;
        return;
    }
    Process *current = *header;
    while (current->next != NULL && current->next->arrival_time <= arrival) {
        current = current->next;
    }
    if (current->next != NULL){
        Process *next = current->next;
        process->next = next;
    }
    current->next = process;
    return;
}

char fcfs[] = "First come first serve";
char sjf[] = "Shortest-job-first";
char p[] = "Priority";
char rr[] = "Round-Robin";
char *method = fcfs;

int q_value;

int main(int argc, char *argv[]) {
    printf("%d", argc);
    if(argc != 5){
        printf("Not enough arguments");
        return 1;
    }
    
    char *input_fname, *output_fname;
    int opt;

    while((opt = getopt(argc, argv, "f:o:")) != -1){
        switch (opt)
        {
        case 'f':
            input_fname = optarg;
            break;
        case 'o':
            output_fname = optarg;
            break;
        default:
            exit(1);
            break;
        }
    }

    Process *header = NULL;
    int burst,arrival,priority;
    bool p_mode = false;
    int option;
    FILE *input_file = fopen(input_fname, "r");
    FILE *output_file = fopen(output_fname, "w"); 

    while(1){
    printf("                    CPU Scheduler Simulator");
    printf("\n1) Scheduling Method (%s)\n2) Preemptive Mode (%s)\n3) Show Results\n4) End Program", method, p_mode? "On":"Off");
    printf("\noption > ");
    scanf("%d", &option);
    switch (option){
        case 1:
            int c;
            printf("                    Scheduling Methods\n");
                printf("1) First come first serve\n2) Shortest-job-first\n3) Priority\n4) Round-Robin\n");
                printf("Option > ");
                scanf("%d", &c);
                if (c == 1) method = fcfs;
                else if (c == 2) method = sjf;
                else if (c == 3) method = p;
                else if (c == 4) {
                    method = rr;
                    printf("Enter quantem value: ");
                    scanf("%d", &q_value);
                }else {
                    printf("invalid input, pick an option from 1-4\n");
                } 
                break;
        case 2:
            if (p_mode){
                p_mode = false;
            }else{
                p_mode = true;
            }
            break;
        case 3: 
            if (input_file == NULL) {
                printf("Error opening input file\n");
                exit(1);
            }
            if (output_file == NULL) {
                printf("Error opening output file\n");
                exit(1);
            }

            while (fscanf(input_file, "%d:%d:%d\n", &burst, &arrival, &priority) != EOF) { // read processes data from file
                insert_process(&header, burst, arrival, priority);
            }

            if(method == fcfs){
                first_come_first_served();
                break;
            }else if(method == sjf){
                shortest_job_first();
                break;
            }else if(method == p){
                priority_sch();
                break;
            }else if(method == rr){
                p_mode = false;
                round_robin(q_value);
                break;
            }
            break;
        case 4:
            exit(0);
            break;
        default:
            printf("invalid input, pick an option from 1-4\n");
            break;
    }
    }

}

    void first_come_first_served() {

    }

    void shortest_job_first() {

    }

    void priority_sch() {

    }

    void round_robin(int Q_value) { // does preemptive mode apply with round-robin quantem time?!
        
    }

    void preemptive_mode(int num_of_jobs){ // does preemptive mode apply with round-robin quantem time?!
        int clock = 0, counter = 0; 
    }

    