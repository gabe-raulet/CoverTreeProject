#ifndef GRAPH_UTILS_H_
#define GRAPH_UTILS_H_

#include <vector>
#include <random>
#include <unordered_set>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "timer.h"
#include "fileinfo.h"

using namespace std;

template <class Index>
struct GraphUtils
{
    using IndexVec = vector<Index>;
    using IndexSet = unordered_set<Index>;

    using VecGraph = vector<IndexVec>;
    using SetGraph = vector<IndexSet>;

    template <class Graph>
    static Index num_edges(const Graph& g);

    template <class Graph, class Real>
    static void get_info(const Graph& g, Index& verts, Index& edges, Real& avg_deg);

    template <class Graph>
    static Index read_graph_file(Graph& g, const char *fname, bool verbose = true);

    template <class Graph>
    static void write_graph_file(const Graph& g, const char *fname, bool verbose = true);

    template <class Graph1, class Graph2>
    static bool compare_graphs(const Graph1& g1, const Graph2& g2, bool verbose = true);

    template <class Graph, class Real>
    static Index erdos_renyi(Graph& g, Index n, Real p, int seed = -1);

    template <class Graph, class Real, class RandomGen>
    static Index erdos_renyi(Graph& g, Index n, Real p, RandomGen& gen);

    static void shuffle_vector_graph(VecGraph& g, int seed = -1, bool verbose = true);

    template <class RandomGen>
    static void shuffle_vector_graph(VecGraph& g, RandomGen& gen, bool verbose = true);
};

#include "graphutils.hpp"

#endif
