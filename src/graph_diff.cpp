#include <vector>
#include <stdio.h>
#include "graphutils.h"

using namespace std;

using Index = int64_t;
using IndexVector = vector<Index>;
using Graph = vector<IndexVector>;

int main(int argc, char *argv[])
{
    LocalTimer timer;
    timer.start_timer();

    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <graph1> <graph2>\n", argv[0]);
        return 1;
    }

    Graph graph1, graph2;
    Index m1, m2;

    m1 = GraphUtils<Index>::read_graph_file(graph1, argv[1], true);
    m2 = GraphUtils<Index>::read_graph_file(graph2, argv[2], true);

    GraphUtils<Index>::compare_graphs(graph1, m1, graph2, m2, true);

    timer.stop_timer();

    main_msg(argc, argv, timer.get_elapsed());
    return 0;
}
