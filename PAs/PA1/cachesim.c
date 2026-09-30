#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <inttypes.h>
// other headers as needed

#define ADDRESS_LENGTH 64  // 64-bit memory addressing

typedef struct {
    int valid;           // 0 means this line is empty
    uint64_t tag;        // identifies the block stored in this line
    uint64_t last_used;  // access number for future LRU tracking
} CacheLine;


/* 
 * this function provides a standard way for your cache
 * simulator to display its final statistics (i.e., hit and miss)
 */ 
void print_summary(int hits, int misses, int evictions)
{
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
}

/*
 * print usage info
 */
void print_usage(char* argv[])
{
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/trace01.dat\n", argv[0]);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/trace01.dat\n", argv[0]);
}

/* Convert a nonnegative decimal integer; return -1 for invalid input. */
int parse_number(const char *text)
{
    char *end;
    long value;

    errno = 0;
    value = strtol(text, &end, 10);
    if (text == end || *end != '\0' || errno == ERANGE ||
        value < 0 || value > INT_MAX) {
        return -1;
    }
    return (int)value;
}

/* These helpers use s and b already checked by argument parsing. */
uint64_t get_set_index(uint64_t address, int s, int b)
{
    uint64_t set_mask = ((uint64_t)1 << s) - 1;
    return (address >> b) & set_mask;
}

uint64_t get_tag(uint64_t address, int s, int b)
{
    /* If all 64 bits are used, no tag bits remain. */
    if (s + b == ADDRESS_LENGTH) {
        return 0;
    }
    return address >> (s + b);
}

/* Process one access and track when each line was last used. */
void access_cache(CacheLine *cache, int s, int E, int b, uint64_t address,
                  int *hit_count, int *miss_count, int *eviction_count,
                  uint64_t *access_count, int verbose)
{
    size_t set_index = (size_t)get_set_index(address, s, b);
    uint64_t tag = get_tag(address, s, b);
    size_t first_line = set_index * (size_t)E;
    int empty_line = -1;

    /* The caller starts this counter at zero for each simulation. */
    (*access_count)++;

    for (int i = 0; i < E; i++) {
        size_t index = first_line + (size_t)i;

        if (cache[index].valid && cache[index].tag == tag) {
            (*hit_count)++;
            cache[index].last_used = *access_count;
            if (verbose) {
                printf("hit ");
            }
            return;
        }

        if (!cache[index].valid && empty_line == -1) {
            empty_line = i;
        }
    }

    (*miss_count)++;
    if (verbose) {
        printf("miss ");
    }
    if (empty_line != -1) {
        size_t index = first_line + (size_t)empty_line;
        cache[index].valid = 1;
        cache[index].tag = tag;
        cache[index].last_used = *access_count;
        return;
    }

    /* No empty line: all E lines in this set are valid. */
    size_t lru_index = first_line;
    for (int i = 1; i < E; i++) {
        size_t index = first_line + i;

        if (cache[index].last_used < cache[lru_index].last_used) {
            lru_index = index;
        }
    }

    (*eviction_count)++;
    cache[lru_index].tag = tag;
    cache[lru_index].last_used = *access_count;
    if (verbose) {
        printf("eviction ");
    }
}

/*
 * starting point
 */
int main(int argc, char* argv[])
{
    int s = -1;
    int E = -1;
    int b = -1;
    int verbose = 0;
    char *trace_file = NULL;
    int option;
    int hit_count = 0;
    int miss_count = 0;
    int eviction_count = 0;
    uint64_t access_count = 0;

    while ((option = getopt(argc, argv, "s:E:b:t:vh")) != -1) {
        switch (option) {
            case 's':
                s = parse_number(optarg);
                if (s < 0 || s >= ADDRESS_LENGTH) {
                    fprintf(stderr, "Invalid -s: use an integer from 0 to 63.\n");
                    print_usage(argv);
                    return 1;
                }
                break;
            case 'E':
                E = parse_number(optarg);
                if (E <= 0) {
                    fprintf(stderr, "Invalid -E: use a positive integer within int range.\n");
                    print_usage(argv);
                    return 1;
                }
                break;
            case 'b':
                b = parse_number(optarg);
                if (b < 0 || b >= ADDRESS_LENGTH) {
                    fprintf(stderr, "Invalid -b: use an integer from 0 to 63.\n");
                    print_usage(argv);
                    return 1;
                }
                break;
            case 't':
                trace_file = optarg;
                break;
            case 'v':
                verbose = 1;
                break;
            case 'h':
                print_usage(argv);
                return 0;
            default:
                print_usage(argv);
                return 1;
        }
    }

    if (s < 0 || E < 0 || b < 0 || trace_file == NULL ||
        trace_file[0] == '\0' || optind < argc) {
        fprintf(stderr, "Provide -s, -E, -b, and -t with no extra arguments.\n");
        print_usage(argv);
        return 1;
    }
    if (s + b > ADDRESS_LENGTH) {
        fprintf(stderr, "Set and block bits cannot total more than 64.\n");
        print_usage(argv);
        return 1;
    }

    /* Store each set's E lines together in one array. */
    if (s >= (int)(sizeof(size_t) * CHAR_BIT)) {
        fprintf(stderr, "Too many sets for this system.\n");
        return 1;
    }
    size_t set_count = (size_t)1 << s;
    if (set_count > SIZE_MAX / sizeof(CacheLine) / (size_t)E) {
        fprintf(stderr, "Requested cache size is too large.\n");
        return 1;
    }
    size_t line_count = set_count * (size_t)E;
    CacheLine *cache = malloc(line_count * sizeof(CacheLine));
    if (cache == NULL) {
        fprintf(stderr, "Could not allocate cache memory.\n");
        return 1;
    }

    for (size_t i = 0; i < line_count; i++) {
        cache[i].valid = 0;
        cache[i].tag = 0;
        cache[i].last_used = 0;
    }

    FILE *trace = fopen(trace_file, "r");
    if (trace == NULL) {
        perror(trace_file);
        free(cache);
        return 1;
    }

    char line[256];
    size_t line_number = 0;
    while (fgets(line, sizeof(line), trace) != NULL) {
        line_number++;
        char operation;
        uint64_t address;
        unsigned int size;
        int fields_read;
        char extra;

        /* A fourth field catches unexpected text after the byte count. */
        fields_read = sscanf(line, " %c %" SCNx64 ",%u %c",
                             &operation, &address, &size, &extra);
        if (fields_read != 3 || size == 0) {
            fprintf(stderr, "Invalid trace line %zu.\n", line_number);
            fclose(trace);
            free(cache);
            return 1;
        }

        if (operation == 'I') {
            continue;
        }
        if (operation != 'L' && operation != 'S' && operation != 'M') {
            fprintf(stderr, "Unknown operation on trace line %zu.\n", line_number);
            fclose(trace);
            free(cache);
            return 1;
        }

        if (verbose) {
            printf("%c %" PRIx64 ",%u ", operation, address, size);
        }
        access_cache(cache, s, E, b, address, &hit_count, &miss_count,
                     &eviction_count, &access_count, verbose);
        if (operation == 'M') {
            access_cache(cache, s, E, b, address, &hit_count, &miss_count,
                         &eviction_count, &access_count, verbose);
        }
        if (verbose) {
            printf("\n");
        }
    }

    if (ferror(trace)) {
        fprintf(stderr, "Error reading trace file: %s\n", trace_file);
        fclose(trace);
        free(cache);
        return 1;
    }
    if (fclose(trace) != 0) {
        perror("Could not close trace file");
        free(cache);
        return 1;
    }

    free(cache);

    // output cache hit and miss statistics
    print_summary(hit_count, miss_count, eviction_count);
    
    // assignment done. life is good!
    return 0;
}
