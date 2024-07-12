#include <iostream>
#include <random>
#include <unistd.h>
#include "graphutils.h"
#include "timer.h"

using namespace std;

template <class Index, class Real>
void read_options(int argc, char *argv[], char *&fname, Index& n, Real& p, int& seed, bool& shuffle);

template <class Graph, class Index, class Real>
void build_erdos_renyi(Graph& g, Index n, Real p, int seed);

int main(int argc, char *argv[])
{
    using Index = int64_t;
    using Real = double;

    char *fname;
    Index n = 1000;
    Real p = 0.25;
    bool shuffle = false;
    int seed = -1;
    GraphUtils<Index>::VecGraph g;

    read_options(argc, argv, fname, n, p, seed, shuffle);
    build_erdos_renyi(g, n, p, seed);

    if (shuffle) GraphUtils<Index>::shuffle_vector_graph(g);

    GraphUtils<Index>::write_graph_file(g, fname);
    return 0;
}

template <class Index, class Real>
void read_options(int argc, char *argv[], char *&fname, Index& n, Real& p, int& seed, bool& shuffle)
{
    LocalTimer timer;
    timer.start_timer();

    auto usage = [&argv, &n, &p, &seed] (int err)
    {
        fprintf(stderr, "Usage: %s [options] <graphfname>\n", argv[0]);
        fprintf(stderr, "Options: -n INT   number of vertices [%lu]\n", (size_t)n);
        fprintf(stderr, "         -p FLOAT probability [%.2f]\n", p);
        fprintf(stderr, "         -s INT   rng seed [%s]\n", seed<0? "random" : to_string(seed).c_str());
        fprintf(stderr, "         -R       randomly shuffle graph\n");
        fprintf(stderr, "         -h       help message\n");
        exit(err);
    };

    int c;
    while ((c = getopt(argc, argv, "n:p:s:Rh")) >= 0)
    {
        if      (c == 'n') n = atoi(optarg);
        else if (c == 'p') p = atof(optarg);
        else if (c == 's') seed = atoi(optarg);
        else if (c == 'R') shuffle = true;
        else if (c == 'h') usage(0);
    }

    if (argc - optind < 1)
    {
        timer.stop_timer();
        fprintf(stderr, "[time=%.3f,err::%s] :: missing argument(s)\n", timer.get_elapsed(), __func__);
        usage(1);
    }

    fname = argv[optind];

    random_device rd;
    if (seed < 0) seed = rd();

    timer.stop_timer();
    fprintf(stderr, "[time=%.3f,msg::%s] :: read options [fname='%s',n=%lu,p=%.2f,seed=%d,shuffle=%s]\n", timer.get_elapsed(), __func__, fname, (size_t)n, p, seed, shuffle? "true" : "false");
}

template <class Graph, class Index, class Real>
void build_erdos_renyi(Graph& g, Index n, Real p, int seed)
{
    Index m;
    Real avg_deg;

    LocalTimer timer;
    timer.start_timer();

    default_random_engine gen(seed);
    m = GraphUtils<Index>::erdos_renyi(g, n, p, gen);

    timer.stop_timer();

    avg_deg = (m+0.0)/(n+0.0);
    fprintf(stderr, "[time=%.3f,msg::%s] :: [n=%lu,m=%lu,m/n=%.3f]\n", timer.get_elapsed(), __func__, (size_t)n, (size_t)m, avg_deg);
}
