#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "cachelab.h"

void print_help_msg()
{
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("  -h\t\tPrint this help message.\n");
    printf("  -v\t\tOptional verbose flag.\n");
    printf("  -s <num>\tNumber of set index bits.\n");
    printf("  -E <num>\tNumber of lines per set.\n");
    printf("  -b <num>\tNumber of block offset bits.\n");
    printf("  -t <file>\tTrace file.\n");
}

typedef struct
{
    int valid;
    unsigned long tag;
    int lru_count;
} cache_line_t;

typedef struct
{
    cache_line_t* lines;
} cache_set_t;

typedef struct
{
    cache_set_t* sets;
    int s, E, b;
} cache_t;

cache_t* init_cache(int s, int E, int b)
{
    int S = 1 << s;
    cache_t* cache = (cache_t*)malloc(sizeof(cache_t));
    cache->sets = (cache_set_t*)malloc(S * sizeof(cache_set_t));
    
    for (int i = 0; i < S; i++)
    {
        cache->sets[i].lines = (cache_line_t*)malloc(E *sizeof(cache_line_t));
    }

    cache->s = s;
    cache->E = E;
    cache->b = b;
    return cache;
}

void free_cache(cache_t* cache)
{
    int S = 1 << cache->s;
    for (int i = 0; i < S; i++)
    {
        free(cache->sets[i].lines);
    }
    free(cache->sets);
}

void access_cache(cache_t* cache, unsigned long address, int* hit, int* miss, int* eviction, int verbose)
{
    unsigned long tag = address >> (cache->s + cache->b);
    int set_idx = (address >> cache->b) & ((1 << cache->s) - 1);

    cache_set_t* set = &cache->sets[set_idx];
    cache_line_t* line = set->lines;

    for (int i = 0; i < cache->E; i++)
    {
        if (line[i].valid && line[i].tag == tag)
        {
            if (verbose) printf(" hit");
            (*hit)++;
            // update other line's lru_count
            for (int j = 0; j < cache->E; j++) if (line[j].valid) line[j].lru_count++;
            // this line
            line[i].lru_count = 0;
            return;
        }
    }

    if (verbose) printf(" miss");
    (*miss)++;

    int max_lru = -1;
    int evict_idx = -1;
    for (int i = 0; i < cache->E; i++)
    {
        if (!line[i].valid)
        {
            evict_idx = i;
            break;
        }

        if (line[i].lru_count > max_lru)
        {
            max_lru = line[i].lru_count;
            evict_idx = i;
        }
    }

    if (line[evict_idx].valid)
    {
        if (verbose) printf(" eviction");
        (*eviction)++;
    }

    line[evict_idx].valid = 1;
    line[evict_idx].tag = tag;
    // update other line's lru_count
    for (int j = 0; j < cache->E; j++) if (line[j].valid) line[j].lru_count++;
    line[evict_idx].lru_count = 0;
}

void process_trace_file(cache_t* cache, char* trace_file, int* hit, int* miss, int* eviction, int verbose)
{
    FILE* file = fopen(trace_file, "r");
    if (!file)
    {
        perror("Error opening file\n.");
        exit(1);
    }

    char op;
    unsigned long addr;
    int size;
    while (fscanf(file, " %c %lx,%d", &op, &addr, &size) == 3)
    {
        // I L S M
        if (op == 'I') continue;
        if (verbose) printf("%c %lx,%d", op, addr, size);
        if (op == 'M') access_cache(cache, addr, hit, miss, eviction, verbose);  // M = L and S
        access_cache(cache, addr, hit, miss, eviction, verbose);
        if (verbose) printf("\n");
    }
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        print_help_msg(argv[0]);
        return 0;
    }

    int verbose = 0;
    int s = -1, E = -1, b = -1;
    char* trace_file = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            print_help_msg();
            exit(0);
        case 'v':
            verbose = 1;
            break;
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            trace_file = optarg;
            break;
        default:
            print_help_msg();
            exit(1);
        }
    }

    if (s == -1 || E == -1 || b == -1 || trace_file == NULL)
    {
        printf("%s: Missing required command line argument.\n", argv[0]);
        print_help_msg();
        exit(1);
    }

    int hit = 0, miss = 0, eviction = 0;
    cache_t* cache = init_cache(s, E, b);
    process_trace_file(cache, trace_file, &hit, &miss, &eviction, verbose);
    free_cache(cache);
    printSummary(hit, miss, eviction);
    return 0;
}
