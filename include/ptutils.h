#ifndef POINT_UTILS_H_
#define POINT_UTILS_H_

#include <vector>
#include "timer.h"
#include "fileinfo.h"

using namespace std;

template <class PointTraits, class Index>
struct PointUtils
{
    using Real = typename PointTraits::Real;
    using Point = typename PointTraits::Point;

    using PointVector = vector<Point>;

    static void read_points_file(PointVector& points, const char *fname, bool verbose = false);
};

#include "ptutils.hpp"

#endif
