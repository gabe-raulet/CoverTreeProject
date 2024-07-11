template <class PointTraits, class Index>
struct GraphInserter
{
    using Real = typename PointTraits::Real;
    using Point = typename PointTraits::Point;
    using PointVector = vector<Point>;

    template <class NeighborContainer>
    Index operator()(const PointVector& points, Real radius, vector<NeighborContainer>& graph) const
    {
        graph.clear();
        auto distance = PointTraits::distance();
        Index num_edges = 0;

        for (Index i = 0; i < points.size(); ++i)
        {
            graph.emplace_back();
            auto& neighs = graph.back();
            auto it = inserter(neighs, neighs.end());

            for (Index j = 0; j < points.size(); ++j)
                if (distance(points[i], points[j]) <= radius)
                    *it++ = j;

            num_edges += neighs.size();
        }

        return num_edges;
    }
};

template <class PointTraits, class Index>
Index BruteForcer<PointTraits, Index>::build_rgraph(Real radius, IndexSetVector& graph) const
{
    return GraphInserter<PointTraits, Index>()(points, radius, graph);
}

template <class PointTraits, class Index>
template <class Iter> requires is_iter_type<Iter, typename PointTraits::Point>
void PrunedForcer<PointTraits, Index>::assign(Iter first, Iter last)
{
    points.assign(first, last);
    GraphInserter<PointTraits, Index>()(points, cutoff, cutoff_neighs);
}

template <class PointTraits, class Index>
Index PrunedForcer<PointTraits, Index>::build_rgraph(Real radius, IndexSetVector& graph) const
{
    graph.clear();
    auto distance = PointTraits::distance();
    Index num_edges = 0;

    for (Index i = 0; i < size(); ++i)
    {
        graph.emplace_back();
        auto& neighs = graph.back();

        for (Index j : cutoff_neighs[i])
            if (distance(points[i], points[j]) <= radius)
                neighs.insert(j);

        num_edges += neighs.size();
    }

    return num_edges;
}
