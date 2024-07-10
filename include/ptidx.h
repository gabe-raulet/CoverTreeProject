#ifndef POINT_INDEX_H_
#define POINT_INDEX_H_

using namespace std;

#include <unordered_set>

template <class Iter, class T>
concept is_iter_type = same_as<typename Iter::value_type, T>;

template <class PointTraits, class Index, class Kind>
class PointIndexer
{
    public:

        using Real = typename PointTraits::Real;
        using Point = typename PointTraits::Point;

        using PointVector = vector<Point>;
        using IndexSet = unordered_set<Index>;
        using IndexSetVector = vector<IndexSet>;

        Index size() const { return points.size(); }

        template <class Iter> requires is_iter_type<Iter, Point>
        void assign(Iter first, Iter last)
        {
            static_cast<Kind&>(*this).assign(first, last);
        }

        Index build_rgraph(Real radius, IndexSetVector& graph) const
        {
            return static_cast<const Kind&>(*this).build_rgraph(radius, graph);
        }

    protected:

        PointVector points;
};

template <class PointTraits, class Index>
class BruteForcer : public PointIndexer<PointTraits, Index, BruteForcer<PointTraits, Index>>
{
    public:

        using base_type = PointIndexer<PointTraits, Index, BruteForcer>;

        using base_type::size;
        using base_type::points;

        using typename base_type::Real;
        using typename base_type::Point;
        using typename base_type::IndexSet;
        using typename base_type::IndexSetVector;

        template <class Iter> requires is_iter_type<Iter, Point>
        void assign(Iter first, Iter last)
        {
            points.assign(first, last);
        }

        Index build_rgraph(Real radius, IndexSetVector& graph) const;
};

template <class PointTraits, class Index>
class PrunedForcer : public PointIndexer<PointTraits, Index, PrunedForcer<PointTraits, Index>>
{
    public:

        using base_type = PointIndexer<PointTraits, Index, PrunedForcer>;

        using base_type::size;
        using base_type::points;

        using typename base_type::Real;
        using typename base_type::Point;
        using typename base_type::IndexSet;
        using typename base_type::IndexSetVector;

        using IndexVector = vector<Index>;
        using IndexVectorVector = vector<IndexVector>;

        PrunedForcer(Real cutoff) : cutoff(cutoff) {}

        template <class Iter> requires is_iter_type<Iter, Point>
        void assign(Iter first, Iter last)
        {
            points.assign(first, last);
            cutoff_neighs.clear();

            auto distance = PointTraits::distance();

            for (Index i = 0; i < size(); ++i)
            {
                cutoff_neighs.emplace_back();
                auto& es = cutoff_neighs.back();

                for (Index j = 0; j < size(); ++j)
                    if (distance(points[i], points[j]) <= cutoff)
                        es.push_back(j);
            }
        }

        Index build_rgraph(Real radius, IndexSetVector& graph) const;

    private:

        Real cutoff;
        IndexVectorVector cutoff_neighs;
};

#include "ptidx.hpp"

#endif
