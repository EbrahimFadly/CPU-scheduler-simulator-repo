# CPU Scheduler Simulator

This is a simple CPU scheduler simulator implemented in C++.
The simulator supports four scheduling algorithms: 
-  First Come First Serve (FCFS)
-  Shortest Job First (SJF) 
-  Priority Scheduling
-  Round Robin
The user can choose the scheduling method, enable or disable preemptive mode, and view the results which include process waiting times and their average waiting time.


The simulator uses a linked list to represent processes, each process has attributes such as burst time, arrival time, priority, and execution order. 
The main file (app.cpp) includes functions for each scheduling algorithm, as well as a function to display the results both in CLI and to a text file


## Usage

To use the simulator, you need to provide input and output file names as command-line arguments. 
The input file should contain process details in the following format, with each line representing a process:

```plaintext
8:5:1
1:2:6
6:1:6
```
-  The first number represents the burst time.
-  The second number represents the arrival time.
-  The third number represents the priority.

#### Compilation
```bash
gcc -o scheduler app.cpp
```

#### execution
```bash./scheduler -f input_file.txt -o output_file.txt``` 





