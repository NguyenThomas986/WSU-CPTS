#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <inttypes.h>

typedef enum {false, true} bool;        // Allows boolean types in C

typedef enum { UNSTARTED, READY, RUNNING, BLOCKED, TERMINATED } ProcessState;

typedef enum { FCFS, RR, SJF } Algorithm;

/* Defines a job struct */
typedef struct Process {
    uint32_t A;                         // A: Arrival time of the process
    uint32_t B;                         // B: Upper Bound of CPU burst times of the given random integer list
    uint32_t C;                         // C: Total CPU time required
    uint32_t M;                         // M: Multiplier of CPU burst time
    uint32_t processID;                 // The process ID given upon input read

    ProcessState state;
    uint32_t currentCPUTimeRun;
    uint32_t cpuBurstRemaining;
    uint32_t cpuBurstLength;             // Full burst, preserved across RR preemption
    uint32_t ioBurstRemaining;
    uint32_t quantumRemaining;
    uint32_t readySince;
    uint32_t finishingTime;
    uint32_t currentIOBlockedTime;
    uint32_t currentWaitingTime;
} _process;

uint32_t TOTAL_CREATED_PROCESSES = 0;   // The total number of processes constructed
const uint32_t SEED_VALUE = 200;  // Seed value for reading from file

uint32_t TOTAL_FINISHED_PROCESSES = 0;
uint32_t CURRENT_CYCLE = 0;
uint32_t TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED = 0;


/**
 * Reads a random non-negative integer X from a file with a given line named random-numbers (in the current directory)
 */
uint32_t getRandNumFromFile(uint32_t line, FILE* random_num_file_ptr){
    uint32_t end, loop;
    char str[512];

    rewind(random_num_file_ptr); // reset to be beginning
    for(end = loop = 0;loop<line;++loop){
        if(0==fgets(str, sizeof(str), random_num_file_ptr)){ //include '\n'
            end = 1;  //can't input (EOF)
            break;
        }
    }
    if(!end) {
        return (uint32_t) atoi(str);
    }

    // Fail-safe return
    return (uint32_t) 1804289383;
}



/**
 * Reads a random non-negative integer X from a file named random-numbers.
 * Returns the CPU Burst: : 1 + (random-number-from-file % upper_bound)
 */
uint32_t randomOS(uint32_t upper_bound, uint32_t process_indx, FILE* random_num_file_ptr)
{
    uint32_t unsigned_rand_int = (uint32_t) getRandNumFromFile(SEED_VALUE+process_indx, random_num_file_ptr);
    uint32_t returnValue = 1 + (unsigned_rand_int % upper_bound);

    return returnValue;
} 


/********************* SOME PRINTING HELPERS *********************/


/**
 * Prints to standard output the original input
 * process_list is the original processes inputted (in array form)
 */
void printStart(_process process_list[])
{
    printf("The original input was: %u", TOTAL_CREATED_PROCESSES);

    uint32_t i = 0;
    for (; i < TOTAL_CREATED_PROCESSES; ++i)
    {
        printf(" ( %u %u %u %u)", process_list[i].A, process_list[i].B,
               process_list[i].C, process_list[i].M);
    }
    printf("\n");
} 

/**
 * Prints to standard output the final output
 * sorted_process_list is ordered by arrival time, then original input position.
 */
void printFinal(_process sorted_process_list[])
{
    printf("The (sorted) input is: %u", TOTAL_CREATED_PROCESSES);

    uint32_t i = 0;
    for (; i < TOTAL_CREATED_PROCESSES; ++i)
    {
        printf(" ( %u %u %u %u)", sorted_process_list[i].A, sorted_process_list[i].B,
               sorted_process_list[i].C, sorted_process_list[i].M);
    }
    printf("\n");
} // End of the print final function

/**
 * Prints out specifics for each process  (helper function, you may need to adjust variables accordingly)
 * @param process_list The original processes inputted, in array form
 */
void printProcessSpecifics(_process process_list[])
{
    uint32_t i = 0;
    printf("\n");
    for (; i < TOTAL_CREATED_PROCESSES; ++i)
    {
        printf("Process %u:\n", process_list[i].processID);
        printf("\t(A,B,C,M) = (%u,%u,%u,%u)\n", process_list[i].A, process_list[i].B,
               process_list[i].C, process_list[i].M);
        printf("\tFinishing time: %u\n", process_list[i].finishingTime);
        printf("\tTurnaround time: %u\n", process_list[i].finishingTime - process_list[i].A);
        printf("\tI/O time: %u\n", process_list[i].currentIOBlockedTime);
        printf("\tWaiting time: %u\n", process_list[i].currentWaitingTime);
        printf("\n");
    }
} // End of the print process specifics function

/**
 * Prints out the summary data (helper function, you may need to adjust variables accordingly)
 * process_list The original processes inputted in array form
 */
void printSummaryData(_process process_list[])
{
    uint32_t i = 0;
    double total_amount_of_time_utilizing_cpu = 0.0;
    double total_amount_of_time_spent_waiting = 0.0;
    double total_turnaround_time = 0.0;
    uint32_t final_finishing_time = CURRENT_CYCLE - 1;
    for (; i < TOTAL_CREATED_PROCESSES; ++i)
    {
        total_amount_of_time_utilizing_cpu += process_list[i].currentCPUTimeRun;
        total_amount_of_time_spent_waiting += process_list[i].currentWaitingTime;
        total_turnaround_time += (process_list[i].finishingTime - process_list[i].A);
    }

    // Calculates the CPU utilisation
    double cpu_util = total_amount_of_time_utilizing_cpu / final_finishing_time;

    // Calculates the IO utilisation
    double io_util = (double) TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED / final_finishing_time;

    // Calculates the throughput (Number of processes over the final finishing time times 100)
    double throughput =  100 * ((double) TOTAL_CREATED_PROCESSES/ final_finishing_time);

    // Calculates the average turnaround time
    double avg_turnaround_time = total_turnaround_time / TOTAL_CREATED_PROCESSES;

    // Calculates the average waiting time
    double avg_waiting_time = total_amount_of_time_spent_waiting / TOTAL_CREATED_PROCESSES;

    printf("Summary Data:\n");
    printf("\tFinishing time: %u\n", CURRENT_CYCLE - 1);
    printf("\tCPU Utilisation: %6f\n", cpu_util);
    printf("\tI/O Utilisation: %6f\n", io_util);
    printf("\tThroughput: %6f processes per hundred cycles\n", throughput);
    printf("\tAverage turnaround time: %6f\n", avg_turnaround_time);
    printf("\tAverage waiting time: %6f\n", avg_waiting_time);
} // End of the print summary data function


/* Read exactly the declared tuples; any trailing comment is ignored. */
static _process *readProcesses(const char *filename)
{
    FILE *input = fopen(filename, "r");
    if (input == NULL) {
        perror(filename);
        return NULL;
    }
    if (fscanf(input, "%" SCNu32, &TOTAL_CREATED_PROCESSES) != 1 ||
        TOTAL_CREATED_PROCESSES == 0) {
        fprintf(stderr, "Input must begin with a positive process count.\n");
        fclose(input);
        return NULL;
    }
    _process *processes = calloc(TOTAL_CREATED_PROCESSES, sizeof(*processes));
    if (processes == NULL) {
        perror("calloc");
        fclose(input);
        return NULL;
    }
    for (uint32_t i = 0; i < TOTAL_CREATED_PROCESSES; ++i) {
        _process *p = &processes[i];
        char closing;
        if (fscanf(input, " ( %" SCNu32 " %" SCNu32 " %" SCNu32
                   " %" SCNu32 " %c", &p->A, &p->B, &p->C, &p->M, &closing) != 5 ||
            closing != ')' || p->B == 0 || p->C == 0) {
            fprintf(stderr, "Invalid process tuple at index %" PRIu32 ".\n", i);
            free(processes);
            fclose(input);
            return NULL;
        }
        p->processID = i;
        p->state = UNSTARTED;
    }
    fclose(input);
    return processes;
}

/* Earlier arrival, then earlier input position, breaks every priority tie. */
static bool earlierProcess(const _process *a, const _process *b)
{
    return a->A < b->A || (a->A == b->A && a->processID < b->processID);
}

static _process *chooseReady(_process processes[], Algorithm algorithm)
{
    _process *best = NULL;
    for (uint32_t i = 0; i < TOTAL_CREATED_PROCESSES; ++i) {
        _process *p = &processes[i];
        if (p->state != READY)
            continue;
        if (best == NULL) {
            best = p;
            continue;
        }
        uint32_t priority = algorithm == SJF ? p->C - p->currentCPUTimeRun : p->readySince;
        uint32_t bestPriority = algorithm == SJF ? best->C - best->currentCPUTimeRun : best->readySince;
        if (priority < bestPriority ||
            (priority == bestPriority && earlierProcess(p, best)))
            best = p;
    }
    return best;
}

/* Each iteration accounts for the interval [time, time + 1). */
static void simulate(_process processes[], FILE *random_file, Algorithm algorithm)
{
    uint32_t time = 0;
    _process *running = NULL;
    TOTAL_FINISHED_PROCESSES = 0;
    TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED = 0;
    while (TOTAL_FINISHED_PROCESSES < TOTAL_CREATED_PROCESSES) {
        for (uint32_t i = 0; i < TOTAL_CREATED_PROCESSES; ++i) {
            _process *p = &processes[i];
            if ((p->state == UNSTARTED && p->A <= time) ||
                (p->state == BLOCKED && p->ioBurstRemaining == 0)) {
                p->state = READY;
                p->readySince = time;
            }
        }
        if (running == NULL) {
            running = chooseReady(processes, algorithm);
            if (running != NULL) {
                running->state = RUNNING;
                running->quantumRemaining = 2;
                if (running->cpuBurstRemaining == 0) {
                    uint32_t burst = randomOS(running->B, running->processID, random_file);
                    uint32_t remaining = running->C - running->currentCPUTimeRun;
                    if (burst > remaining)
                        burst = remaining;
                    running->cpuBurstRemaining = burst;
                    running->cpuBurstLength = burst;
                }
            }
        }
        bool anyBlocked = false;
        for (uint32_t i = 0; i < TOTAL_CREATED_PROCESSES; ++i) {
            _process *p = &processes[i];
            if (p->state == READY)
                ++p->currentWaitingTime;
            else if (p->state == BLOCKED) {
                ++p->currentIOBlockedTime;
                --p->ioBurstRemaining;
                anyBlocked = true;
            }
        }
        if (anyBlocked)
            ++TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED;
        ++time;
        if (running != NULL) {
            ++running->currentCPUTimeRun;
            --running->cpuBurstRemaining;
            if (algorithm == RR)
                --running->quantumRemaining;
            // Termination, then blocking, then quantum expiration.
            if (running->currentCPUTimeRun == running->C) {
                running->state = TERMINATED;
                running->finishingTime = time;
                ++TOTAL_FINISHED_PROCESSES;
                running = NULL;
            } else if (running->cpuBurstRemaining == 0) {
                running->state = BLOCKED;
                running->ioBurstRemaining = running->cpuBurstLength * running->M;
                running = NULL;
            } else if (algorithm == RR && running->quantumRemaining == 0) {
                running->state = READY;
                running->readySince = time;
                running = NULL;
            }
        }
    }
    CURRENT_CYCLE = time + 1; // Starter summary helper uses CURRENT_CYCLE - 1.
}

static int compareArrival(const void *left, const void *right)
{
    const _process *a = left;
    const _process *b = right;
    if (earlierProcess(a, b))
        return -1;
    if (earlierProcess(b, a))
        return 1;
    return 0;
}

static void runAlgorithm(const _process original[], FILE *random_file, Algorithm algorithm)
{
    _process *processes = malloc(TOTAL_CREATED_PROCESSES * sizeof(*processes));
    if (processes == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    memcpy(processes, original, TOTAL_CREATED_PROCESSES * sizeof(*processes));
    const char *banners[] = {"FIRST COME FIRST SERVE", "ROUND ROBIN", "SHORTEST JOB FIRST"};
    const char *names[] = {"First Come First Serve", "Round Robin", "Shortest Job First"};
    printf("######################### START OF %s #########################\n", banners[algorithm]);
    printStart(processes);
    qsort(processes, TOTAL_CREATED_PROCESSES, sizeof(*processes), compareArrival);
    printFinal(processes);
    simulate(processes, random_file, algorithm);
    printf("\nThe scheduling algorithm used was %s\n", names[algorithm]);
    printProcessSpecifics(processes);
    printSummaryData(processes);
    printf("######################### END OF %s #########################\n", banners[algorithm]);
    free(processes);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s input-file\n", argv[0]);
        return EXIT_FAILURE;
    }
    _process *process_list = readProcesses(argv[1]);
    if (process_list == NULL)
        return EXIT_FAILURE;
    FILE *random_file = fopen("random-numbers", "r");
    if (random_file == NULL) {
        perror("random-numbers");
        free(process_list);
        return EXIT_FAILURE;
    }
    runAlgorithm(process_list, random_file, FCFS);
    runAlgorithm(process_list, random_file, RR);
    runAlgorithm(process_list, random_file, SJF);
    fclose(random_file);
    free(process_list);
    return EXIT_SUCCESS;
}
