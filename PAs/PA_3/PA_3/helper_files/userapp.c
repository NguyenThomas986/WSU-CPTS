#define _POSIX_C_SOURCE 200809L

#include "userapp.h"
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/* fclose checks the actual write, including any buffered output. */
int register_process(unsigned int pid)
{
    FILE *status = fopen(STATUS_PATH, "w");

    if (status == NULL) {
        perror("open status for registration");
        return -1;
    }
    if (fprintf(status, "%u\n", pid) < 0) {
        perror("write PID");
        fclose(status);
        return -1;
    }
    if (fclose(status) != 0) {
        perror("finish PID registration");
        return -1;
    }
    return 0;
}

static int print_status(void)
{
    char buffer[256];
    size_t count;
    FILE *status = fopen(STATUS_PATH, "r");

    if (status == NULL) {
        perror("open status for reading");
        return -1;
    }
    while ((count = fread(buffer, 1, sizeof(buffer), status)) > 0) {
        if (fwrite(buffer, 1, count, stdout) != count) {
            perror("print status");
            fclose(status);
            return -1;
        }
    }
    if (ferror(status)) {
        perror("read status");
        fclose(status);
        return -1;
    }
    if (fclose(status) != 0) {
        perror("close status");
        return -1;
    }
    if (fflush(stdout) != 0) {
        perror("flush output");
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    long seconds = 12;
    char *end;
    struct timespec start;
    struct timespec now;
    /* Volatile keeps the compiler from discarding the integer workload. */
    volatile uint32_t value = 1;
    unsigned int i;

    if (argc > 2) {
        fprintf(stderr, "Usage: %s [positive seconds]\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (argc == 2) {
        errno = 0;
        seconds = strtol(argv[1], &end, 10);
        if (errno != 0 || end == argv[1] || *end != '\0' ||
            seconds <= 0 || seconds > INT_MAX) {
            fprintf(stderr, "Duration must be an integer from 1 to %d.\n", INT_MAX);
            return EXIT_FAILURE;
        }
    }

    if (register_process((unsigned int)getpid()) != 0)
        return EXIT_FAILURE;
    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0) {
        perror("start clock");
        return EXIT_FAILURE;
    }

    do {
        /* Unsigned arithmetic deliberately wraps; no floating point is used. */
        for (i = 0; i < 100000; ++i)
            value = value * UINT32_C(1664525) + UINT32_C(1013904223);
        if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
            perror("read clock");
            return EXIT_FAILURE;
        }
    } while (now.tv_sec - start.tv_sec < seconds ||
             (now.tv_sec - start.tv_sec == seconds && now.tv_nsec < start.tv_nsec));

    if (print_status() != 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
