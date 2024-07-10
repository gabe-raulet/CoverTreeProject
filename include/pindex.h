#ifndef POINT_INDEX_H_
#define POINT_INDEX_H_

using namespace std;

#include <unordered_set>

template <class PointTraits, class Index>
class PointIndex
{
    public:

        using Real = typename PointTraits::Real;
        using Point = typename PointTraits::Point;

        using IndexVector = vector<Index>;
        using PointVector = vector<Point>;
        using IndexSet = unordered_set<Index>;
        using IndexSetVector = vector<IndexSet>;
        using IndexVectorVector = vector<IndexVector>;

        PointIndex();
        PointIndex(const PointVector& points);

        template <class Iter> requires same_as<typename Iter::value_type, Point>
        void assign(Iter first, Iter last);

        Index size() const { return points.size(); }

        Index build_rgraph(Real radius, IndexSetVector& graph) const;

    private:

        PointVector points;
};

#include "pindex.hpp"

#endif
