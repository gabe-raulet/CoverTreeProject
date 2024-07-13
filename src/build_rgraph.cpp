#include <iostream>
#include <random>
#include <vector>
#include <unistd.h>
#include "ptidx.h"
#include "ptraits.h"
#include "ptutils.h"
#include "graphutils.h"
#include "timer.h"
#include "misc.h"

using Index = int64_t;
using PointTraits = SelectPoint<FPSIZE, PTDIM>::PointTraits;

template <class Kind>
using PointIndex = PointIndexer<PointTraits, Index, Kind>;

using BruteForce = BruteForcer<PointTraits, Index>;
using TreeIndex = CoverTreeIndex<PointTraits, Index>;

using Real = PointTraits::Real;
using Point = PointTraits::Point;
using Graph = vector<unordered_set<Index>>;

enum Indexer {BruteOpt, TreeOpt};

void read_options(int argc, char *argv[], char *&infname, char *&outfname, double& radius, double& base, Indexer& indexer);
void write_graph_file(const Graph& graph, const char *fname);

template <class Kind>
void build_point_index(PointIndex<Kind>& ptidx, const vector<Point>& points);

template <class Kind>
void build_rgraph(PointIndex<Kind>& ptidx, double radius, Graph& graph);

int main(int argc, char *argv[])
{
    double radius, base = 2.0;
    char *infname, *outfname;
    Indexer indexer = TreeOpt;

    LocalTimer timer;
    timer.start_timer();

    vector<Point> points;

    read_options(argc, argv, infname, outfname, radius, base, indexer);

    PointUtils<PointTraits, Index>::read_points_file(points, infname);

    BruteForce bf;
    TreeIndex ti(base);

    if (indexer == TreeOpt) build_point_index(ti, points);
    else build_point_index(bf, points);

    Graph graph;

    if (indexer == TreeOpt) build_rgraph(ti, radius, graph);
    else build_rgraph(bf, radius, graph);

    write_graph_file(graph, outfname);

    timer.stop_timer();
    main_msg(argc, argv, timer.get_elapsed());

    return 0;
}

void read_options(int argc, char *argv[], char *&infname, char *&outfname, double& radius, double& base, Indexer& indexer)
{
    LocalTimer timer;
    timer.start_timer();

    auto usage = [&argv, &indexer, &base](int err)
    {
        fprintf(stderr, "Usage: %s [options] <ptsfname> <graphfname> <radius>\n", argv[0]);
        fprintf(stderr, "Options: -T  use cover tree indexer [default]\n");
        fprintf(stderr, "         -F  use brute force indexer\n");
        fprintf(stderr, "         -b  cover tree base [%.2f]\n", base);
        fprintf(stderr, "         -h  help message\n");
        exit(err);
    };

    int c;
    while ((c = getopt(argc, argv, "TFb:h")) >= 0)
    {
        if      (c == 'T') indexer = TreeOpt;
        else if (c == 'F') indexer = BruteOpt;
        else if (c == 'b') base = atof(optarg);
        else if (c == 'h') usage(0);
    }

    if (argc - optind < 3)
    {
        fprintf(stderr, "[err::%s] missing argument(s)\n", __func__);
        usage(1);
    }

    infname = argv[optind++];
    outfname = argv[optind++];
    radius = atof(argv[optind]);

    timer.stop_timer();

    fprintf(stderr, "[time=%.3f,msg::%s] :: [ptsfname='%s',graphfname='%s',radius=%.2f,indexer=%s]\n", timer.get_elapsed(), __func__, infname, outfname, radius, indexer == BruteOpt? "brute_force" : "cover_tree");
}

template <class Kind>
void build_point_index(PointIndex<Kind>& ptidx, const vector<Point>& points)
{
    LocalTimer timer;
    timer.start_timer();

    ptidx.build(points.begin(), points.end());

    timer.stop_timer();

    fprintf(stderr, "[time=%.3f,msg::%s] :: built index using %s indexer\n", timer.get_elapsed(), __func__, ptidx.repr());
}

template <class Kind>
void build_rgraph(PointIndex<Kind>& ptidx, double radius, Graph& graph)
{
    LocalTimer timer;
    timer.start_timer();

    Index num_edges = ptidx.build_rgraph(static_cast<Real>(radius), graph);

    timer.stop_timer();

    fprintf(stderr, "[time=%.3f,msg::%s] :: built %.3f-graph [num_verts=%lu,num_edges=%lu,avg_deg=%.3f]\n", timer.get_elapsed(), __func__, radius, graph.size(), static_cast<size_t>(num_edges), (num_edges+0.0)/graph.size());
}

void write_graph_file(const Graph& graph, const char *fname)
{
    GraphUtils<Index>::write_graph_file(graph, fname, true);
}
