#ifndef GRAPH_UTILS_H_
#define GRAPH_UTILS_H_

#include <iostream>
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

template <class Index_ = int64_t>
struct GraphUtils
{
    using Index = Index_;

    using IndexVector = vector<Index>;
    using IndexSet = unordered_set<Index>;

    using GraphV = vector<IndexVector>;
    using GraphS = vector<IndexSet>;

    template <class Graph>
    static Index num_edges(const Graph& g);

    template <class Graph>
    static double read_graph_file(Graph& g, const char *fname, bool verbose = false);

    template <class Graph>
    static double write_graph_file(const Graph& g, const char *fname, bool verbose = false);

    template <class Graph1, class Graph2>
    static bool compare_graphs(const Graph1& g1, const Graph2& g2, bool verbose = false);

    template <class Graph, class Real, class RandomGen>
    static double erdos_renyi(Graph& g, Index n, Real p, RandomGen& gen, bool verbose = false);

    static void shuffle_vector_graph(GraphV& g, int seed = -1);
};

#include "graphutils.hpp"

#endif
