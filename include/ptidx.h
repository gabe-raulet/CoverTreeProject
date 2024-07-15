#ifndef POINT_INDEX_H_
#define POINT_INDEX_H_

#include "covertree.h"

using namespace std;

template <class PointTraits_, class Index_, class Kind>
class PointIndexer
{
    public:

        using PointTraits = PointTraits_;
        using Index = Index_;

        using Real = typename PointTraits::Real;
        using Point = typename PointTraits::Point;
        using PointVector = typename PointTraits::PointVector;
        using PointSet = typename PointTraits::PointSet;

        using IndexVector = vector<Index>;
        using IndexVectorVector = vector<IndexVector>;

        Index size() const;

        template <class Iter>
        void build(Iter first, Iter last);

        template <class Container>
        void radii_query(Index id, Real radius, Container& ids) const;

        template <class Graph>
        Index build_rgraph(Real radius, Graph& g) const;

        const char* repr() const;
};

template <class PointTraits_, class Index_>
class BruteForcer : public PointIndexer<PointTraits_, Index_, BruteForcer<PointTraits_, Index_>>
{
    public:

        static constexpr const char *name = "force_index";

        using PointTraits = PointTraits_;
        using Index = Index_;

        using base_type = PointIndexer<PointTraits, Index, BruteForcer<PointTraits, Index>>;

        using typename base_type::Real;
        using typename base_type::Point;
        using typename base_type::PointVector;
        using typename base_type::IndexVector;
        using typename base_type::IndexVectorVector;

        BruteForcer(Real cutoff = numeric_limits<Real>::max()) : cutoff(cutoff) {}

        template <class Iter>
        void build(Iter first, Iter last);

        Index size() const { return points.size(); }
        const char* repr() const { return name; }

        template <class Container>
        void radii_query(Index id, Real radius, Container& ids) const;

        template <class Graph>
        Index build_rgraph(Real radius, Graph& g) const;

    private:

        Real cutoff;
        IndexVectorVector cutoff_neighs;
        PointVector points;
};

template <class PointTraits_, class Index_>
class TreeIndexer : public PointIndexer<PointTraits_, Index_, TreeIndexer<PointTraits_, Index_>>
{
    public:

        static constexpr const char *name = "ctree_index";

        using PointTraits = PointTraits_;
        using Index = Index_;

        using base_type = PointIndexer<PointTraits, Index, TreeIndexer<PointTraits, Index>>;
        using typename base_type::Real;

        TreeIndexer();
        TreeIndexer(Real base);
        TreeIndexer(Real base, Real cutoff);

        template <class Iter>
        void build(Iter first, Iter last);

        Index size() const { return tree.size(); }
        const char* repr() const { return name; }

        template <class Container>
        void radii_query(Index id, Real radius, Container& ids) const;

        template <class Graph>
        Index build_rgraph(Real radius, Graph& g) const;

    private:

        Real base, cutoff;
        CoverTree<PointTraits, Index> tree;
};

#include "ptidx.hpp"

#endif
