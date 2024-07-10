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
        PointIndex(const PointVector& points) : points(points) {}
        PointIndex(const PointVector& points, Real cutoff);

        Index size() const { return points.size(); }

    private:

        PointVector points;
};

#endif
