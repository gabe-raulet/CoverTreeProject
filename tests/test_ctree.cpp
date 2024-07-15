#include <iostream>
#include "ptidx.h"
#include "ptraits.h"
#include "ptutils.h"
#include "graphutils.h"

using namespace std;

template <class Index, class Real, int D>
void test_ctree(Index n, Real var, Real cutoff, Real damping, int iters, bool verify);

int main(int argc, char *argv[])
{
    test_ctree<int64_t, float, 8>(100000, 10.0, 10.0, 0.9, 25, true);

    return 0;
}

template <class Index, class Real, int D>
void test_ctree(Index n, Real var, Real cutoff, Real damping, int iters, bool verify)
{
    using PointTraits = EuclideanTraits<Real, D>;
    using PointUtils = PointUtils<PointTraits, Index>;
    using GraphUtils = GraphUtils<Index>;

    using Point = typename PointTraits::Point;
    using PointVector = typename PointTraits::PointVector;
    using RealVector = typename PointTraits::RealVector;

    using Graph = typename GraphUtils::GraphV;

    using ForceIndex = BruteForcer<PointTraits, Index>;
    /* using TreeIndex = TreeIndexer<PointTraits, Index>; */

    PointVector points;
    PointTraits::generate_random_gaussian(points, n, var, -1, true);

    ForceIndex force_index(cutoff);
    PointUtils::build_point_index(force_index, points, true);

    Graph g;
    RealVector radii(iters, cutoff);
    for (int i = 1; i < iters; ++i) radii[i] = radii[i-1]*damping;

    for (Real radius : radii)
    {
        PointUtils::build_rgraph(force_index, radius, g, true);
    }
}
