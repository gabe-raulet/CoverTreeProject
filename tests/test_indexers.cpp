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


    return 0;
}

void generate_points(vector<Point>& points, Index totsize, double var, int seed)
{
    LocalTimer timer;
    timer.start_timer();

    PointGenerator<PointTraits, Index> ptgen(seed);
    ptgen.generate_points(points, totsize, var);

    timer.stop_timer();
    fprintf(stderr, "[time=%.3f,msg::%s] :: generated %lu points\n", timer.get_elapsed(), __func__, static_cast<size_t>(totsize));
}

template <class Kind>
void build_point_index(PointIndex<Kind>& ptidx, const vector<Point>& points)
{
    LocalTimer timer;
    timer.start_timer();

    ptidx.build(points.begin(), points.end());

    timer.stop_timer();

    fprintf(stderr, "[time=%.3f,msg::%s] :: built index using %s indexer\n", Kind::repr());
}

template <class Kind>
void build_rgraph(PointIndex<Kind>& ptidx, double radius, vector<unordered_set<Index>>& graph)
{
    LocalTimer timer;
    timer.start_timer();

    Index num_edges = ptidx.build_rgraph(static_cast<Real>(radius), graph);

    timer.stop_timer();

    fprintf(stderr, "[time=%.3f,msg::%s] :: [num_verts=%lu,num_edges=%lu,avg_deg=%.3f]\n", timer.get_elapsed(), __func__, graph.size(), static_cast<size_t>(num_edges), (num_edges+0.0)/graph.size());
}
