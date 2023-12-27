#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

typedef struct process{
    int brust_time;
    int arrival_time;
    int priority; // lower numbers are prioritized
    int order; // order of the process according to the input file
    int waiting_time;
    process *next;
    bool finished;
} Process;

void scheduling_method();
void normal_scheduling();
void first_come_first_served(Process *header);
void shortest_job_first(Process *header);
void priority_sch(Process *header);
void round_robin(Process *header, int Q_value);
void preemptive_mode(); 
void show_output(Process *header, char *method, bool p_mode);


int order = 1;

process *createProcess(int brust, int arrival, int priority){
    process *node = (process*) malloc(sizeof(process));
    node->brust_time = brust;
    node->arrival_time = arrival;
    node->priority = priority;
    node->order = order;
    order++;
    node->next = NULL; 
    node->finished = false;
    return node;
}

void insert_process(process **header, int brust, int arrival, int priority) { // taking values from input file, sorted accoeding to arrival time
    Process *new_p = createProcess(brust, arrival, priority);
    if (*header == NULL) {
        *header = new_p;
        printf("inserted");
        return;
    }
    Process *current = *header;
    while (current->next != NULL && current->next->arrival_time <= new_p->arrival_time) {
        current = current->next;
    }
    if (current->next != NULL){
        Process *next = current->next;
        new_p->next = next;
    }
    current->next = new_p;
    printf("inserted");
    return;
} // logical error in sorting the first entered node (ariival time) 


int main(int argc, char *argv[]) {

    char fcfs[] = "First come first serve";
    char sjf[] = "Shortest-job-first";
    char p[] = "Priority";
    char rr[] = "Round-Robin";
    char *method = fcfs;

    int q_value;

    if(argc != 5){
        printf("Not enough arguments");
        return 1;
    }
    
    char *input_fname = NULL;
    char *output_fname = NULL;
    int opt;

    while((opt = getopt(argc, argv, "f:o:")) != -1){
        switch (opt){
            case 'f':
                input_fname = optarg;
                break;
            case 'o':
                output_fname = optarg;
                break;
            default:
                printf("error in the parameters format {-f input_file -o output_file}");
                exit(1);
        }
    }
    printf("%s, %s", input_fname, output_fname);

    Process *header = NULL;
    int burst,arrival,priority;
    bool p_mode = false;
    int option;

    while(1){
    FILE *input_file = fopen(input_fname, "r");
    FILE *output_file = fopen(output_fname, "w"); 
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
            p_mode = !p_mode;
            break;
        case 3:     
            printf("reached");
            if (input_file == NULL) {
                printf("Error opening input file\n");
                exit(1);
            }
            if (output_file == NULL) {
                printf("Error opening output file\n");
                exit(1);
            }
            printf("reached");
            while (fscanf(input_file, "%d:%d:%d\n", &burst, &arrival, &priority) != EOF) { // read processes data from file
                printf("reached");
                insert_process(&header, burst, arrival, priority);
            }

            if(method == fcfs){
                p_mode = false;
                first_come_first_served(header);
                show_output(header, method, p_mode);
            }else if(method == sjf){
                shortest_job_first(header);
            }else if(method == p){
                priority_sch(header);
            }else if(method == rr){
                p_mode = false;
                round_robin(header, q_value);
            }
            fclose(input_file);
            fclose(output_file);
            printf("\nclosed");
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

    void first_come_first_served(Process *header) {
        
    }

    void shortest_job_first(Process *header) {
        Process *current = header;
        int total_t = 0;
    }

    void priority_sch(Process *header) {
        Process *current = header;
        int total_t = 0;
    }

    void round_robin(Process *header, int Q_value) { // does preemptive mode apply with round-robin quantem time?!
        Process *current = header;
        int total_t = 0;
    }

    void preemptive_mode(int num_of_jobs){ // does preemptive mode apply with round-robin quantem time?!
        int clock = 0, counter = 0; 
    }

    
    void show_output(Process *header, char *method, bool p_mode) {
        
    }