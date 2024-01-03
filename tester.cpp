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
} Process;

static int order = 1;

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
    return node;
}

void print_list(Process *header){
    Process *current = header;
    while(current){
        printf("\nProcess %d wait time:%d executed: %s \n", current->order, current->waiting_time, current->executed? "true":"false");
        current = current->next;
    }
}

Process *get_shortest_job(Process *header){ // returns shortest arrived process that is not executed yet
        if(!header){
            return NULL;
        }
        Process *current = header;
        Process *shortest = NULL;
        while(1){
            if(!current){
                return shortest;
            }else if(current->executed){ // check if process is executed
                if(current->next == NULL) return shortest;
                current = current->next;
            }else{
                if(shortest){ // if shortest exists
                    if(shortest->arrival_time < current->arrival_time){
                       return shortest;
                    }else if(shortest->arrival_time = current->arrival_time){
                        if(shortest->brust_time <= current->brust_time){
                            current = current->next;
                        }else{
                            shortest = current;
                            current = current->next;
                        }
                    }
                }else{ // assign a shortest if no shortest exists
                    shortest = current;
                    current = current->next;
                }
                
            }
        }
        return shortest;
}


void shortest_job_first(Process *header){
        int timer = 0;
        while(1){
            Process *current = get_shortest_job(header); // returns shortest (lowest brust) arrived process that is not executed yet
            if(current) printf("\n got process %d\n", current->order);
            if(current == NULL)return;
            if(timer == current->arrival_time){ // process running 
                current->brust_time--;
                    current->arrival_time++;
                    if(current->brust_time == 0){
                    current->executed = true;
                        printf("\n process %d is finished", current->order);
                    }
            }else if(timer > current->arrival_time){  // process running 
                current->brust_time--;
                    current->arrival_time++;
                    if(current->brust_time == 0){
                    current->executed = true;
                        printf("\n process %d is finished", current->order);
                    }
            }
            // Updating the waiting time for all processes that arrived but not finished except for current as it was running
            Process *temp = header;
            while (temp) {
                if (temp != current && !temp->executed && temp->arrival_time <= timer) {
                    temp->waiting_time++;
                }
                temp = temp->next;
            }
            timer++;
            printf("\ntime updated\n");
        }
    }


void priority_sch(Process *header, bool p_mode){
        int timer = 0;
        while(1){
            Process *current = get_priority_job(header, p_mode); // returns piority arrived process that is not executed yet
            if(current) printf("\n got process %d\n", current->order);
            if(current == NULL){
                printf("\nlist finished\n");
                return;
            }
            if(timer == current->arrival_time){ // process running 
                current->brust_time--;
                    current->arrival_time++;
                    if(current->brust_time == 0){
                    current->executed = true;
                        printf("\n process %d is finished", current->order);
                    }
            }else if(timer > current->arrival_time){  // process running 
                    current->brust_time--;
                    current->arrival_time++;
                    if(current->brust_time == 0){
                    current->executed = true;
                        printf("\n process %d is finished", current->order);
                    }
            }
            Process *temp = header;
            while (temp) {
                if (temp != current && !temp->executed && temp->arrival_time <= timer) {
                    temp->waiting_time++;
                }
                temp = temp->next;
            }
            timer++;
            printf("\ntime updated\n");
        }
    }
int main(){
    // 5:0:3 P1
    // 4:1:2 P2
    // 3:1:1 P3
    // 4:2:2 P4
    // 3:3:1 P5
    int arr[15] = {10, 0, 3,
                   5, 1, 2,
                   3, 1, 1,
                   4, 2, 2,
                   3, 3, 1};
    Process *header = NULL;
    Process *current = header; 
    for (int i = 0; i <= 12; i += 3) {
        if (header == NULL) {
            header = createProcess(arr[i], arr[i + 1], arr[i + 2]);
            current = header;
        } else {
            current->next = createProcess(arr[i], arr[i + 1], arr[i + 2]);
            current = current->next;
        }
    }
    // header->executed = true;
    // header->next->executed = true;
    // header->next->next->executed = true;
    // header->next->next->next->executed = true;
    // header->next->next->next->next->executed = true;

    // Process *tmp =  get_shortest_job(header);
    // if(tmp) printf("\nProcess %d with burst of %d and arrival of %d and an execution of %s\n", tmp->order, tmp->brust_time, tmp->arrival_time, tmp->executed? "true":"false");
    
    
    shortest_job_first(header);
    print_list(header);

    return 0;
}

