#ifndef COVER_TREE_INDEX_H_
#define COVER_TREE_INDEX_H_

#include "itree.h"
#include "vcell.h"
#include <list>
#include <tuple>
#include <unordered_set>

using namespace std;

template <class PointTraits, class Index>
class TreeIndex
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

        TreeIndex(const PointVector& points, Real base);

        Index build_rgraph(Real radius, IndexSetVector& rgraph) const;
        Index radii_query(const Point& query, Real radius, IndexSet& ids) const;

        Index size() const { return points.size(); }

        Real get_max_radius() const { return max_radius; }

    private:

        Real base, max_radius;
        PointVector points;
        Tree tree;

        void init();

        Real level_radius(Index level) const { return ::pow(base, -1. * level); }
};

#include "treeindex.hpp"

#endif
