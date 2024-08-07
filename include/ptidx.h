#ifndef POINT_INDEX_H_
#define POINT_INDEX_H_

using namespace std;

#include "covertree.h"
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

        Index size() const
        {
            return static_cast<const Kind&>(*this).size();
        }

        template <class Iter> requires is_iter_type<Iter, Point>
        void build(Iter first, Iter last)
        {
            static_cast<Kind&>(*this).build(first, last);
        }

        Index build_rgraph(Real radius, IndexSetVector& graph) const
        {
            return static_cast<const Kind&>(*this).build_rgraph(radius, graph);
        }

        const char* repr() const
        {
            return static_cast<const Kind&>(*this).repr();
        }

    protected:

        PointVector points;
};

template <class PointTraits, class Index>
class BruteForcer : public PointIndexer<PointTraits, Index, BruteForcer<PointTraits, Index>>
{
    public:

        static constexpr const char* name = "brute_force";

        using base_type = PointIndexer<PointTraits, Index, BruteForcer>;

        using base_type::size;
        using base_type::points;

        using typename base_type::Real;
        using typename base_type::Point;
        using typename base_type::IndexSet;
        using typename base_type::IndexSetVector;

        template <class Iter> requires is_iter_type<Iter, Point>
        void build(Iter first, Iter last)
        {
            points.assign(first, last);
        }

        Index size() const { return points.size(); }
        Index build_rgraph(Real radius, IndexSetVector& graph) const;
        const char* repr() const { return name; }
};

template <class PointTraits, class Index>
class PrunedForcer : public PointIndexer<PointTraits, Index, PrunedForcer<PointTraits, Index>>
{
    public:

        static constexpr const char* name = "prune_force";

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

        template <class Iter> requires is_iter_type<Iter, typename PointTraits::Point>
        void build(Iter first, Iter last);

        Index size() const { return points.size(); }
        Index build_rgraph(Real radius, IndexSetVector& graph) const;
        const char* repr() const { return name; }

    private:

        Real cutoff;
        IndexVectorVector cutoff_neighs;
};

template <class PointTraits, class Index>
class CoverTreeIndex : public PointIndexer<PointTraits, Index, CoverTreeIndex<PointTraits, Index>>
{
    public:

        static constexpr const char* name = "cover_tree";

        using base_type = PointIndexer<PointTraits, Index, CoverTreeIndex>;

        using base_type::size;
        using base_type::points;

        using typename base_type::Real;
        using typename base_type::Point;
        using typename base_type::IndexSet;
        using typename base_type::IndexSetVector;

        CoverTreeIndex(Real base) : base(base) {}

        template <class Iter> requires is_iter_type<Iter, typename PointTraits::Point>
        void build(Iter first, Iter last);

        Index size() const { return covertree.size(); }
        Index build_rgraph(Real radius, IndexSetVector& graph) const;
        const char* repr() const { return name; }

    private:

        Real base;
        CoverTree<PointTraits, Index> covertree;
};

#include "ptidx.hpp"

#endif
