#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <memory>


typedef struct process{
    int brust_time;
    int arrival_time;
    int priority; // lower numbers are prioritized
    int order; // order of the process according to the input file
    int waiting_time;
    process *next;
    bool executed;
    int time_left;
    int rotation;
} Process;

static int order = 1;

void first_come_first_served(Process *header);
void round_robin(Process *header, int Q_value);
void show_output(Process *header, char *method, bool p_mode, FILE *output_file);
Process *get_process(Process *header, int timer, bool atr);
void shortest_job_first(Process *header, bool p_mode, bool atr);
void priority_sch(Process *header, bool p_mode, bool atr);


process *createProcess(int brust, int arrival, int priority){
    process *node = (process*) malloc(sizeof(process));
    node->brust_time = brust;
    node->arrival_time = arrival;
    node->priority = priority;
    node->order = order;
    order++;
    node->next = NULL; 
    node->executed = false;
    node->waiting_time = 0;
    node->time_left = brust;
    node->rotation = 0;
    return node;
}

// void delete_list(process **header){
//     Process *current = *header;
//     Process *next;
//     while(current){
//         next = current->next;
//         free(current);  
//         current = next;
//     }
//     *header = NULL;
// }

void sort_process_arrival(process **header, int brust, int arrival, int priority){ // taking values from input file, sorted accoeding to arrival time
    Process *new_p = createProcess(brust, arrival, priority);
    if (*header == NULL || new_p->arrival_time < (*header)->arrival_time){
        new_p->next = *header;
        *header = new_p;
        return;
    }
    Process *current = *header;
    while(current->next != NULL && current->next->arrival_time <= new_p->arrival_time){
        current = current->next;
    }
    if (current->next != NULL){
        Process *next = current->next;
        new_p->next = next;
    }
    current->next = new_p;
}

int main(int argc, char *argv[]){

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
    int burst,arrival,priority;
    Process *header = NULL;
    bool p_mode = false;
    int option;
    bool atr = false;
    FILE *output_file = fopen(output_fname, "a");
    FILE *input_file = fopen(input_fname, "r");
    while(fscanf(input_file, "%d:%d:%d\n", &burst, &arrival, &priority) != EOF){
        sort_process_arrival(&header, burst, arrival, priority);
    }
    fclose(input_file); 

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
                else if (c == 4){
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
            if (input_file == NULL){
                printf("Error opening input file\n");
                exit(1);
            }
            if (output_file == NULL){
                printf("Error opening output file\n");
                exit(1);
            }

            if(method == fcfs){
                p_mode = false;
                first_come_first_served(header);
                show_output(header, method, p_mode, output_file);
            }else if(method == sjf){
                shortest_job_first(header, p_mode, false);
                show_output(header, method, p_mode, output_file);      
            }else if(method == p){
                priority_sch(header, p_mode, true);
                show_output(header, method, p_mode, output_file);
            }else if(method == rr){
                p_mode = false;
                round_robin(header, q_value);
                show_output(header, method, p_mode, output_file);
            }
            exit(1);
            break;
        case 4:
            exit(0);
            break;
        default:
            printf("invalid input, pick an option from 1-4\n");
            break;
        }
        // delete_list(&header);
    }

}

    void first_come_first_served(Process *header){
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

    void shortest_job_first(Process *header, bool p_mode, bool atr){
        int timer = 0;
        while(1){
            Process *current = get_process(header, timer, atr);
            if(current->executed) return;
            if(p_mode){// ----------------------Preemptive----------------------
                if(current->arrival_time > timer){
                    current->waiting_time = timer - current->arrival_time + 1;
                }else{
                    current->brust_time--;
                }
                if(current->brust_time == 0) current->executed = true;
                Process *temp = header;
                while(temp){
                    if (temp != current && !temp->executed && temp->arrival_time <= timer) temp->waiting_time++;
                    temp = temp->next;
                }
                timer++;
            }else{// ----------------------Non-Preemptive----------------------
                if(current->arrival_time > timer) timer = current->arrival_time;
                current->executed = true;
                current->waiting_time = timer - current->arrival_time;
                timer += current->brust_time;
            }
        }
    }

    void priority_sch(Process *header, bool p_mode, bool atr){ // Complete
        int timer = 0;
        while(1){
            Process *current = get_process(header, timer, atr);
            if(current->executed) return;
            if(p_mode){// ----------------------Preemptive----------------------
                if(current->arrival_time > timer){
                    current->waiting_time = timer - current->arrival_time + 1;
                }else{
                    current->brust_time--;
                }
                if(current->brust_time == 0) current->executed = true;
                Process *temp = header;
                while(temp){
                    if (temp != current && !temp->executed && temp->arrival_time <= timer) temp->waiting_time++;
                    temp = temp->next;
                }
                timer++;
            }else{// ----------------------Non-Preemptive----------------------
                if(current->arrival_time > timer) timer = current->arrival_time;
                current->executed = true;
                current->waiting_time = timer - current->arrival_time;
                timer += current->brust_time;
            }
        }
    }
    
    void round_robin(Process *header, int Q_value){ // should be similar to shortest job first preemptive but should get the next job thats not finished and timer should be incrmented by quantem instead of timer++
        Process *current = header;
        int total_t = 0;
    }
    
    void show_output(Process *header, char *method, bool p_mode, FILE *output_file){
        Process *current = header;
        fprintf(output_file, "\noutputs%d\n");
        printf("\nScheduling Method: %s %s\n", method, p_mode ? "Preemptive" : "Non preemptive");
        printf("Process Waiting Times:\n");
        float total_wait = 0, count = 0;
        while(current){
            total_wait += current->waiting_time;
            printf("P%d %d ms\n", current->order, current->waiting_time);
            fprintf(output_file, "P%d %d ms\n", current->order, current->waiting_time);
            current = current->next;
            count++;
        }
        float avg_wait = total_wait / count;
        printf("Average Waiting Time: %f ms\n", avg_wait);
    }

    Process *get_process(Process *header, int timer, bool atr){ // returns the process based on attribute {true = priority, false = brust} 
        Process *current = header;
        Process *returned_process = current;
        while (current && current->arrival_time <= timer) {
            if(atr == true) if(!current->executed && current->priority < returned_process->priority) returned_process = current;
            if(atr == false) if(!current->executed && current->brust_time < returned_process->brust_time) returned_process = current;
            if(current->executed && returned_process == current && current->next){
                returned_process = current->next;
            }
            current = current->next;
        }
        return returned_process;
    }