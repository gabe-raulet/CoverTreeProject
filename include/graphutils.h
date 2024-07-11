#ifndef GRAPH_UTILS_H_
#define GRAPH_UTILS_H_

#include <vector>
#include <unordered_set>
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
    using IndexSet = unordered_set<Index>;

    template <class Container>
    struct NeighborComparator;

    template <>
    struct NeighborComparator<IndexVector>
    {
        bool operator()(const IndexVector& a, const IndexVector& b) { return is_permutation(a.cbegin(), a.cend(), b.cbegin()); }
    };

    template <>
    struct NeighborComparator<IndexSet>
    {
        bool operator()(const IndexSet& a, const IndexSet& b) { return (a == b); }
    };

    static Index read_graph_file(vector<IndexVector>& graph, const char *fname, bool verbose = false);

    template <class NeighborContainer>
    static void write_graph_file(const vector<NeighborContainer>& graph, const char *fname, bool verbose = false);

    template <class NeighborContainer>
    static bool compare_graphs(const vector<NeighborContainer>& g1, Index m1, const vector<NeighborContainer>& g2, Index m2, bool verbose = false);


};

#include "graphutils.hpp"

#endif
