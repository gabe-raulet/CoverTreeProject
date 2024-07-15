#include <iostream>
#include "graphutils.h"

using namespace std;

int main(int argc, char *argv[])
{
    using Real = double;
    using GraphUtils = GraphUtils<>;
    using Index = GraphUtils::Index;
    using GraphV = GraphUtils::GraphV;
    using GraphS = GraphUtils::GraphS;

    random_device rd;
    default_random_engine gen(rd());

    GraphV v1, v2;
    GraphS s1, s2;

    GraphUtils::erdos_renyi(v1, 1000, 0.1, gen, true);
    GraphUtils::write_graph_file(v1, "v1", true);
    GraphUtils::read_graph_file(s1, "v1", true);
    GraphUtils::read_graph_file(v2, "v1", true);

    assert((GraphUtils::compare_graphs(v1, s1)));
    assert((GraphUtils::compare_graphs(v1, v2)));

    GraphUtils::shuffle_vector_graph(v2);
    assert((GraphUtils::compare_graphs(v1, v2)));

    GraphUtils::write_graph_file(v2, "v2", true);
    GraphUtils::read_graph_file(s2, "v2", true);

    assert((GraphUtils::compare_graphs(s1, s2)));

    v2.back().pop_back();
    assert(!(GraphUtils::compare_graphs(v1, v2)));
}
