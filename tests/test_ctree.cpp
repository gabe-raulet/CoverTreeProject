#include <iostream>
#include "ptidx.h"
#include "ptraits.h"
#include "ptutils.h"
#include "graphutils.h"

using namespace std;

template <class Index, class Real, int D>
void test_ctree(Index n, Real var, Real base, Real cutoff, Real damping, int iters, int seed, bool verify);

int main(int argc, char *argv[])
{
    LocalTimer timer;
    timer.start_timer();

    test_ctree<int64_t, float, 8>(70000, 10.0, 2.0, 2.0, 0.5, 10, 515151, false);
    test_ctree<int64_t, float, 8>(70000, 10.0, 1.5, 2.0, 0.5, 10, 515151, false);
    test_ctree<int64_t, float, 8>(70000, 10.0, 1.3, 2.0, 0.5, 10, 515151, false);

    timer.stop_timer();

    fprintf(stderr, "[time=%.3f, msg::%s] :: command: %s\n", timer.get_elapsed(), __func__, argv[0]);
    return 0;
}

template <class Index, class Real, int D>
void test_ctree(Index n, Real var, Real base, Real cutoff, Real damping, int iters, int seed, bool verify)
{
    using PointTraits = EuclideanTraits<Real, D>;
    using PointUtils = PointUtils<PointTraits, Index>;
    using GraphUtils = GraphUtils<Index>;

    using Point = typename PointTraits::Point;
    using PointVector = typename PointTraits::PointVector;
    using RealVector = typename PointTraits::RealVector;

    using Graph = typename GraphUtils::GraphV;

    using ForceIndex = BruteForcer<PointTraits, Index>;
    using TreeIndex = TreeIndexer<PointTraits, Index>;

    PointVector points;
    PointTraits::generate_random_gaussian(points, n, var, seed, true);

    ForceIndex force_index(cutoff);
    TreeIndex tree_index(base, cutoff);

    if (verify) PointUtils::build_point_index(force_index, points, true);
    PointUtils::build_point_index(tree_index, points, true);

    Graph g1, g2;
    RealVector radii(iters, cutoff);
    for (int i = 1; i < iters; ++i) radii[i] = radii[i-1]*damping;

    for (Real radius : radii)
    {
        PointUtils::build_rgraph(tree_index, radius, g1, true);
        if (verify)
        {
            PointUtils::build_rgraph(force_index, radius, g2, true);
            assert((GraphUtils::compare_graphs(g1, g2, true)));
        }
    }
}
