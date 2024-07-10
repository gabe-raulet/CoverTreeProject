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

int main(int argc, char *argv[])
{
    random_device rd;
    default_random_engine gen{rd()};
    normal_distribution<Real> dist{0.0, 10.0};

    vector<Point> points(100);
    PointTraits::fill_random_points(points.begin(), points.end(), gen, dist);

    PIndex ptidx(points);

    return 0;
}
