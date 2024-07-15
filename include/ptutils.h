#ifndef POINT_UTILS_H_
#define POINT_UTILS_H_

using namespace std;

#include "ptidx.h"
#include "ptraits.h"
#include "fileinfo.h"
#include "timer.h"

template <class PointTraits_, class Index_ = int64_t>
struct PointUtils
{
    using PointTraits = PointTraits_;
    using Index = Index_;

    using Real = typename PointTraits::Real;
    using Point = typename PointTraits::Point;

    template <class Kind>
    using PointIndex = PointIndexer<PointTraits, Index, Kind>;

    template <class PointContainer>
    static double read_points_file(PointContainer& points, const char *fname, bool verbose = false);

    template <class PointContainer>
    static double write_points_file(const PointContainer& points, const char *fname, bool verbose = false);

    template <class PointContainer1, class PointContainer2>
    static bool compare_points(const PointContainer1& lhs, const PointContainer2& rhs);

    template <class PointContainer, class Kind>
    static double build_point_index(PointIndex<Kind>& ptidx, const PointContainer& points, bool verbose = false);

    template <class Graph, class Kind>
    static double build_rgraph(PointIndex<Kind>& ptidx, Real radius, Graph& g, bool verbose = false);
};

#include "ptutils.hpp"

#endif
