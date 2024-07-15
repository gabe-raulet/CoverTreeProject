#include <iostream>
#include <random>
#include <iomanip>
#include "graphutils.h"
#include "timer.h"

using namespace std;

int main(int argc, char *argv[])
{
    LocalTimer timer;
    timer.start_timer();

    using Index = int;
    using Real = double;

    using GraphUtils = GraphUtils<Index>;
    using GraphV = GraphUtils::GraphV;
    using GraphS = GraphUtils::GraphS;

    fprintf(stderr, "\nGenerating erdos renyi graph type1 [n=1000,p=0.25]\n\n");

    GraphV type1;
    {
        random_device rd;
        default_random_engine gen(rd());
        GraphUtils::erdos_renyi(type1, 1000, 0.25, gen, true);
    }

    GraphUtils::shuffle_vector_graph(type1);
    GraphUtils::write_graph_file(type1, "type1.A.txt", true);

    GraphUtils::shuffle_vector_graph(type1);
    GraphUtils::write_graph_file(type1, "type1.B.txt", true);

    GraphUtils::shuffle_vector_graph(type1);
    GraphUtils::write_graph_file(type1, "type1.C.txt", true);

    fprintf(stderr, "\n");

    GraphV A1_v, B1_v, C1_v;
    GraphS A1_s, B1_s, C1_s;

    GraphUtils::read_graph_file(A1_v, "type1.A.txt", true);
    GraphUtils::read_graph_file(A1_s, "type1.A.txt", true);

    GraphUtils::read_graph_file(B1_v, "type1.B.txt", true);
    GraphUtils::read_graph_file(B1_s, "type1.B.txt", true);

    GraphUtils::read_graph_file(C1_v, "type1.C.txt", true);
    GraphUtils::read_graph_file(C1_s, "type1.C.txt", true);

    fprintf(stderr, "\nA1_vector vs B1_vector; A1_vector vs C1_vector; B1_vector vs C1_vector;\n\n");

    assert((GraphUtils::compare_graphs(A1_v, B1_v, true)));
    assert((GraphUtils::compare_graphs(A1_v, C1_v, true)));
    assert((GraphUtils::compare_graphs(B1_v, C1_v, true)));

    fprintf(stderr, "\nA1_set vs B1_set; A1_set vs C1_set; B1_set vs C1_set;\n\n");

    assert((GraphUtils::compare_graphs(A1_s, B1_s, true)));
    assert((GraphUtils::compare_graphs(A1_s, C1_s, true)));
    assert((GraphUtils::compare_graphs(B1_s, C1_s, true)));

    fprintf(stderr, "\nA1_set vs B1_vector; A1_set vs C1_vector; B1_set vs C1_vector;\n\n");

    assert((GraphUtils::compare_graphs(A1_s, B1_v, true)));
    assert((GraphUtils::compare_graphs(A1_s, C1_v, true)));
    assert((GraphUtils::compare_graphs(B1_s, C1_v, true)));

    fprintf(stderr, "\nA1_vector vs B1_set; A1_vector vs C1_set; B1_vector vs C1_set;\n\n");

    assert((GraphUtils::compare_graphs(A1_v, B1_s, true)));
    assert((GraphUtils::compare_graphs(A1_v, C1_s, true)));
    assert((GraphUtils::compare_graphs(B1_v, C1_s, true)));

    fprintf(stderr, "\nGenerating erdos renyi graph type2 [n=12500,p=0.023]\n\n");

    GraphV type2;
    {
        random_device rd;
        default_random_engine gen(rd());
        GraphUtils::erdos_renyi(type2, 12500, 0.023, gen, true);
    }

    GraphUtils::shuffle_vector_graph(type2);
    GraphUtils::write_graph_file(type2, "type2.A.txt", true);

    GraphUtils::shuffle_vector_graph(type2);
    GraphUtils::write_graph_file(type2, "type2.B.txt", true);

    GraphUtils::shuffle_vector_graph(type2);
    GraphUtils::write_graph_file(type2, "type2.C.txt", true);

    fprintf(stderr, "\n");

    GraphV A2_v, B2_v, C2_v;
    GraphS A2_s, B2_s, C2_s;

    GraphUtils::read_graph_file(A2_v, "type2.A.txt", true);
    GraphUtils::read_graph_file(A2_s, "type2.A.txt", true);

    GraphUtils::read_graph_file(B2_v, "type2.B.txt", true);
    GraphUtils::read_graph_file(B2_s, "type2.B.txt", true);

    GraphUtils::read_graph_file(C2_v, "type2.C.txt", true);
    GraphUtils::read_graph_file(C2_s, "type2.C.txt", true);

    fprintf(stderr, "\nA2_vector vs B2_vector; A2_vector vs C2_vector; B2_vector vs C2_vector;\n\n");

    assert((GraphUtils::compare_graphs(A2_v, B2_v, true)));
    assert((GraphUtils::compare_graphs(A2_v, C2_v, true)));
    assert((GraphUtils::compare_graphs(B2_v, C2_v, true)));

    fprintf(stderr, "\nA2_set vs B2_set; A2_set vs C2_set; B2_set vs C2_set;\n\n");

    assert((GraphUtils::compare_graphs(A2_s, B2_s, true)));
    assert((GraphUtils::compare_graphs(A2_s, C2_s, true)));
    assert((GraphUtils::compare_graphs(B2_s, C2_s, true)));

    fprintf(stderr, "\nA2_set vs B2_vector; A2_set vs C2_vector; B2_set vs C2_vector;\n\n");

    assert((GraphUtils::compare_graphs(A2_s, B2_v, true)));
    assert((GraphUtils::compare_graphs(A2_s, C2_v, true)));
    assert((GraphUtils::compare_graphs(B2_s, C2_v, true)));

    fprintf(stderr, "\nA2_vector vs B2_set; A2_vector vs C2_set; B2_vector vs C2_set;\n\n");

    assert((GraphUtils::compare_graphs(A2_v, B2_s, true)));
    assert((GraphUtils::compare_graphs(A2_v, C2_s, true)));
    assert((GraphUtils::compare_graphs(B2_v, C2_s, true)));

    fprintf(stderr, "\nGenerating erdos renyi graph type3 [n=20000,p=0.00001]\n\n");

    GraphV type3;
    {
        random_device rd;
        default_random_engine gen(rd());
        GraphUtils::erdos_renyi(type3, 20000, 0.00001, gen, true);
    }

    GraphUtils::shuffle_vector_graph(type3);
    GraphUtils::write_graph_file(type3, "type3.A.txt", true);

    GraphUtils::shuffle_vector_graph(type3);
    GraphUtils::write_graph_file(type3, "type3.B.txt", true);

    GraphUtils::shuffle_vector_graph(type3);
    GraphUtils::write_graph_file(type3, "type3.C.txt", true);

    fprintf(stderr, "\n");

    GraphV A3_v, B3_v, C3_v;
    GraphS A3_s, B3_s, C3_s;

    GraphUtils::read_graph_file(A3_v, "type3.A.txt", true);
    GraphUtils::read_graph_file(A3_s, "type3.A.txt", true);

    GraphUtils::read_graph_file(B3_v, "type3.B.txt", true);
    GraphUtils::read_graph_file(B3_s, "type3.B.txt", true);

    GraphUtils::read_graph_file(C3_v, "type3.C.txt", true);
    GraphUtils::read_graph_file(C3_s, "type3.C.txt", true);

    fprintf(stderr, "\nA3_vector vs B3_vector; A3_vector vs C3_vector; B3_vector vs C3_vector;\n\n");

    assert((GraphUtils::compare_graphs(A3_v, B3_v, true)));
    assert((GraphUtils::compare_graphs(A3_v, C3_v, true)));
    assert((GraphUtils::compare_graphs(B3_v, C3_v, true)));

    fprintf(stderr, "\nA3_set vs B3_set; A3_set vs C3_set; B3_set vs C3_set;\n\n");

    assert((GraphUtils::compare_graphs(A3_s, B3_s, true)));
    assert((GraphUtils::compare_graphs(A3_s, C3_s, true)));
    assert((GraphUtils::compare_graphs(B3_s, C3_s, true)));

    fprintf(stderr, "\nA3_set vs B3_vector; A3_set vs C3_vector; B3_set vs C3_vector;\n\n");

    assert((GraphUtils::compare_graphs(A3_s, B3_v, true)));
    assert((GraphUtils::compare_graphs(A3_s, C3_v, true)));
    assert((GraphUtils::compare_graphs(B3_s, C3_v, true)));

    fprintf(stderr, "\nA3_vector vs B3_set; A3_vector vs C3_set; B3_vector vs C3_set;\n\n");

    assert((GraphUtils::compare_graphs(A3_v, B3_s, true)));
    assert((GraphUtils::compare_graphs(A3_v, C3_s, true)));
    assert((GraphUtils::compare_graphs(B3_v, C3_s, true)));

    timer.stop_timer();

    fprintf(stderr, "\n[time=%.3f,msg::%s] command:", timer.get_elapsed(), __func__);
    for (int i = 0; i < argc; ++i) fprintf(stderr, " %s", argv[i]);
    fprintf(stderr, "\n");

    return 0;
}
