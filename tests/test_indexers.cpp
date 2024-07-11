#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <stdio.h>
#include "ptgen.h"
#include "ptraits.h"
#include "ptidx.h"
#include "timer.h"

using namespace std;

using Index = int64_t;
using PointTraits = SelectPoint<FPSIZE, PTDIM>::PointTraits;
using Real = PointTraits::Real;
using Point = PointTraits::Point;

template <class Kind>
using PointIndex = PointIndexer<PointTraits, Index, Kind>;

using BruteForce = BruteForcer<PointTraits, Index>;
using PrunedForce = PrunedForcer<PointTraits, Index>;
using TreeIndex = CoverTreeIndex<PointTraits, Index>;

void generate_points(vector<Point>& points, Index totsize, double var, int seed);

template <class Kind>
void build_point_index(PointIndex<Kind>& ptidx, const vector<Point>& points);

template <class Kind>
void build_rgraph(PointIndex<Kind>& ptidx, double radius, vector<unordered_set<Index>>& graph);

int main(int argc, char *argv[])
{
    const vector<Index> counts = {100, 1000, 25000};
    const vector<double> vars = {100.0, 33.3, 10.};
    const vector<Real> radii = {1.0, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1};

    int seed;

    for (Index n : counts)
        for (double var : vars)
        {
            vector<Point> points;
            random_device rd; seed = rd();
            generate_points(points, n, var, seed);

            BruteForce bf;
            PrunedForce pf(radii.front());
            TreeIndex ti(2.0);

            build_point_index(bf, points);
            build_point_index(pf, points);
            build_point_index(ti, points);

            for (Real radius : radii)
            {
                vector<unordered_set<Index>> g1, g2, g3;

                build_rgraph(bf, radius, g1);
                build_rgraph(pf, radius, g2);
                build_rgraph(ti, radius, g3);
            }
        }

    return 0;
}

void generate_points(vector<Point>& points, Index totsize, double var, int seed)
{
    LocalTimer timer;
    timer.start_timer();

    PointGenerator<PointTraits, Index> ptgen(seed);
    ptgen.generate_points(points, totsize, var);

    timer.stop_timer();
    fprintf(stderr, "[time=%.3f,msg::%s] :: generated %lu points [var=%.3f,seed=%d]\n", timer.get_elapsed(), __func__, static_cast<size_t>(totsize), var, seed);
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
void build_rgraph(PointIndex<Kind>& ptidx, double radius, vector<unordered_set<Index>>& graph)
{
    LocalTimer timer;
    timer.start_timer();

    Index num_edges = ptidx.build_rgraph(static_cast<Real>(radius), graph);

    timer.stop_timer();

    fprintf(stderr, "[time=%.3f,msg::%s] :: built r-graph with r=%.3f [num_verts=%lu,num_edges=%lu,avg_deg=%.3f]\n", timer.get_elapsed(), __func__, radius, graph.size(), static_cast<size_t>(num_edges), (num_edges+0.0)/graph.size());
}
