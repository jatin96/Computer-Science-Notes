# Operating Systems

## Process

- A program under execution is called a process.
- The process tries to access resources.
- We need a mediator to distribute the resources optimally.


## Main job of an OS

- Memory management
- CPU scheduling 
- storage management
- security

## Multiprogramming

- Bring maximum processes to the RAM
- A single core CPU can achieve multiprogramming

## When a process leave CPU ? 
- After finishing execution
- Waiting for IO
- killed due to a bug

## Issues with the above 
- If a high priority process comes, then it might not be taken first.
- If a process is taking a lot of time, it might block other processes i.e. no time sharing allowed.

## Time sharing

- To fix the above, you can have a time quanta for the process. 
- When the time quanta is over, the other processes will get CPU.
- This approach is better than multi programming.

## Multitasking
- Doing two things at a time and switching between them without finishing the tasks.


## Non-preemptive vs Preemptive scheduling

- In non-preemptive scheduling, there is no time bound on process to leave the CPU.
- In preemptive scheduling, a process can leave a the CPU if it's time is over.
- **Multiprogramming + preemptive scheduling = multitasking**

## Multi - processing

- It is a hardware concept.
- We need multiple cores for this.
- Canot be achieved with one CPU.

## Process Control Block

- A data structure used by the OS to store all the information about the process.
- Structure of PCB
    - Process state
    - process number
    - process counter
    - registers
    - list of open files
- stored in kernel space in RAM

## Process states / Life Cycle

- When a process is started, it is in the NEW state. A PCB for it is created.
- Scheduler takes the process from NEW state, and push it to the RAM to bring them to READY state
- scheduler takes the process from READY state and moves it to the CPU. It will be in RUNNING state.
- If the process is performing IO, it will reach the WAITING STATE.

![process lifecycle](2023-01-05-23-59-59.png)


## Metrics to Measure Performance

### Throughput

- Number of processes that can be handled in 1 sec.

###  Response Time

- Time taken to start processing a task.

### CPU Utilization

- More CPU utilization, better the algorithm is.

## How to measure these metrics ? 

- **Arrival Time** : time of arrival of the process.
- **Waiting Time** : time for which the process was in waiting state.
- **Burst Time** : time needed to execute the process in 1 burst. 1 burst means assuming the process got all the cpu with no wait time etc.
- **Turn Around Time**: Time taken to complete a process. This includes wait time as well.
- **Completion Time**: Time at which the process got completed. The exact time.

![example](2023-01-06-00-57-17.png)

