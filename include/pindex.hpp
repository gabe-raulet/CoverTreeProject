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
