#include <iostream>
#include <random>
#include <iomanip>
#include "graphutils.h"
#include "timer.h"

using namespace std;

int main(int argc, char *argv[])
{
    using Index = int;
    using Real = double;

    using GraphUtils = GraphUtils<Index>;
    using Graph = GraphUtils::VecGraph;

    Graph A, B;

    GraphUtils::erdos_renyi(A, 200, 0.05, 3141592);
    GraphUtils::erdos_renyi(B, 200, 0.05, 3141592);

    GraphUtils::shuffle_vector_graph(A);
    GraphUtils::shuffle_vector_graph(B);

    GraphUtils::write_graph_file(A, "A.txt");
    GraphUtils::write_graph_file(B, "B.txt");

    Graph AA, BB;

    GraphUtils::read_graph_file(AA, "A.txt");
    GraphUtils::read_graph_file(BB, "B.txt");

    GraphUtils::shuffle_vector_graph(AA);
    GraphUtils::shuffle_vector_graph(BB);

    assert((GraphUtils::compare_graphs(AA, BB)));

    return 0;
}
