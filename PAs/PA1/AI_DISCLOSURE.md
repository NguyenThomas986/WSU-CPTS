# AI Disclosure

This file records AI assistance used while completing CPTS 360 Programming Assignment 1.

## AI Interaction — Cache Concept Learning

**Date:** 2026-09-29

**Tool:** Codex

### Prompt

Summary of the learning session, not a verbatim transcript: I asked the AI to read the assignment documentation and explain the assignment and basic cache concepts before implementation. The discussion covered cache lines, sets, `s`, `E`, `b`, hits, misses, evictions, and LRU.

### AI Response

Summary: The AI explained the simulator's purpose, memory traces and their operations, and the required deliverables. It taught cache concepts using simple examples and short practice questions, including how blocks share a set, how different addresses can belong to one block, and how LRU chooses a block to evict.

### How It Was Used

I used the explanations to understand the assignment before implementing the simulator. This session did not produce implementation code, report text, debugging changes, or implementation decisions beyond explaining the assignment's requirements.

### Student Contribution

I answered practice questions myself and asked for clarification about eviction. No implementation code was directly copied from this interaction. The final LRU practice question had not been answered when this entry was consolidated.

### Files Affected

None by the concept tutoring. Only `AI_DISCLOSURE.md` was updated to record the assistance.

### Verification

The AI read `AGENTS.md`, `README.md`, and all five pages of `pa1_documentation.pdf`, and checked the examples against the assignment requirements. No compilation or program tests were run because no code changed.

---



## AI Interaction — Part 1: Command-Line Argument Parsing

**Date:** 2026-09-29

**Tool:** Codex

### Prompt

Exact excerpts from the student's request:

> Read `AGENTS.md`, `pa1_documentation.pdf`, `cachesim.c`, `Makefile`, and `AI_DISCLOSURE.md`.

> I want you to fully implement the cache simulator, but do it in parts instead of all at once.

> Start with Part 1 only: command-line argument parsing.

> Do not continue to Part 2 until I tell you to continue.

Summary of additional instructions: Explain the part and its code in beginner-friendly language, show changes, compile and test when appropriate, update the disclosure, preserve starter functions unless the assignment requires a change, and stop after Part 1. Do not create branches.

### AI Response

Codex implemented command-line parsing, numeric validation, and help/error exit handling, and explained how they work.

### How It Was Used

The implementation was applied to `cachesim.c`.

### Student Contribution

I requested the staged implementation and this step, reviewed the explanation, and used it to understand the assignment. I learned cache concepts and answered practice questions during the learning session. Codex wrote the code for this part.

### Files Affected

- `cachesim.c`
- `AI_DISCLOSURE.md`

### Verification

The code was compiled and relevant tests were run.

## AI Interaction — Part 2: Cache Data Structures and Allocation

**Date:** 2026-09-29

**Tool:** Codex

### Prompt

Exact request:

> Continue to Part 2: cache data structures and allocation.
>
> Follow the same rules as before:
> - explain what you are adding in simple terms
> - implement only Part 2
> - keep the code beginner-friendly
> - compile/test anything relevant
> - update `AI_DISCLOSURE.md` if needed
> - show me what changed and explain it
> - stop before Part 3

### AI Response

Codex implemented the cache-line structure, array allocation with size and failure checks, initialization, and memory release, and explained how they work.

### How It Was Used

The implementation was applied to `cachesim.c`.

### Student Contribution

I requested the staged implementation and this step, reviewed the explanation, and used it to understand the assignment. I learned cache concepts and answered practice questions during the learning session. Codex wrote the code for this part.

### Files Affected

- `cachesim.c`
- `AI_DISCLOSURE.md`

### Verification

The code was compiled and relevant tests were run.

## AI Interaction — Part 3: Address Breakdown

**Date:** 2026-09-29

**Tool:** Codex

### Prompt

Exact request:

> Continue to Part 3: address breakdown into set index and tag.
>
> Follow the same rules as before:
> - explain the concept first in simple terms
> - show a small example of how an address is split into tag, set index, and block offset
> - implement only the Part 3 code needed to calculate the set index and tag
> - keep the code beginner-friendly
> - do not move on to hit/miss logic yet
> - compile/test anything relevant
> - update `AI_DISCLOSURE.md` if this materially changes submitted work
> - show me exactly what changed and explain each new line or expression
> - stop before Part 4

### AI Response

Codex implemented set-index and tag calculations using masks and shifts, including 64-bit boundary handling, and explained how they work.

### How It Was Used

The implementation was applied to `cachesim.c`.

### Student Contribution

I requested the staged implementation and this step, reviewed the explanation, and used it to understand the assignment. I learned cache concepts and answered practice questions during the learning session. Codex wrote the code for this part.

### Files Affected

- `cachesim.c`
- `AI_DISCLOSURE.md`

### Verification

The code was compiled and relevant tests were run.

## AI Interaction — Part 4: Cache Hits and Misses

**Date:** 2026-09-29

**Tool:** Codex

### Prompt

Summary: Implement most of Part 4, explain lookup and matching valid tags, leave one meaningful small piece for me to write, and wait for my attempt. Test after my piece is added, defer LRU eviction, use concise disclosure entries with accurate authorship, and stop before Part 5. Do not create branches.

Follow-up request summary: Review my hit-counter update, make only small fixes if necessary, compile/test Part 4, record accurate authorship, and stop before Part 5.

### AI Response

Codex added a cache-access helper that searches the selected set, detects matching valid tags, counts misses, and fills an empty line. It left the hit-counter update for the student, then reviewed and verified the student's `(*hit_count)++;` as correct without changes. Full-set replacement and LRU remain deferred.

### How It Was Used

The implementation was applied to `cachesim.c`. Trace processing and full-set replacement remain for later parts.

### Student Contribution

I requested this step and wrote `(*hit_count)++;` to update the hit counter. Codex wrote the rest of the helper and reviewed and tested my addition without changing it.

### Files Affected

- `cachesim.c`
- `AI_DISCLOSURE.md`

### Verification

The code compiled without warnings, and relevant Part 4 tests passed with UndefinedBehaviorSanitizer. LRU replacement and trace processing were not tested because they are not implemented yet.

## AI Interaction — Part 5: LRU Replacement

**Date:** 2026-09-29

**Tool:** Codex

### Prompt

Summary: Explain LRU and recency tracking, implement surrounding setup, and leave a manageable multi-line student task for selecting and replacing the least recently used line. Explain available variables and give a hint without providing the completed solution. Review the student's attempt before finishing and testing Part 5, keep disclosure concise with accurate authorship, and stop before Part 6. Do not create branches.

Follow-up: I supplied the LRU selection loop, eviction-counter increment, and tag/timestamp replacement statements for review.

### AI Response

Codex added an access-counter parameter, timestamp updates on hits and empty-line fills, and the starting candidate for a full-set replacement. It left the replacement logic for the student with a pseudocode hint, then reviewed and applied the student's correct loop and updates without changes to their logic.

### How It Was Used

The setup and student-written replacement logic were applied to `cachesim.c`.

### Student Contribution

I wrote the loop that selects the smallest `last_used`, the eviction-counter increment, and the assignments replacing the chosen line's tag and timestamp. Codex supplied the initial candidate and surrounding timestamp setup, applied my code, and tested it. My Part 4 hit-counter statement was preserved.

### Files Affected

- `cachesim.c`
- `AI_DISCLOSURE.md`

### Verification

The code compiled without warnings, and relevant LRU tests passed with UndefinedBehaviorSanitizer. Trace-file processing remains unimplemented and was not tested.

## AI Interaction — Part 6: Trace-File Parsing

**Date:** 2026-09-29

**Tool:** Codex

### Prompt

Summary: Explain trace format, L/S/M/I, and line-by-line reading; implement most of Part 6 while leaving a meaningful multi-line parsing task for me. Explain the task without giving the finished answer, review my attempt, and test afterward. Keep disclosure concise with accurate authorship and stop before Part 7.

Follow-up request summary: Finish the remaining simulator work in one session, including connecting trace parsing to cache accesses. Codex completed the pending parsing prerequisite under this request.

### AI Response

Codex added trace-file opening, a fgets loop, field declarations, line numbering, read-error handling, and resource cleanup. It initially left field extraction for the student, then completed sscanf parsing and validation when asked to finish the remaining work.

### How It Was Used

The implementation was applied to `cachesim.c`.

### Student Contribution

I requested this step and later requested completion of the remaining work. I did not submit Part 6 parsing code; Codex wrote the file-reading, parsing, and validation code.

### Files Affected

- `cachesim.c`
- `AI_DISCLOSURE.md`

### Verification

The code compiled without warnings, and relevant trace-reading and error-handling tests passed.

## AI Interaction — Part 7: Trace Operations

**Date:** 2026-09-29

**Tool:** Codex

### Prompt

Summary: Complete the remaining parts in one session. For Part 7, connect parsing to access_cache, ignore I, process L/S once and M twice, keep the code simple, record authorship, and do not create branches.

### AI Response

Codex connected parsed operations to access_cache, skipped I operations, and processed M as two accesses to the same address. It initialized the access counter and rejected unknown operations.

### How It Was Used

The implementation was applied to `cachesim.c`.

### Student Contribution

I requested completion of the remaining parts. Codex wrote this integration and preserved my earlier hit-counter and LRU replacement code.

### Files Affected

- `cachesim.c`
- `AI_DISCLOSURE.md`

### Verification

The code compiled without warnings, and all four assignment traces and relevant generated-trace tests passed.

## AI Interaction — Part 8: Verbose Mode

**Date:** 2026-09-29

**Tool:** Codex

### Prompt

Summary: Implement -v with the required output format, including two results for M, and compare output with the provided examples.

### AI Response

Codex added per-operation prefixes and per-access hit, miss, and eviction output, including both results for M. Output spacing matches the provided files.

### How It Was Used

The implementation was applied to `cachesim.c`.

### Student Contribution

I requested verbose mode. Codex wrote the formatting and output code.

### Files Affected

- `cachesim.c`
- `AI_DISCLOSURE.md`

### Verification

Compilation succeeded, and both provided verbose examples matched byte for byte. Additional generated-trace verbose comparisons passed.

## AI Interaction — Part 9: Memory Cleanup

**Date:** 2026-09-29

**Tool:** Codex

### Prompt

Summary: Ensure opened files are closed and dynamically allocated memory is freed, keeping cleanup simple.

### AI Response

Codex checked normal and error exits, retained fclose/free cleanup, and added handling for a file-close failure.

### How It Was Used

The implementation was applied to `cachesim.c`.

### Student Contribution

I requested cleanup verification. Codex wrote and reviewed the resource-management code.

### Files Affected

- `cachesim.c`
- `AI_DISCLOSURE.md`

### Verification

Compilation and relevant normal/error-path tests passed. AddressSanitizer and UndefinedBehaviorSanitizer checks passed with leak detection disabled because of the previously observed environment limitation; cleanup was also reviewed manually.

## AI Interaction — Part 10: Final Testing and Debugging

**Date:** 2026-09-29

**Tool:** Codex

### Prompt

Summary: Run make clean and make, fix warnings/errors, compare all four traces with the supplied expected totals, test verbose examples, and report only results actually verified. Explain the completed program and student contributions, then stop.

### AI Response

Codex built and tested the completed simulator. All four assignment totals and both verbose fixtures matched. An independent LRU model matched generated traces, and relevant argument/file-error and sanitizer checks passed. No further simulator fixes were needed after integration.

### How It Was Used

The verification confirmed the implemented simulator behavior.

### Student Contribution

I supplied the expected results and requested testing. Codex wrote and ran the temporary test scripts. My implementation contributions remain the Part 4 hit-counter update and Part 5 LRU selection/replacement code.

### Files Affected

- `AI_DISCLOSURE.md`
- Temporary verification files under `/tmp`; no submitted test files added.

### Verification

make clean and make passed without warnings. All four assignment cases, both byte-for-byte verbose comparisons, six generated traces in normal/verbose modes, and relevant error checks passed. Sanitizer runs passed with leak detection disabled; automated leak verification remains unavailable.
