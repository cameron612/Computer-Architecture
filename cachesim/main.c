#include <stdio.h>
#include <stdlib.h>

#include "cachesim.h"

FILE *open_trace(const char *filename) 
{
  return fopen(filename, "r");
}

addr_t prev_addr;
int next_line(FILE* trace)
{
  if (feof(trace) || ferror(trace)) return 0;
  else {
    char t;
    unsigned long long va, pa;
    unsigned sz;
    volatile int scan_code = fscanf(trace, "%c %llx %llx %u\n", &t, &va, &pa, &sz); //Scan the trace line
    prev_addr = pa;
    cachesim_access(pa, (t == 'w'));
  }
  return 1;
}

int main(int argc, char **argv) 
{
  FILE *input;

  if (argc != 5) {
    fprintf(stderr, "Usage:\n  %s <trace> <block size(bytes)>"
                    " <cache size(bytes)> <ways>\n", argv[0]);
    return 1;
  }

  input = open_trace(argv[1]);
  if (!input)
  {
    fprintf(stderr, "Trace file %s not found!\n", argv[1]);
    return 1;
  }

  cachesim_init(atol(argv[2]), atol(argv[3]), atol(argv[4]));
  while (next_line(input));
  cachesim_print_stats(); //Print the stats of the cache
  cachesim_destruct(); //Cleanup

  return 0;
}
