#include <iostream>
#include "ptidx.h"
#include "ptraits.h"
#include "ptutils.h"
#include "graphutils.h"

using namespace std;

int main(int argc, char *argv[])
{
    using PointTraits = EuclideanTraits<float, 3>;
    using PointUtils = PointUtils<PointTraits>;

    using Real = PointTraits::Real;

    using Point = PointTraits::Point;
    using Index = PointUtils::Index;

    using PointVector = PointTraits::PointVector;
    using RealVector = PointTraits::RealVector;
    using BruteForce = BruteForcer<PointTraits, Index>;

    PointVector points;
    PointTraits::generate_random_gaussian(points, 10000, 10., -1, true);

    BruteForce bf(2.0);
    PointUtils::build_point_index(bf, points, true);

    using Graph = GraphUtils<Index>::GraphV;

    Graph g;

    for (Real radius : {2.0, 1.0, 0.5, 0.25, 0.1})
        PointUtils::build_rgraph(bf, radius, g, true);

    return 0;
}
