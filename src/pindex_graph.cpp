#include <iostream>
#include <random>
#include <vector>
#include <unistd.h>
#include "ptidx.h"
#include "ptraits.h"
#include "timer.h"
#include "misc.h"

using namespace std;

using Index = int64_t;
using PointTraits = SelectPoint<FPSIZE, PTDIM>::PointTraits;

template <class Kind>
using PointIndex = PointIndexer<PointTraits, Index, Kind>;
using BruteForce = BruteForcer<PointTraits, Index>;
using PrunedForce = PrunedForcer<PointTraits, Index>;
using TreeIndex = CoverTreeIndex<PointTraits, Index>;

using Real = PointTraits::Real;
using Point = PointTraits::Point;
using PointVector = vector<Point>;
using IndexSetVector = vector<unordered_set<Index>>;

void read_options(int argc, char *argv[], char *&fname, double& cutoff, int& iters, double& damping, char *&oprefix);
void read_points_file(PointVector& points, const char *fname);
void write_rgraph_file(const IndexSetVector& rgraph, const char *oprefix, int iter);

template <class Kind>
void build_point_index(PointIndex<Kind>& ptidx, const PointVector& points);

template <class Kind>
void build_rgraph(PointIndex<Kind>& ptidx, double radius, IndexSetVector& rgraph, int iter);

int main(int argc, char *argv[])
{
    LocalTimer timer;
    timer.start_timer();

    char *fname, *oprefix = NULL;
    double cutoff;
    int iters = 5;
    double damping = 0.5;

    read_options(argc, argv, fname, cutoff, iters, damping, oprefix);

    BruteForce bf;
    PrunedForce pf(cutoff);
    TreeIndex tf(2.0);

    PointVector points;
    vector<IndexSetVector> graphs;

    read_points_file(points, fname);

    build_point_index(bf, points);
    /* build_point_index(pf, points); */
    /* build_point_index(tf, points); */


    double radius = cutoff;
    for (int iter = 1; iter <= iters; ++iter)
    {
        IndexSetVector g1, g2, g3;

        build_rgraph(bf, radius, g1, iter);
        /* build_rgraph(pf, radius, g2, iter); */
        /* build_rgraph(tf, radius, g3, iter); */

        graphs.push_back(g1);
        radius *= damping;
    }

    if (oprefix)
        for (int iter = 1; const auto& graph : graphs)
            write_rgraph_file(graph, oprefix, iter++);

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

    fprintf(stderr, "[time=%.3f,msg::%s] read points file '%s' [numpts=%lu,filesize=%s]\n", timer.get_elapsed(), __func__, fname, static_cast<size_t>(points.size()), FileInfo(fname).get_file_size_str());
}

template <class Kind>
void build_point_index(PointIndex<Kind>& ptidx, const PointVector& points)
{
    LocalTimer timer;
    timer.start_timer();

    ptidx.build(points.begin(), points.end());

    timer.stop_timer();

    fprintf(stderr, "[time=%.3f,msg::%s] :: [-]\n", timer.get_elapsed(), __func__);
}

template <class Kind>
void build_rgraph(PointIndex<Kind>& ptidx, double radius, IndexSetVector& rgraph, int iter)
{
    LocalTimer timer;
    timer.start_timer();

    Index num_edges = ptidx.build_rgraph(static_cast<Real>(radius), rgraph);

    timer.stop_timer();

    fprintf(stderr, "[time=%.3f,msg::%s] :: [num_verts=%lu,num_edges=%lu,avg_deg=%.3f]\n", timer.get_elapsed(), __func__, rgraph.size(), static_cast<size_t>(num_edges), (num_edges+0.0)/rgraph.size());
}

void write_rgraph_file(const IndexSetVector& rgraph, const char *oprefix, int iter)
{
    LocalTimer timer;
    timer.start_timer();

    stringstream ss;
    ss << oprefix << "_" << iter << ".rgraph";
    string fname = ss.str();

    ss.clear();
    ss.str({});

    Index num_edges = 0;

    for (Index u = 0; const auto& neighs : rgraph)
    {
        num_edges += neighs.size();

        for (Index v : neighs)
            ss << u+1 << " " << v+1 << "\n";
    }

    ofstream os;
    os.open(fname, ios::out);

    os << rgraph.size() << " " << num_edges << "\n";
    os << ss.str();

    os.close();

    timer.stop_timer();

    fprintf(stderr, "[time=%.3f,msg::%s] :: wrote file '%s' [filesize=%s]\n", timer.get_elapsed(), __func__, fname.c_str(), FileInfo(fname).get_file_size_str());
}
