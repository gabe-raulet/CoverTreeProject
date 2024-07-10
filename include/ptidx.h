#ifndef POINT_INDEX_H_
#define POINT_INDEX_H_

using namespace std;

#include <unordered_set>

template <class Iter, class T>
concept is_iter_type = same_as<typename Iter::value_type, T>;

template <class PointTraits, class Index>
class PointIndexer
{
    public:

        using Real = typename PointTraits::Real;
        using Point = typename PointTraits::Point;

        using PointVector = vector<Point>;
        using IndexSet = unordered_set<Index>;
        using IndexSetVector = vector<IndexSet>;

        PointIndexer() {}
        PointIndexer(const PointVector& points) : points(points) {}

        template <class Iter>
        requires is_iter_type<Iter, Point>
        void assign(Iter first, Iter last) { points.assign(first, last); }

        Index size() const { return points.size(); }

        virtual Index build_rgraph(Real radius, IndexSetVector& graph) const = 0;

    protected:

        PointVector points;
};

template <class PointTraits, class Index>
class BruteForcer : public PointIndexer<PointTraits, Index>
{
    public:

        using PointIndexer<PointTraits, Index>::size;
        using PointIndexer<PointTraits, Index>::points;

        using Real = typename PointTraits::Real;

        using IndexSet = unordered_set<Index>;
        using IndexSetVector = vector<IndexSet>;

        Index build_rgraph(Real radius, IndexSetVector& graph) const override;
};

#include "ptidx.hpp"

#endif
