template <class PointTraits, class Index>
Index BruteForcer<PointTraits, Index>::build_rgraph(Real radius, IndexSetVector& graph) const
{
    graph.clear();
    auto distance = PointTraits::distance();
    Index num_edges = 0;

    for (Index i = 0; i < size(); ++i)
    {
        graph.emplace_back();
        auto& neighs = graph.back();

        for (Index j = 0; j < size(); ++j)
            if (distance(points[i], points[j]) <= radius)
                neighs.insert(j);

        num_edges += neighs.size();
    }

    return num_edges;
}

template <class PointTraits, class Index>
template <class Iter> requires is_iter_type<Iter, typename PointTraits::Point>
void PrunedForcer<PointTraits, Index>::assign(Iter first, Iter last)
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
