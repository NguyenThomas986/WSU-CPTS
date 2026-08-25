# CPTS 360 — Systems Programming

Washington State University

## Course Overview

CPTS 360 focuses on systems programming and the development of software that interacts closely with computer systems and operating systems. The course covers C programming, memory management, processes, scheduling, inter-process communication, networking, concurrency, and Unix/Linux system abstractions.

**Credits:** 3
**Instructor:** Monowar Hasan
**Textbooks:** *Computer Systems: A Programmer's Perspective*, 3rd Edition — Randal E. Bryant and David R. O'Hallaron; *Operating Systems: Three Easy Pieces* — Remzi H. Arpaci-Dusseau and Andrea C. Arpaci-Dusseau

## Topics

* C programming and systems programming fundamentals
* Computer memory and memory layout
* Cache memory
* Linking
* Library interpositioning
* CPU scheduling

  * First-Come, First-Served
  * Round Robin
  * Shortest Job First
* Kernel programming
* Virtual memory
* Memory-related bugs
* Process control
* Signals
* I/O and network programming
* Network programming
* Concurrency
* Concurrency issues and bugs

## Coursework

### Programming Assignments

Five individual programming assignments focused on practical systems programming.

* [PA 0 — Unix Filesystem Tree Simulator](./programming-assignments/pa-00)
* [PA 1 — Cache Memory Simulator](./programming-assignments/pa-01)
* [PA 2 — CPU Schedulers](./programming-assignments/pa-02)
* [PA 3 — Linux Kernel Programming](./programming-assignments/pa-03)
* [PA 4 — Web Proxy](./programming-assignments/pa-04)

### Exams

* Exam 1
* Exam 2

## Learning Goals

By completing this course, I aim to develop the ability to:

* Write and debug systems software in C.
* Understand how programs are represented and executed in memory.
* Work with Unix/Linux system abstractions and interfaces.
* Understand processes, scheduling, and process control.
* Develop programs using signals and other operating system mechanisms.
* Understand virtual memory and identify memory-related bugs.
* Develop networked applications using sockets.
* Apply concurrency concepts to systems software.
* Work effectively in a Linux development environment.

## Repository Structure

```text
cpts-360/
├── README.md
├── programming-assignments/
│   ├── pa-00/
│   ├── pa-01/
│   ├── pa-02/
│   ├── pa-03/
│   └── pa-04/
├── notes/
│   ├── c/
│   ├── memory/
│   ├── linking/
│   ├── scheduling/
│   ├── kernel/
│   ├── virtual-memory/
│   ├── processes/
│   ├── networking/
│   └── concurrency/
└── exams/
    ├── exam-01/
    └── exam-02/
```

## Resources

* [CPTS 360 Course Website](https://wsucpts.gitbook.io/cpts360fa26)
* [Computer Systems: A Programmer's Perspective](https://www.pearson.com/en-us/subject-catalog/p/computer-systems-a-programmers-perspective/P200000003479/9780134092669)
* [Operating Systems: Three Easy Pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/)
* [Linux Kernel Module Programming Guide](https://sysprog21.github.io/lkmpg/)
* [GNU C Reference](https://www.gnu.org/software/gnu-c-manual/gnu-c-manual.html)
* [Git Book](https://git-scm.com/book)
* [Linux Kernel Documentation](https://www.kernel.org/doc/html/latest/)

---

*Part of my WSU CPTS coursework repository.*
