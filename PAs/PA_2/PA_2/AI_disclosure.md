# AI Disclosure

This file records AI assistance used while completing CPTS 360 Programming Assignment 2.

## AI Interaction — Learning CPU Scheduling

**Date:** 2026-10-22

**Tool:** ChatGPT / Codex

### Prompt

I asked AI to explain CPU bursts, I/O bursts, process states, and how the scheduling algorithms work.

### AI Response

AI explained the scheduling concepts using simple examples and helped me understand the process states and CPU/I/O calculations.

### How It Was Used

I used these explanations to better understand the assignment before working on the simulator.

### Student Contribution

I worked through practice questions, checked CPU and I/O calculations, and practiced declaring process states and variables.

### Files Affected

None directly during the learning session.

---

## AI Interaction — Part 1: Process Setup

**Date:** 2026-10-22

**Tool:** Codex

### Prompt

I asked Codex to help implement the assignment in smaller parts, starting with process structures and initialization.

### AI Response

Codex implemented process states, tracking variables, input parsing, and initialization.

### How It Was Used

The generated code was added to `scheduler.c`.

### Student Contribution

I reviewed the process structure, worked through the state declarations during tutoring, and provided the assignment requirements.

### Files Affected

- `scheduler.c`
- `AI_disclosure.md`

### Verification

The program compiled, and the sample inputs were parsed successfully.

---

## AI Interaction — Part 2: First Come First Served

**Date:** 2026-10-22

**Tool:** Codex

### Prompt

I requested the FCFS implementation using the assignment's scheduling and tie-breaking rules.

### AI Response

Codex implemented FCFS scheduling, process transitions, and CPU/I/O time tracking.

### How It Was Used

The implementation was applied to `scheduler.c`.

### Student Contribution

I provided the requirements and reviewed the scheduling logic and how processes move between states.

### Files Affected

- `scheduler.c`
- `AI_disclosure.md`

### Verification

The FCFS results matched all three provided sample outputs.

---

## AI Interaction — Part 3: Round Robin

**Date:** 2026-10-22

**Tool:** Codex

### Prompt

I requested Round Robin scheduling with a quantum of 2 and the required event priorities.

### AI Response

Codex implemented CPU preemption and maintained remaining CPU burst time when processes were interrupted.

### How It Was Used

The implementation was applied to `scheduler.c`.

### Student Contribution

I reviewed how the quantum works and how termination, blocking, and preemption are handled.

### Files Affected

- `scheduler.c`
- `AI_disclosure.md`

### Verification

The Round Robin results matched the provided sample outputs, and additional scheduling checks passed.

---

## AI Interaction — Part 4: Shortest Job First

**Date:** 2026-10-22

**Tool:** Codex

### Prompt

I requested non-preemptive SJF scheduling based on remaining CPU time and the assignment's tie-breaking rules.

### AI Response

Codex implemented SJF process selection using remaining CPU work, arrival time, and input order.

### How It Was Used

The implementation was applied to `scheduler.c`.

### Student Contribution

I provided the requirements and reviewed how SJF selects processes and handles ties.

### Files Affected

- `scheduler.c`
- `AI_disclosure.md`

### Verification

The SJF results matched all three sample outputs.

---

## AI Interaction — Part 5: Statistics and Output

**Date:** 2026-10-22

**Tool:** Codex

### Prompt

I requested statistics calculations and output formatting that matched the provided sample files.

### AI Response

Codex updated the output formatting, process ordering, and Makefile while preserving the provided reporting helpers.

### How It Was Used

The changes were applied to `scheduler.c` and `Makefile`.

### Student Contribution

I supplied the expected output requirements and reviewed the results for formatting and correctness.

### Files Affected

- `scheduler.c`
- `Makefile`
- `AI_disclosure.md`

### Verification

All three sample outputs matched the expected results.

---

## AI Interaction — Part 6: Final Testing

**Date:** 2026-10-22

**Tool:** Codex / ChatGPT

### Prompt

I requested final compilation, testing, and comparison against the expected outputs.

### AI Response

Codex ran compilation and additional verification tests. ChatGPT provided terminal commands for comparing the simulator output with the reference files.

### How It Was Used

The tests helped verify that the simulator produced the expected results.

### Student Contribution

I ran `make test01`, `make test02`, and `make test03` in my Ubuntu VM. I also compared the program outputs with the expected files using `diff` and confirmed that all three passed.

### Files Affected

No additional implementation changes were needed.

### Verification

All three provided test cases passed and matched their expected outputs.
