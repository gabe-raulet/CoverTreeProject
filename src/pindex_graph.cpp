#include <iostream>
#include <random>
#include <vector>
#include <unistd.h>
#include "pindex.h"
#include "ptraits.h"
#include "timer.h"
#include "misc.h"

using namespace std;

using Index = int64_t;
using PointTraits = SelectPoint<FPSIZE, PTDIM>::PointTraits;
using PIndex = PointIndex<PointTraits, Index>;
using Real = PointTraits::Real;
using Point = PointTraits::Point;
using PointVector = vector<Point>;
using IndexSetVector = vector<unordered_set<Index>>;

void read_options(int argc, char *argv[], char *&fname, double& cutoff, int& iters, double& damping, char *&oprefix);
void read_points_file(PointVector& points, const char *fname);
void build_rgraph(PIndex& ptidx, double radius, IndexSetVector& rgraph, int iter);
void write_rgraph_file(const IndexSetVector& rgraph, const char *oprefix, int iter);

int main(int argc, char *argv[])
{
    LocalTimer timer;
    timer.start_timer();

    char *fname, *oprefix = NULL;
    double cutoff;
    int iters = 5;
    double damping = 0.5;
    PointVector points;

    read_options(argc, argv, fname, cutoff, iters, damping, oprefix);
    read_points_file(points, fname);

    timer.stop_timer();
    main_msg(argc, argv, timer.get_elapsed());
    return 0;
}

void read_options(int argc, char *argv[], char *&fname, double& cutoff, int& iters, double& damping, char *&oprefix)
{
    LocalTimer timer;
    timer.start_timer();

    auto usage = [&argv, &iters, &damping, &oprefix](int err)
    {
        fprintf(stderr, "Usage: %s [options] <ptsfname> <cutoff>\n", argv[0]);
        fprintf(stderr, "Options: -n INT   iterations [%d]\n", iters);
        fprintf(stderr, "         -d FLOAT damping [%.2f]\n", damping);
        fprintf(stderr, "         -o STR   output prefix [%s]\n", oprefix? oprefix : "optional");
        fprintf(stderr, "         -h       help message\n");
        exit(err);
    };

    int c;
    while ((c = getopt(argc, argv, "n:d:o:h")) >= 0)
    {
        if      (c == 'n') iters = atoi(optarg);
        else if (c == 'd') damping = atof(optarg);
        else if (c == 'o') oprefix = optarg;
        else if (c == 'h') usage(0);
    }

    if (argc - optind < 2)
    {
        fprintf(stderr, "[err::%s] missing argument(s)\n", __func__);
        usage(1);
    }

    fname = argv[optind++];
    cutoff = atof(argv[optind]);

    timer.stop_timer();

    fprintf(stderr, "[time=%.3f,msg::%s] :: [fname='%s',cutoff=%.2f,iters=%d,damping=%.2f,oprefix='%s']\n", timer.get_elapsed(), __func__, fname, cutoff, iters, damping, oprefix);
}

void read_points_file(PointVector& points, const char *fname)
{
    LocalTimer timer;
    timer.start_timer();

    points.clear();
    PointTraits::read_from_file(back_inserter(points), fname);

    timer.stop_timer();

    fprintf(stderr, "[time=%.3f,msg::%s] read points file '%s' [numpts=%lu,filesize=%s]\n", timer.get_elapsed(), __func__, fname, static_cast<size_t>(points.size()), PrettyFileSize::str(fname).c_str());
}
