/*  Name: Rui shaopu
    ID: ics517030910217
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <math.h>
#include "cachelab.h"

static int E = 0;
static int S = 0;
static int B = 0;
static int hitCount = 0;
static int missCount = 0;
static int evictionCount = 0;
static char line[120];
static FILE *in;
static long long int *cache;

#define start(index) (index * 2 * E)
#define end(index) ((index + 1) * 2 * E)

int addressCache(char action, long long int address)
{
    int hit = 0;
    int miss = 0;
    int evict = 1;
    int last = 0;
    int miss_in = 0;
    int evict_in = 0;

    long long int index = (address / B) % S;
    long long int tag = address / (B * S);

    switch (action)
    {
    case 'L':
    case 'M':
    case 'S':
    {
        for (int j = start(index); j < end(index); j += 2)
        {
            if (cache[j] == tag && cache[j + 1] != 0)
            {
                for (int i = start(index) + 1; (cache[i] != 0 && i < end(index) + 1); i += 2)
                {
                    cache[i]++;
                }
                cache[j + 1] = 1;
                hit = 1;
                hitCount++;
                break;
            }
            else if (!miss && cache[j] == 0 && cache[j + 1] == 0)
            {
                miss = 1;
                miss_in = j;
            }
            else if (cache[j + 1] > last)
            {
                last = cache[j + 1];
                evict_in = j;
            }
        }

        if (!hit && miss)
        {
            cache[miss_in] = tag;
            for (int j = start(index) + 1; j < miss_in + 1; j += 2)
            {
                cache[j]++;
            }
            cache[miss_in + 1] = 1;
            evict = 0;
            missCount++;
        }
        else if (!hit && evict)
        {
            for (int k = start(index) + 1; k < end(index) + 1; k += 2)
            {
                cache[k]++;
            }
            cache[evict_in] = tag;
            cache[evict_in + 1] = 1;
            evictionCount++;
            missCount++;
        }

        if (action == 'M')
        {
            hitCount++;
        }
        break;
    }
    default:
    {
        return 0;
    }
    }
    return 0;
}

static void usage(int error_flag, char error)
{
    if (error_flag == 0)
    {
        printf("./csim: Missing required command line argument\n");
    }
    else if (error_flag == 1)
    {
        printf("./csim: invalid option -- '%c'", error);
    }
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -in>\n");
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -in>  Trace file.\n");
    printf("Examples:\n");
    printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}

/* address inputs, set flags and filename*/
int getInput(int argc, char *argv[])
{
    if (getopt(argc, argv, "s:E:b:t:") != 's')
    {
        usage(0, ' ');
        return -1;
    }
    S = (int)pow(2, atoi(optarg));
    if (getopt(argc, argv, "s:E:b:t:") != 'E')
    {
        usage(0, ' ');
        return -1;
    }
    E = atoi(optarg);
    if (getopt(argc, argv, "s:E:b:t:") != 'b')
    {
        usage(0, ' ');
        return -1;
    }
    B = (int)pow(2, atoi(optarg));
    if (getopt(argc, argv, "s:E:b:t:") != 't')
    {
        usage(0, ' ');
        return -1;
    }

    in = fopen(optarg, "r");
    if (!in)
    {
        printf("%s: No such file or directory", optarg);
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (getInput(argc, argv) == -1)
    {
        return 0;
    }

    cache = (long long int *)calloc(S, E * 2 * sizeof(long long int));

    while (fgets(line, 100, in))
    {
        char action = line[1];

        char str[10] = "";
        for (int i = 3; line[i] != ','; i++)
        {
            str[i - 3] = line[i];
        }
        long long int address = strtoll(str, NULL, 16);

        addressCache(action, address);
    }

    free(cache);
    fclose(in);
    printSummary(hitCount, missCount, evictionCount);
    return 0;
}