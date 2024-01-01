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
void show_output(Process *header, char *method, bool p_mode, FILE *output_file);
// Process *find_shortest_process(Process *header);
// Process *find_priority_process(Process *header);

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

void delete_list(process **header) {
    Process* current = *header;
    Process* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *header = NULL;
}

void sort_process_arrival(process **header, int brust, int arrival, int priority) { // taking values from input file, sorted accoeding to arrival time
    Process *new_p = createProcess(brust, arrival, priority);
    if (*header == NULL || new_p->arrival_time < (*header)->arrival_time) {
        new_p->next = *header;
        *header = new_p;
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
    printf("\nP%d\n", current->order);
    current->next = new_p;
}

void sort_process_shortest(process **header, int brust, int arrival, int priority) {
    Process *new_p = createProcess(brust, arrival, priority);
    if (*header == NULL || new_p->brust_time < (*header)->brust_time) {
        new_p->next = *header;
        *header = new_p;
        return;
    }
    Process *current = *header;
    while (current->next != NULL && current->next->brust_time <= new_p->brust_time) {
        current = current->next;
    }
    if (current->next != NULL){
        Process *next = current->next;
        new_p->next = next;
    }
    printf("\nP%d\n", current->order);
    current->next = new_p;
}

void sort_process_priority(process **header, int brust, int arrival, int priority) {
    Process *new_p = createProcess(brust, arrival, priority);
    if (*header == NULL || new_p->priority < (*header)->priority) {
        new_p->next = *header;
        *header = new_p;
        return;
    }
    Process *current = *header;
    while (current->next != NULL && current->next->priority <= new_p->priority) {
        current = current->next;
    }
    if (current->next != NULL){
        Process *next = current->next;
        new_p->next = next;
    }
    printf("\nP%d\n", current->order);
    current->next = new_p;
}

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

    Process *header = NULL;
    bool p_mode = false;
    int option;
    

    while(1){
    int burst,arrival,priority;
    FILE *input_file = fopen(input_fname, "r");
    FILE *output_file = fopen(output_fname, "a"); 
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
            if (input_file == NULL) {
                printf("Error opening input file\n");
                exit(1);
            }
            if (output_file == NULL) {
                printf("Error opening output file\n");
                exit(1);
            }

            if(method == fcfs){
                delete_list(&header);
                while (fscanf(input_file, "%d:%d:%d\n", &burst, &arrival, &priority) != EOF) {
                    sort_process_arrival(&header, burst, arrival, priority);
                }
                fclose(input_file);
                p_mode = false;
                first_come_first_served(header);
                show_output(header, method, p_mode, output_file);
            }else if(method == sjf){
                delete_list(&header);
                while (fscanf(input_file, "%d:%d:%d\n", &burst, &arrival, &priority) != EOF) {
                    sort_process_shortest(&header, burst, arrival, priority);
                }
                fclose(input_file);
                shortest_job_first(header);
                show_output(header, method, p_mode, output_file);                
            }else if(method == p){
                delete_list(&header);
                while (fscanf(input_file, "%d:%d:%d\n", &burst, &arrival, &priority) != EOF) {
                    sort_process_priority(&header, burst, arrival, priority);
                }
                fclose(input_file);
                priority_sch(header);
                show_output(header, method, p_mode, output_file);
            }else if(method == rr){
                delete_list(&header);
                while (fscanf(input_file, "%d:%d:%d\n", &burst, &arrival, &priority) != EOF) {
                    sort_process_arrival(&header, burst, arrival, priority);
                }
                fclose(input_file);
                p_mode = false;
                round_robin(header, q_value);
                show_output(header, method, p_mode, output_file);
            }
            fclose(output_file);
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
        Process *current = header;
        int total_t = 0;
        while(current){
            if(total_t >= current->arrival_time){ 
                current->waiting_time = 0;
            }else{
                current->waiting_time = current->arrival_time - total_t;
                total_t = current->waiting_time + total_t;
            }
            total_t = current->brust_time + total_t;
            current = current->next; 
        }
    }

    // Process *find_shortest_process(Process *header){
    //     if(!header){
    //         return NULL;
    //     }else{
    //         Process *shortest = header;
    //         Process *current = header->next;
    //         while(current){
    //             if(current->brust_time >= shortest->brust_time){
    //                 current = current->next;
    //             }else{
    //                 shortest = current;
    //             }
    //         }
    //         return shortest;
    //     }
    // }

    void shortest_job_first(Process *header) {
        Process *current = header;
        int total_t = 0;
        while(current){
            if(total_t >= current->arrival_time){ 
                current->waiting_time = 0;
            }else{
                current->waiting_time = current->arrival_time - total_t;
                total_t = current->waiting_time + total_t;
            }
            total_t = current->brust_time + total_t;
            current = current->next; 
        }
    }

    void priority_sch(Process *header) {
        Process *current = header;
        int total_t = 0;
        while(current){
            if(total_t >= current->arrival_time){ 
                current->waiting_time = 0;
            }else{
                current->waiting_time = current->arrival_time - total_t;
                total_t = current->waiting_time + total_t;
            }
            total_t = current->brust_time + total_t;
            current = current->next; 
        }
    }

    void round_robin(Process *header, int Q_value) { // does preemptive mode apply with round-robin quantem time?!
        Process *current = header;
        int total_t = 0;
    }

    void preemptive_mode(int num_of_jobs){ // does preemptive mode apply with round-robin quantem time?!
        int clock = 0, counter = 0; 
    }

    
    void show_output(Process *header, char *method, bool p_mode, FILE *output_file) {
        Process *current = header;
        fprintf(output_file, "\noutputs%d\n");
        printf("\nScheduling Method: %s %s\n", method, p_mode ? "Preemptive" : "Non preemptive");
        printf("Process Waiting Times:\n");
        int total_wait = 0, count = 0;
        while (current) {
            total_wait += current->waiting_time;
            printf("P%d %d ms\n", current->order, current->waiting_time);
            fprintf(output_file, "P%d %d ms\n", current->order, current->waiting_time);
            current = current->next;
            count++;
        }
        float avg_wait = total_wait / count;
        printf("Average Waiting Time: %.2f ms\n", avg_wait);
    }