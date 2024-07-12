#include <iostream>
#include <random>
#include <iomanip>
#include "graphutils.h"
#include "timer.h"

using namespace std;

int main(int argc, char *argv[])
{
    int seed = -1;
    if (argc == 2) seed = atoi(argv[1]);

    using Index = int;
    using Real = double;

    using GraphUtils = GraphUtils<Index>;

    GraphUtils::VecGraph g1;
    GraphUtils::SetGraph g2;

    GraphUtils::erdos_renyi(g1, 2000, 0.05, 5);
    GraphUtils::erdos_renyi(g2, 2000, 0.05, 5);

    GraphUtils::write_graph_file(g1, "A.txt");
    GraphUtils::write_graph_file(g2, "B.txt");

    GraphUtils::VecGraph h1;
    GraphUtils::SetGraph h2;

    GraphUtils::read_graph_file(h1, "A.txt");
    GraphUtils::read_graph_file(h2, "B.txt");

    GraphUtils::write_graph_file(h1, "X.txt");
    GraphUtils::write_graph_file(h2, "Y.txt");

    return 0;
}
