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
