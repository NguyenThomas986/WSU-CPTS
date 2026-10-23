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

## Part 3: Round Robin

**Date:** 2026-10-22

**Tool:** Codex

### Prompt

The verbatim implementation request above applies to this incremental part.

### AI Response

Added a two-unit quantum with termination before blocking before preemption. Preempted processes retain their remaining CPU burst and full original burst length for I/O calculation.

Exact generated source changes:

```diff
diff --git a/PAs/PA_2/PA_2/scheduler.c b/PAs/PA_2/PA_2/scheduler.c
index b40912e..cbc40b3 100644
--- a/PAs/PA_2/PA_2/scheduler.c
+++ b/PAs/PA_2/PA_2/scheduler.c
@@ -9,6 +9,8 @@ typedef enum {false, true} bool;        // Allows boolean types in C
 
 typedef enum { UNSTARTED, READY, RUNNING, BLOCKED, TERMINATED } ProcessState;
 
+typedef enum { FCFS, RR } Algorithm;
+
 /* Defines a job struct */
 typedef struct Process {
     uint32_t A;                         // A: Arrival time of the process
@@ -236,7 +238,7 @@ static _process *chooseReady(_process processes[])
 }
 
 /* Each iteration accounts for the interval [time, time + 1). */
-static void simulate(_process processes[], FILE *random_file)
+static void simulate(_process processes[], FILE *random_file, Algorithm algorithm)
 {
     uint32_t time = 0;
     _process *running = NULL;
@@ -255,6 +257,7 @@ static void simulate(_process processes[], FILE *random_file)
             running = chooseReady(processes);
             if (running != NULL) {
                 running->state = RUNNING;
+                running->quantumRemaining = 2;
                 if (running->cpuBurstRemaining == 0) {
                     uint32_t burst = randomOS(running->B, running->processID, random_file);
                     uint32_t remaining = running->C - running->currentCPUTimeRun;
@@ -282,7 +285,9 @@ static void simulate(_process processes[], FILE *random_file)
         if (running != NULL) {
             ++running->currentCPUTimeRun;
             --running->cpuBurstRemaining;
-            // Termination takes priority over blocking.
+            if (algorithm == RR)
+                --running->quantumRemaining;
+            // Termination, then blocking, then quantum expiration.
             if (running->currentCPUTimeRun == running->C) {
                 running->state = TERMINATED;
                 running->finishingTime = time;
@@ -292,13 +297,17 @@ static void simulate(_process processes[], FILE *random_file)
                 running->state = BLOCKED;
                 running->ioBurstRemaining = running->cpuBurstLength * running->M;
                 running = NULL;
+            } else if (algorithm == RR && running->quantumRemaining == 0) {
+                running->state = READY;
+                running->readySince = time;
+                running = NULL;
             }
         }
     }
     CURRENT_CYCLE = time + 1; // Starter summary helper uses CURRENT_CYCLE - 1.
 }
 
-static void runAlgorithm(const _process original[], FILE *random_file)
+static void runAlgorithm(const _process original[], FILE *random_file, Algorithm algorithm)
 {
     _process *processes = malloc(TOTAL_CREATED_PROCESSES * sizeof(*processes));
     if (processes == NULL) {
@@ -306,14 +315,16 @@ static void runAlgorithm(const _process original[], FILE *random_file)
         exit(EXIT_FAILURE);
     }
     memcpy(processes, original, TOTAL_CREATED_PROCESSES * sizeof(*processes));
-    printf("######################### START OF FIRST COME FIRST SERVE #########################\n");
+    const char *banners[] = {"FIRST COME FIRST SERVE", "ROUND ROBIN"};
+    const char *names[] = {"First Come First Serve", "Round Robin"};
+    printf("######################### START OF %s #########################\n", banners[algorithm]);
     printStart(processes);
-    simulate(processes, random_file);
+    simulate(processes, random_file, algorithm);
     printFinal(processes);
-    printf("\nThe scheduling algorithm used was First Come First Serve\n");
+    printf("\nThe scheduling algorithm used was %s\n", names[algorithm]);
     printProcessSpecifics(processes);
     printSummaryData(processes);
-    printf("######################### END OF FIRST COME FIRST SERVE #########################\n");
+    printf("######################### END OF %s #########################\n", banners[algorithm]);
     free(processes);
 }
 
@@ -332,7 +343,8 @@ int main(int argc, char *argv[])
         free(process_list);
         return EXIT_FAILURE;
     }
-    runAlgorithm(process_list, random_file);
+    runAlgorithm(process_list, random_file, FCFS);
+    runAlgorithm(process_list, random_file, RR);
     fclose(random_file);
     free(process_list);
     return EXIT_SUCCESS;
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

All three samples matched both implemented algorithms exactly. A strict-warning C assertion harness verified two competing long bursts finish at times 8 and 7, with waits 3 and 4. A single 12-unit job with a 10-unit burst and M=2 finished at 32 with exactly 20 I/O units and no waiting, confirming burst preservation across preemption.

## Part 4: Shortest Job First

**Date:** 2026-10-22

**Tool:** Codex

### Prompt

The verbatim implementation request above applies to this incremental part.

### AI Response

Added non-preemptive SJF using total remaining CPU work, then arrival and input-position tie breakers. A running burst continues even if a shorter job arrives.

Exact generated source changes:

```diff
diff --git a/PAs/PA_2/PA_2/scheduler.c b/PAs/PA_2/PA_2/scheduler.c
index cbc40b3..470ac2b 100644
--- a/PAs/PA_2/PA_2/scheduler.c
+++ b/PAs/PA_2/PA_2/scheduler.c
@@ -9,7 +9,7 @@ typedef enum {false, true} bool;        // Allows boolean types in C
 
 typedef enum { UNSTARTED, READY, RUNNING, BLOCKED, TERMINATED } ProcessState;
 
-typedef enum { FCFS, RR } Algorithm;
+typedef enum { FCFS, RR, SJF } Algorithm;
 
 /* Defines a job struct */
 typedef struct Process {
@@ -223,15 +223,21 @@ static bool earlierProcess(const _process *a, const _process *b)
     return a->A < b->A || (a->A == b->A && a->processID < b->processID);
 }
 
-static _process *chooseReady(_process processes[])
+static _process *chooseReady(_process processes[], Algorithm algorithm)
 {
     _process *best = NULL;
     for (uint32_t i = 0; i < TOTAL_CREATED_PROCESSES; ++i) {
         _process *p = &processes[i];
         if (p->state != READY)
             continue;
-        if (best == NULL || p->readySince < best->readySince ||
-            (p->readySince == best->readySince && earlierProcess(p, best)))
+        if (best == NULL) {
+            best = p;
+            continue;
+        }
+        uint32_t priority = algorithm == SJF ? p->C - p->currentCPUTimeRun : p->readySince;
+        uint32_t bestPriority = algorithm == SJF ? best->C - best->currentCPUTimeRun : best->readySince;
+        if (priority < bestPriority ||
+            (priority == bestPriority && earlierProcess(p, best)))
             best = p;
     }
     return best;
@@ -254,7 +260,7 @@ static void simulate(_process processes[], FILE *random_file, Algorithm algorith
             }
         }
         if (running == NULL) {
-            running = chooseReady(processes);
+            running = chooseReady(processes, algorithm);
             if (running != NULL) {
                 running->state = RUNNING;
                 running->quantumRemaining = 2;
@@ -315,8 +321,8 @@ static void runAlgorithm(const _process original[], FILE *random_file, Algorithm
         exit(EXIT_FAILURE);
     }
     memcpy(processes, original, TOTAL_CREATED_PROCESSES * sizeof(*processes));
-    const char *banners[] = {"FIRST COME FIRST SERVE", "ROUND ROBIN"};
-    const char *names[] = {"First Come First Serve", "Round Robin"};
+    const char *banners[] = {"FIRST COME FIRST SERVE", "ROUND ROBIN", "SHORTEST JOB FIRST"};
+    const char *names[] = {"First Come First Serve", "Round Robin", "Shortest Job First"};
     printf("######################### START OF %s #########################\n", banners[algorithm]);
     printStart(processes);
     simulate(processes, random_file, algorithm);
@@ -345,6 +351,7 @@ int main(int argc, char *argv[])
     }
     runAlgorithm(process_list, random_file, FCFS);
     runAlgorithm(process_list, random_file, RR);
+    runAlgorithm(process_list, random_file, SJF);
     fclose(random_file);
     free(process_list);
     return EXIT_SUCCESS;
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

All three full sample outputs matched byte for byte through their Makefile targets. Strict C harness verified non-preemption, remaining rather than original total work, and both tie breakers.

## Part 5: Statistics and output formatting

**Date:** 2026-10-22

**Tool:** Codex

### Prompt

The verbatim implementation request above applies to this incremental part.

### AI Response

Preserved starter statistics helpers, corrected unsigned output specifiers, removed an unused total, and sorted the displayed process list by arrival/input order. Enabled C11 and compiler warnings in the provided Makefile. I/O utilization counts intervals with any blocked process, separately from summed per-process I/O time.

Exact generated source changes:

```diff
diff --git a/PAs/PA_2/PA_2/Makefile b/PAs/PA_2/PA_2/Makefile
index d1a8876..4f0e32a 100644
--- a/PAs/PA_2/PA_2/Makefile
+++ b/PAs/PA_2/PA_2/Makefile
@@ -1,8 +1,8 @@
 CC = gcc
-CFLAGS = -g
+CFLAGS = -std=c11 -g -Wall -Wextra -Wpedantic
 
 scheduler: scheduler.c
-	$(CC) scheduler.c -o scheduler
+	$(CC) $(CFLAGS) scheduler.c -o scheduler
 
 test01:
 	./scheduler sample_io/input/input-1
@@ -14,4 +14,4 @@ test03:
 	./scheduler sample_io/input/input-3
 
 clean:
-	rm -f scheduler *.o *~
\ No newline at end of file
+	rm -f scheduler *.o *~
diff --git a/PAs/PA_2/PA_2/scheduler.c b/PAs/PA_2/PA_2/scheduler.c
index 470ac2b..c733f53 100644
--- a/PAs/PA_2/PA_2/scheduler.c
+++ b/PAs/PA_2/PA_2/scheduler.c
@@ -85,12 +85,12 @@ uint32_t randomOS(uint32_t upper_bound, uint32_t process_indx, FILE* random_num_
  */
 void printStart(_process process_list[])
 {
-    printf("The original input was: %i", TOTAL_CREATED_PROCESSES);
+    printf("The original input was: %u", TOTAL_CREATED_PROCESSES);
 
     uint32_t i = 0;
     for (; i < TOTAL_CREATED_PROCESSES; ++i)
     {
-        printf(" ( %i %i %i %i)", process_list[i].A, process_list[i].B,
+        printf(" ( %u %u %u %u)", process_list[i].A, process_list[i].B,
                process_list[i].C, process_list[i].M);
     }
     printf("\n");
@@ -98,17 +98,17 @@ void printStart(_process process_list[])
 
 /**
  * Prints to standard output the final output
- * finished_process_list is the terminated processes (in array form) in the order they each finished in.
+ * sorted_process_list is ordered by arrival time, then original input position.
  */
-void printFinal(_process finished_process_list[])
+void printFinal(_process sorted_process_list[])
 {
-    printf("The (sorted) input is: %i", TOTAL_CREATED_PROCESSES);
+    printf("The (sorted) input is: %u", TOTAL_CREATED_PROCESSES);
 
     uint32_t i = 0;
-    for (; i < TOTAL_FINISHED_PROCESSES; ++i)
+    for (; i < TOTAL_CREATED_PROCESSES; ++i)
     {
-        printf(" ( %i %i %i %i)", finished_process_list[i].A, finished_process_list[i].B,
-               finished_process_list[i].C, finished_process_list[i].M);
+        printf(" ( %u %u %u %u)", sorted_process_list[i].A, sorted_process_list[i].B,
+               sorted_process_list[i].C, sorted_process_list[i].M);
     }
     printf("\n");
 } // End of the print final function
@@ -123,13 +123,13 @@ void printProcessSpecifics(_process process_list[])
     printf("\n");
     for (; i < TOTAL_CREATED_PROCESSES; ++i)
     {
-        printf("Process %i:\n", process_list[i].processID);
-        printf("\t(A,B,C,M) = (%i,%i,%i,%i)\n", process_list[i].A, process_list[i].B,
+        printf("Process %u:\n", process_list[i].processID);
+        printf("\t(A,B,C,M) = (%u,%u,%u,%u)\n", process_list[i].A, process_list[i].B,
                process_list[i].C, process_list[i].M);
-        printf("\tFinishing time: %i\n", process_list[i].finishingTime);
-        printf("\tTurnaround time: %i\n", process_list[i].finishingTime - process_list[i].A);
-        printf("\tI/O time: %i\n", process_list[i].currentIOBlockedTime);
-        printf("\tWaiting time: %i\n", process_list[i].currentWaitingTime);
+        printf("\tFinishing time: %u\n", process_list[i].finishingTime);
+        printf("\tTurnaround time: %u\n", process_list[i].finishingTime - process_list[i].A);
+        printf("\tI/O time: %u\n", process_list[i].currentIOBlockedTime);
+        printf("\tWaiting time: %u\n", process_list[i].currentWaitingTime);
         printf("\n");
     }
 } // End of the print process specifics function
@@ -142,14 +142,12 @@ void printSummaryData(_process process_list[])
 {
     uint32_t i = 0;
     double total_amount_of_time_utilizing_cpu = 0.0;
-    double total_amount_of_time_io_blocked = 0.0;
     double total_amount_of_time_spent_waiting = 0.0;
     double total_turnaround_time = 0.0;
     uint32_t final_finishing_time = CURRENT_CYCLE - 1;
     for (; i < TOTAL_CREATED_PROCESSES; ++i)
     {
         total_amount_of_time_utilizing_cpu += process_list[i].currentCPUTimeRun;
-        total_amount_of_time_io_blocked += process_list[i].currentIOBlockedTime;
         total_amount_of_time_spent_waiting += process_list[i].currentWaitingTime;
         total_turnaround_time += (process_list[i].finishingTime - process_list[i].A);
     }
@@ -170,7 +168,7 @@ void printSummaryData(_process process_list[])
     double avg_waiting_time = total_amount_of_time_spent_waiting / TOTAL_CREATED_PROCESSES;
 
     printf("Summary Data:\n");
-    printf("\tFinishing time: %i\n", CURRENT_CYCLE - 1);
+    printf("\tFinishing time: %u\n", CURRENT_CYCLE - 1);
     printf("\tCPU Utilisation: %6f\n", cpu_util);
     printf("\tI/O Utilisation: %6f\n", io_util);
     printf("\tThroughput: %6f processes per hundred cycles\n", throughput);
@@ -313,6 +311,17 @@ static void simulate(_process processes[], FILE *random_file, Algorithm algorith
     CURRENT_CYCLE = time + 1; // Starter summary helper uses CURRENT_CYCLE - 1.
 }
 
+static int compareArrival(const void *left, const void *right)
+{
+    const _process *a = left;
+    const _process *b = right;
+    if (earlierProcess(a, b))
+        return -1;
+    if (earlierProcess(b, a))
+        return 1;
+    return 0;
+}
+
 static void runAlgorithm(const _process original[], FILE *random_file, Algorithm algorithm)
 {
     _process *processes = malloc(TOTAL_CREATED_PROCESSES * sizeof(*processes));
@@ -325,8 +334,9 @@ static void runAlgorithm(const _process original[], FILE *random_file, Algorithm
     const char *names[] = {"First Come First Serve", "Round Robin", "Shortest Job First"};
     printf("######################### START OF %s #########################\n", banners[algorithm]);
     printStart(processes);
-    simulate(processes, random_file, algorithm);
+    qsort(processes, TOTAL_CREATED_PROCESSES, sizeof(*processes), compareArrival);
     printFinal(processes);
+    simulate(processes, random_file, algorithm);
     printf("\nThe scheduling algorithm used was %s\n", names[algorithm]);
     printProcessSpecifics(processes);
     printSummaryData(processes);
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

make clean and make passed without warnings. All three full sample outputs matched byte for byte. A strict C assertion harness verified overlapping I/O counts 4 busy intervals rather than 6 summed process intervals, turnaround equals CPU plus I/O plus waiting, arrival at time 5 handles initial idle time, and zero-duration I/O does not create extra waiting.

## Disclosure restoration

**Date:** 2026-10-22

**Tool:** Codex

### Prompt

ok I accidnetally cleared the AI-disclousre recreate what we had before

### AI Response

Restored parts 1–4 exactly from commit 129cb33, then appended the completed statistics/output work as Part 5.

### How It Was Used

Recovered the accidentally cleared disclosure without reconstructing or inventing earlier records.

### Student Contribution

The student reported accidentally clearing the file and requested restoration.

### Files Affected

- AI_disclosure.md

### Verification

Compared restored bytes against the committed file before appending the new entries; they matched exactly.

## Part 6: Final verification

**Date:** 2026-10-22

**Tool:** Codex

### Prompt

The verbatim implementation request above applies to this incremental part.

### AI Response

Completed the final clean build, sample comparisons, focused C assertion tests, undefined-behavior sanitizer checks, and input validation checks. Removed the compiled scheduler with make clean. No additional source fixes were needed.

Exact generated source changes:

```diff
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

make clean and make passed without warnings. make test01, test02, and test03 matched complete supplied outputs byte for byte. All focused tests passed when rebuilt with -Werror and undefined-behavior sanitizer. Verified stable arrival sorting and trailing comments; rejected zero process count, incomplete tuples, zero burst bound, malformed closing parenthesis, and missing file. git diff --check passed. No binaries were staged.

### Generated verification code

These temporary test harnesses were generated and run by Codex. They are reproduced here for transparency and can be recreated outside the submission directory. The C harnesses include scheduler.c with its main renamed, allowing assertions against the simulation state.

pa2_samples.py:

```python
import subprocess,sys
from pathlib import Path
root=Path('PA_2/PA_2')
n=int(sys.argv[1])
for i in range(1,4):
 actual=subprocess.check_output(['make','-s',f'test0{i}'],cwd=root,text=True)
 expected=(root/f'sample_io/output/summary/output-{i}').read_text()
 parts=expected.split('######################### START OF ')
 expected=parts[0]+''.join('######################### START OF '+p for p in parts[1:n+1])
 assert actual==expected, f'Sample {i} differs'
print(f'All three samples match exactly for {n} implemented algorithm(s).')
```

pa2_rr_test.c:

```c
#define main scheduler_main
#include "/home/vboxuser/WSU-CPTS/PAs/PA_2/PA_2/scheduler.c"
#undef main
#include <assert.h>

int main(void)
{
    FILE *random_file = fopen("random-numbers", "r");
    assert(random_file != NULL);
    TOTAL_CREATED_PROCESSES = 2;
    _process pair[] = {
        {.A=0, .B=10, .C=5, .M=2, .processID=0},
        {.A=0, .B=10, .C=3, .M=2, .processID=1}
    };
    simulate(pair, random_file, RR);
    assert(pair[0].finishingTime == 8 && pair[1].finishingTime == 7);
    assert(pair[0].currentWaitingTime == 3 && pair[1].currentWaitingTime == 4);
    assert(pair[0].currentIOBlockedTime == 0 && pair[1].currentIOBlockedTime == 0);
    TOTAL_CREATED_PROCESSES = 1;
    _process single[] = {{.A=0, .B=10, .C=12, .M=2, .processID=0}};
    simulate(single, random_file, RR);
    assert(single[0].finishingTime == 32);
    assert(single[0].currentIOBlockedTime == 20);
    assert(single[0].currentWaitingTime == 0);
    fclose(random_file);
    puts("RR: preemption, burst preservation, I/O length, and termination checks passed.");
}
```

pa2_sjf_test.c:

```c
#define main scheduler_main
#include "/home/vboxuser/WSU-CPTS/PAs/PA_2/PA_2/scheduler.c"
#undef main
#include <assert.h>
int main(void)
{
    FILE *f = fopen("random-numbers", "r");
    assert(f);
    TOTAL_CREATED_PROCESSES = 2;
    _process p[] = {{.A=0,.B=10,.C=5,.M=1,.processID=0},
                    {.A=1,.B=10,.C=1,.M=1,.processID=1}};
    simulate(p, f, SJF);
    assert(p[0].finishingTime == 5 && p[1].finishingTime == 6);
    _process ready[] = {{.A=0,.C=10,.currentCPUTimeRun=8,.state=READY,.processID=0},
                        {.A=0,.C=3,.state=READY,.processID=1}};
    assert(chooseReady(ready, SJF) == &ready[0]);
    ready[1].C = 2;
    assert(chooseReady(ready, SJF) == &ready[0]);
    ready[0].A = 1;
    assert(chooseReady(ready, SJF) == &ready[1]);
    fclose(f);
    puts("SJF: non-preemption, remaining-work priority, and both tie breakers passed.");
}
```

pa2_stats_test.c:

```c
#define main scheduler_main
#include "/home/vboxuser/WSU-CPTS/PAs/PA_2/PA_2/scheduler.c"
#undef main
#include <assert.h>
int main(void)
{
    FILE *f = fopen("random-numbers", "r");
    assert(f);
    for (Algorithm a = FCFS; a <= SJF; ++a) {
        TOTAL_CREATED_PROCESSES = 2;
        _process p[] = {{.A=0,.B=1,.C=2,.M=3,.processID=0},
                        {.A=0,.B=1,.C=2,.M=3,.processID=1}};
        simulate(p, f, a);
        assert(p[0].currentIOBlockedTime + p[1].currentIOBlockedTime == 6);
        assert(TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED == 4);
        assert(CURRENT_CYCLE - 1 == 6);
        for (unsigned i=0; i<2; ++i)
            assert(p[i].finishingTime - p[i].A == p[i].C +
                   p[i].currentWaitingTime + p[i].currentIOBlockedTime);
        TOTAL_CREATED_PROCESSES = 1;
        _process late[] = {{.A=5,.B=1,.C=2,.M=0}};
        simulate(late, f, a);
        assert(late[0].finishingTime == 7 && late[0].currentWaitingTime == 0);
        assert(TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED == 0);
    }
    fclose(f);
    puts("Statistics: overlapping I/O, turnaround identity, idle arrival, zero I/O passed.");
}
```
