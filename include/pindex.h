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

template <class PointTraits, class Index>
PointIndex<PointTraits, Index>::PointIndex() {}

template <class PointTraits, class Index>
PointIndex<PointTraits, Index>::PointIndex(const PointVector& points)
    : points(points) {}

template <class PointTraits, class Index>
template <class Iter> requires same_as<typename Iter::value_type, typename PointTraits::Point>
void PointIndex<PointTraits, Index>::assign(Iter first, Iter last)
{
    points.assign(first, last);
}

template <class PointTraits, class Index>
Index PointIndex<PointTraits, Index>::build_rgraph(Real radius, IndexSetVector& graph) const
{
    graph.clear();
    auto distance = PointTraits::distance();
    Index num_edges = 0;

    for (Index i = 0; i < size(); ++i)
    {
        graph.emplace_back(); auto& neighs = graph.back();

        for (Index j = 0; j < size(); ++j)
            if (distance(points[i], points[j]) <= radius)
                neighs.insert(j);

        num_edges += neighs.size();
    }

    return num_edges;
}

#endif
