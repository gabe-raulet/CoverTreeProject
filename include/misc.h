#ifndef MISC_H_
#define MISC_H_

#include <fstream>
#include <sstream>
#include <stdio.h>

using namespace std;

void main_msg(int argc, char *argv[], double elapsed)
{
    fprintf(stderr, "[time=%.3f,msg::main] command:", elapsed);
    for (int i = 0; i < argc; ++i) fprintf(stderr, " %s", argv[i]);
    fprintf(stderr, "\n");
}

void main_msg(int argc, char *argv[], double maxtime, double sumtime, int nprocs)
{
    fprintf(stderr, "[maxtime=%.3f,avgtime=%.3f,nprocs=%d,msg::main] command:", maxtime, sumtime/nprocs, nprocs);
    for (int i = 0; i < argc; ++i) fprintf(stderr, " %s", argv[i]);
    fprintf(stderr, "\n");
}

template <integral T>
void get_balanced_counts(vector<T>& counts, size_t totsize)
{
    T blocks = counts.size();
    fill(counts.begin(), counts.end(), totsize/blocks);

    counts.back() = totsize - (blocks-1)*(totsize/blocks);

    T diff = counts.back() - counts.front();

    for (T i = 0; i < diff; ++i)
    {
        counts[blocks-1-i]++;
        counts[blocks-1]--;
    }
}

template <integral T>
T read_integer(char *str)
{
    double x;
    char *p;
    char *buf;

    buf = strdup(str);
    x = strtod(buf, &p);

    if      (toupper(*p) == 'K') x *= (1LL << 10);
    else if (toupper(*p) == 'M') x *= (1LL << 20);
    else if (toupper(*p) == 'G') x *= (1LL << 30);

    T result =  static_cast<T>(x + 0.499);
    free(buf);

    return result;
}

template <class T, integral Index>
Index iselect(const vector<T>& input, const vector<Index>& ids, vector<T>& output)
{
    output.resize(ids.size());
    transform(ids.begin(), ids.end(), output.begin(), [&input](Index id) { return input[id]; });
    return output.size();
}

#endif
