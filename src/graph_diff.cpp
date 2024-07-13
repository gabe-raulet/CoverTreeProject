#include <vector>
#include <stdio.h>
#include "graphutils.h"
#include "timer.h"

using namespace std;

int main(int argc, char *argv[])
{
    using Index = int64_t;
    using GraphUtils = GraphUtils<Index>;
    using Graph = GraphUtils::VecGraph;

    LocalTimer timer;
    timer.start_timer();

    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <graph1> <graph2>\n", argv[0]);
        return 1;
    }

    Graph g1, g2;

    GraphUtils::read_graph_file(g1, argv[1]);
    GraphUtils::read_graph_file(g2, argv[2]);

    GraphUtils::compare_graphs(g1, g2);

    timer.stop_timer();

    fprintf(stderr, "[time=%.3f,msg::%s] command:", timer.get_elapsed(), __func__);
    for (int i = 0; i < argc; ++i) fprintf(stderr, " %s", argv[i]);
    fprintf(stderr, "\n");

    return 0;
}
