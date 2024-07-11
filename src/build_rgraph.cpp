#include <iostream>
#include <random>
#include <vector>
#include <unistd.h>
#include "ptidx.h"
#include "ptraits.h"
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
using PointVector = vector<Point>;
using IndexSetVector = vector<unordered_set<Index>>;

enum Indexer {BruteOpt, TreeOpt};

void read_options(int argc, char *argv[], char *&infname, char *&outfname, double& radius, Indexer& indexer);

int main(int argc, char *argv[])
{
    double radius;
    char *infname, *outfname;
    Indexer indexer = TreeOpt;

    LocalTimer timer;
    timer.start_timer();

    read_options(argc, argv, infname, outfname, radius, indexer);

    timer.stop_timer();
    main_msg(argc, argv, timer.get_elapsed());

    return 0;
}

void read_options(int argc, char *argv[], char *&infname, char *&outfname, double& radius, Indexer& indexer)
{
    LocalTimer timer;
    timer.start_timer();

    auto usage = [&argv, &indexer](int err)
    {
        fprintf(stderr, "Usage: %s [options] <ptsfname> <graphfname> <radius>\n", argv[0]);
        fprintf(stderr, "Options: -T  use cover tree indexer [default]\n");
        fprintf(stderr, "         -B  use brute force indexer\n");
        fprintf(stderr, "         -h  help message\n");
        exit(err);
    };

    int c;
    while ((c = getopt(argc, argv, "TBh")) >= 0)
    {
        if      (c == 'T') indexer = TreeOpt;
        else if (c == 'B') indexer = BruteOpt;
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
