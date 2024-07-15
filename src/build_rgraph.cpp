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

using namespace std;

enum Indexer {BruteOpt, TreeOpt};

template <class Real>
void read_options(int argc, char *argv[], char *&infname, char *&outfname, Real& radius, Real& base, Indexer& indexer);

int main(int argc, char *argv[])
{
    using PointTraits = SelectPoint<FPSIZE, PTDIM>::PointTraits;
    using PointUtils = PointUtils<PointTraits>;

    using Index = PointUtils::Index;
    using Real = PointTraits::Real;
    using Point = PointTraits::Point;
    using PointVector = PointTraits::PointVector;
    using Graph = GraphUtils<Index>::GraphV;

    using BruteForce = BruteForcer<PointTraits, Index>;

    Real radius, base = 2.0;
    char *infname, *outfname;
    Indexer indexer = TreeOpt;

    LocalTimer timer;
    timer.start_timer();

    PointVector points;

    read_options(argc, argv, infname, outfname, radius, base, indexer);

    PointUtils::read_points_file(points, infname);

    BruteForce bf;

    if (indexer == TreeOpt) PointUtils::build_point_index(bf, points, true);
    else PointUtils::build_point_index(bf, points, true);

    Graph graph;

    PointUtils::build_rgraph(bf, radius, graph, true);

    GraphUtils<Index>::write_graph_file(graph, outfname, true);

    timer.stop_timer();
    main_msg(argc, argv, timer.get_elapsed());

    return 0;
}

template <class Real>
void read_options(int argc, char *argv[], char *&infname, char *&outfname, Real& radius, Real& base, Indexer& indexer)
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
