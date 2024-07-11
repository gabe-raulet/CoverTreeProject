#ifndef GRAPH_UTILS_H_
#define GRAPH_UTILS_H_

#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include "misc.h"
#include "timer.h"

using namespace std;

template <class Index>
struct GraphUtils
{
    using IndexVector = vector<Index>;
    using Graph = vector<IndexVector>;

    static Index read_graph_file(Graph& graph, const char *fname, bool verbose = false);
    static bool compare_graphs(const Graph& g1, Index m1, const Graph& g2, Index m2, bool verbose = false);
};

#include "graphutils.hpp"

#endif
