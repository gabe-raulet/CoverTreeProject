#ifndef COVER_TREE_H_
#define COVER_TREE_H_

#include "itree.h"
#include "vcell.h"
#include "misc.h"
#include <list>
#include <tuple>
#include <unordered_set>

using namespace std;

template <class PointTraits, class Index>
class CoverTree
{
    public:

        using Real = typename PointTraits::Real;
        using Point = typename PointTraits::Point;

        using Tree = InsertTree<Index, Index>;
        using VoronoiCell = VoronoiCell<PointTraits, Index>;

        using PointVector = vector<Point>;
        using IndexVector = vector<Index>;
        using RealVector = vector<Real>;

        using IndexSet = unordered_set<Index>;
        using IndexSetVector = vector<IndexSet>;

        template <class Iter>
        void build(Iter first, Iter last, Real base);

        Index radii_query(const Point& query, Real radius, IndexSet& ids) const;
        Index radii_query(const Index& query, Real radius, IndexSet& ids) const { return radii_query(points[query], radius, ids); }

        Index size() const { return points.size(); }
        Real get_max_radius() const { return max_radius; }

    private:

        Real base, max_radius;
        PointVector points;
        Tree tree;

        void init();

        Real level_radius(Index level) const { return ::pow(base, -1. * level); }
};

#include "covertree.hpp"

#endif
