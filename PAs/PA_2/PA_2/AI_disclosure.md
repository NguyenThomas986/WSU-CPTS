AI Disclosure

This file records AI assistance used while completing CPTS 360 Programming Assignment 2.

## Implementation request (verbatim)

Help me implement my CPU Scheduling Simulator assignment in C using the provided scheduler.c starter code.

Implement FCFS, Round Robin (quantum = 2), and non-preemptive SJF according to the assignment requirements.

Follow all CPU/I/O burst rules, process states, tie-breaking rules, and output formatting. Use the provided randomOS() function.

Instructions:

Keep the code simple, clean, and well-organized.

Preserve the existing starter code and helper functions.

Implement the assignment in small, manageable parts.

Complete and test one part before moving to the next.

Make a Git commit after each completed part with a clear, descriptive commit message.

Use the provided Makefile and test against sample outputs.

Ensure everything compiles and runs correctly on Linux.

Focus only on the C implementation, not the PDF report.

AI Disclosure:
Read ai_disclosure.md and follow its exact formatting requirements. Document all AI-generated code, prompts, and corresponding outputs. Update the disclosure as development progresses.

Workflow:

Review starter code and requirements.

Implement process structures and initialization.

Implement FCFS and test, then commit.

Implement Round Robin and test, then commit.

Implement SJF and test, then commit.

Implement statistics and output formatting, then commit.

Run all provided tests, fix issues, and make a final commit.

Work incrementally, keep commits focused, and do not skip testing or AI disclosure updates. Do not commit compiled binaries or executables.

Earlier tutoring (summary): Codex explained CPU bursts, I/O multiplication, process states, and enum/struct declarations. The student supplied ProcessState, state, currentCPUTimeRun, and cpuBurstRemaining declarations in chat. The starter file did not contain these edits when implementation began.

## Part 1: Process structures and initialization

**Date:** 2026-10-22

**Tool:** Codex

### Prompt

The verbatim implementation request above applies to this incremental part.

### AI Response

Added process states, counters, allocation, tuple parsing, and command-line validation. Removed an unused local from randomOS without changing its behavior.

Exact generated source changes:

```diff
diff --git a/PAs/PA_2/PA_2/scheduler.c b/PAs/PA_2/PA_2/scheduler.c
index ec1f6fc..e774d9b 100644
--- a/PAs/PA_2/PA_2/scheduler.c
+++ b/PAs/PA_2/PA_2/scheduler.c
@@ -3,10 +3,12 @@
 #include <string.h>
 #include <stdint.h>
 
-// Headers as needed
+#include <inttypes.h>
 
 typedef enum {false, true} bool;        // Allows boolean types in C
 
+typedef enum { UNSTARTED, READY, RUNNING, BLOCKED, TERMINATED } ProcessState;
+
 /* Defines a job struct */
 typedef struct Process {
     uint32_t A;                         // A: Arrival time of the process
@@ -15,13 +17,24 @@ typedef struct Process {
     uint32_t M;                         // M: Multiplier of CPU burst time
     uint32_t processID;                 // The process ID given upon input read
 
-    // Add other fields as needed
+    ProcessState state;
+    uint32_t currentCPUTimeRun;
+    uint32_t cpuBurstRemaining;
+    uint32_t cpuBurstLength;             // Full burst, preserved across RR preemption
+    uint32_t ioBurstRemaining;
+    uint32_t quantumRemaining;
+    uint32_t readySince;
+    uint32_t finishingTime;
+    uint32_t currentIOBlockedTime;
+    uint32_t currentWaitingTime;
 } _process;
 
 uint32_t TOTAL_CREATED_PROCESSES = 0;   // The total number of processes constructed
 const uint32_t SEED_VALUE = 200;  // Seed value for reading from file
 
-// Additional variables as needed
+uint32_t TOTAL_FINISHED_PROCESSES = 0;
+uint32_t CURRENT_CYCLE = 0;
+uint32_t TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED = 0;
 
 
 /**
@@ -54,8 +67,6 @@ uint32_t getRandNumFromFile(uint32_t line, FILE* random_num_file_ptr){
  */
 uint32_t randomOS(uint32_t upper_bound, uint32_t process_indx, FILE* random_num_file_ptr)
 {
-    char str[20];
-    
     uint32_t unsigned_rand_int = (uint32_t) getRandNumFromFile(SEED_VALUE+process_indx, random_num_file_ptr);
     uint32_t returnValue = 1 + (unsigned_rand_int % upper_bound);
 
@@ -166,17 +177,54 @@ void printSummaryData(_process process_list[])
 } // End of the print summary data function
 
 
-/**
- * The magic starts from here
- */
-int main(int argc, char *argv[])
+/* Read exactly the declared tuples; any trailing comment is ignored. */
+static _process *readProcesses(const char *filename)
 {
-    uint32_t total_num_of_process;               // Read from the file -- number of process to create
-    _process process_list[total_num_of_process]; // Creates a container for all processes
-    // Other variables
-
-
-    // Write code for your shiny scheduler
+    FILE *input = fopen(filename, "r");
+    if (input == NULL) {
+        perror(filename);
+        return NULL;
+    }
+    if (fscanf(input, "%" SCNu32, &TOTAL_CREATED_PROCESSES) != 1 ||
+        TOTAL_CREATED_PROCESSES == 0) {
+        fprintf(stderr, "Input must begin with a positive process count.\n");
+        fclose(input);
+        return NULL;
+    }
+    _process *processes = calloc(TOTAL_CREATED_PROCESSES, sizeof(*processes));
+    if (processes == NULL) {
+        perror("calloc");
+        fclose(input);
+        return NULL;
+    }
+    for (uint32_t i = 0; i < TOTAL_CREATED_PROCESSES; ++i) {
+        _process *p = &processes[i];
+        char closing;
+        if (fscanf(input, " ( %" SCNu32 " %" SCNu32 " %" SCNu32
+                   " %" SCNu32 " %c", &p->A, &p->B, &p->C, &p->M, &closing) != 5 ||
+            closing != ')' || p->B == 0 || p->C == 0) {
+            fprintf(stderr, "Invalid process tuple at index %" PRIu32 ".\n", i);
+            free(processes);
+            fclose(input);
+            return NULL;
+        }
+        p->processID = i;
+        p->state = UNSTARTED;
+    }
+    fclose(input);
+    return processes;
+}
 
-    return 0;
-} 
\ No newline at end of file
+int main(int argc, char *argv[])
+{
+    if (argc != 2) {
+        fprintf(stderr, "Usage: %s input-file\n", argv[0]);
+        return EXIT_FAILURE;
+    }
+    _process *process_list = readProcesses(argv[1]);
+    if (process_list == NULL)
+        return EXIT_FAILURE;
+    printStart(process_list);
+    free(process_list);
+    return EXIT_SUCCESS;
+}
```

### How It Was Used

Applied directly to the assignment working tree.

### Student Contribution

The student supplied the requirements and earlier declarations described above. Codex wrote and tested this implementation; no additional student edits are claimed.

### Files Affected

- scheduler.c (source changes, if shown above)
- Makefile (build changes, if shown above)
- AI_disclosure.md (this record)

### Verification

make clean and make passed. Strict C11 compilation with -Wall -Wextra -Wpedantic -Werror passed. All three parsed inputs matched their sample original-input lines. Missing argument was rejected.

## Part 2: FCFS

**Date:** 2026-10-22

**Tool:** Codex

### Prompt

The verbatim implementation request above applies to this incremental part.

### AI Response

Added the cycle simulation and FCFS selection by ready time, arrival time, then input position. Used starter reporting helpers to verify complete FCFS sample sections. Counted waiting, CPU, and I/O intervals as part of the simulation.

Exact generated source changes:

```diff
diff --git a/PAs/PA_2/PA_2/scheduler.c b/PAs/PA_2/PA_2/scheduler.c
index e774d9b..b40912e 100644
--- a/PAs/PA_2/PA_2/scheduler.c
+++ b/PAs/PA_2/PA_2/scheduler.c
@@ -215,6 +215,108 @@ static _process *readProcesses(const char *filename)
     return processes;
 }
 
+/* Earlier arrival, then earlier input position, breaks every priority tie. */
+static bool earlierProcess(const _process *a, const _process *b)
+{
+    return a->A < b->A || (a->A == b->A && a->processID < b->processID);
+}
+
+static _process *chooseReady(_process processes[])
+{
+    _process *best = NULL;
+    for (uint32_t i = 0; i < TOTAL_CREATED_PROCESSES; ++i) {
+        _process *p = &processes[i];
+        if (p->state != READY)
+            continue;
+        if (best == NULL || p->readySince < best->readySince ||
+            (p->readySince == best->readySince && earlierProcess(p, best)))
+            best = p;
+    }
+    return best;
+}
+
+/* Each iteration accounts for the interval [time, time + 1). */
+static void simulate(_process processes[], FILE *random_file)
+{
+    uint32_t time = 0;
+    _process *running = NULL;
+    TOTAL_FINISHED_PROCESSES = 0;
+    TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED = 0;
+    while (TOTAL_FINISHED_PROCESSES < TOTAL_CREATED_PROCESSES) {
+        for (uint32_t i = 0; i < TOTAL_CREATED_PROCESSES; ++i) {
+            _process *p = &processes[i];
+            if ((p->state == UNSTARTED && p->A <= time) ||
+                (p->state == BLOCKED && p->ioBurstRemaining == 0)) {
+                p->state = READY;
+                p->readySince = time;
+            }
+        }
+        if (running == NULL) {
+            running = chooseReady(processes);
+            if (running != NULL) {
+                running->state = RUNNING;
+                if (running->cpuBurstRemaining == 0) {
+                    uint32_t burst = randomOS(running->B, running->processID, random_file);
+                    uint32_t remaining = running->C - running->currentCPUTimeRun;
+                    if (burst > remaining)
+                        burst = remaining;
+                    running->cpuBurstRemaining = burst;
+                    running->cpuBurstLength = burst;
+                }
+            }
+        }
+        bool anyBlocked = false;
+        for (uint32_t i = 0; i < TOTAL_CREATED_PROCESSES; ++i) {
+            _process *p = &processes[i];
+            if (p->state == READY)
+                ++p->currentWaitingTime;
+            else if (p->state == BLOCKED) {
+                ++p->currentIOBlockedTime;
+                --p->ioBurstRemaining;
+                anyBlocked = true;
+            }
+        }
+        if (anyBlocked)
+            ++TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED;
+        ++time;
+        if (running != NULL) {
+            ++running->currentCPUTimeRun;
+            --running->cpuBurstRemaining;
+            // Termination takes priority over blocking.
+            if (running->currentCPUTimeRun == running->C) {
+                running->state = TERMINATED;
+                running->finishingTime = time;
+                ++TOTAL_FINISHED_PROCESSES;
+                running = NULL;
+            } else if (running->cpuBurstRemaining == 0) {
+                running->state = BLOCKED;
+                running->ioBurstRemaining = running->cpuBurstLength * running->M;
+                running = NULL;
+            }
+        }
+    }
+    CURRENT_CYCLE = time + 1; // Starter summary helper uses CURRENT_CYCLE - 1.
+}
+
+static void runAlgorithm(const _process original[], FILE *random_file)
+{
+    _process *processes = malloc(TOTAL_CREATED_PROCESSES * sizeof(*processes));
+    if (processes == NULL) {
+        perror("malloc");
+        exit(EXIT_FAILURE);
+    }
+    memcpy(processes, original, TOTAL_CREATED_PROCESSES * sizeof(*processes));
+    printf("######################### START OF FIRST COME FIRST SERVE #########################\n");
+    printStart(processes);
+    simulate(processes, random_file);
+    printFinal(processes);
+    printf("\nThe scheduling algorithm used was First Come First Serve\n");
+    printProcessSpecifics(processes);
+    printSummaryData(processes);
+    printf("######################### END OF FIRST COME FIRST SERVE #########################\n");
+    free(processes);
+}
+
 int main(int argc, char *argv[])
 {
     if (argc != 2) {
@@ -224,7 +326,14 @@ int main(int argc, char *argv[])
     _process *process_list = readProcesses(argv[1]);
     if (process_list == NULL)
         return EXIT_FAILURE;
-    printStart(process_list);
+    FILE *random_file = fopen("random-numbers", "r");
+    if (random_file == NULL) {
+        perror("random-numbers");
+        free(process_list);
+        return EXIT_FAILURE;
+    }
+    runAlgorithm(process_list, random_file);
+    fclose(random_file);
     free(process_list);
     return EXIT_SUCCESS;
 }
```

### How It Was Used

Applied directly to the assignment working tree.

### Student Contribution

The student supplied the requirements and earlier declarations described above. Codex wrote and tested this implementation; no additional student edits are claimed.

### Files Affected

- scheduler.c (source changes, if shown above)
- Makefile (build changes, if shown above)
- AI_disclosure.md (this record)

### Verification

All three Makefile sample targets matched their complete FCFS reference sections byte for byte. Strict warning compilation passed.
