template <class PointTraits_, class Index_, class Kind>
typename PointIndexer<PointTraits_, Index_, Kind>::Index
PointIndexer<PointTraits_, Index_, Kind>::size() const
{
    return static_cast<const Kind&>(*this).size();
}

template <class PointTraits_, class Index_, class Kind>
template <class Iter>
void PointIndexer<PointTraits_, Index_, Kind>::build(Iter first, Iter last)
{
    return static_cast<Kind&>(*this).build(first, last);
}

template <class PointTraits_, class Index_, class Kind>
template <class Container>
void PointIndexer<PointTraits_, Index_, Kind>::radii_query(Index id, Real radius, Container& ids) const
{
    return static_cast<const Kind&>(*this).radii_query(id, radius, ids);
}

template <class PointTraits_, class Index_, class Kind>
template <class Graph>
typename PointIndexer<PointTraits_, Index_, Kind>::Index
PointIndexer<PointTraits_, Index_, Kind>::build_rgraph(Real radius, Graph& g) const
{
    return static_cast<const Kind&>(*this).build_rgraph(radius, g);
}

template <class PointTraits_, class Index_, class Kind>
const char* PointIndexer<PointTraits_, Index_, Kind>::repr() const
{
    return static_cast<const Kind&>(*this).repr();
}

template <class PointTraits_, class Index_>
template <class Iter>
void BruteForcer<PointTraits_, Index_>::build(Iter first, Iter last)
{
    points.assign(first, last);

    cutoff_neighs.clear();
    auto distance = PointTraits::distance();

    for (Index i = 0; i < size(); ++i)
    {
        cutoff_neighs.emplace_back();
        auto& neighs = cutoff_neighs.back();
        auto it = inserter(neighs, neighs.end());

        for (Index j = 0; j < size(); ++j)
            if (distance(points[i], points[j]) <= cutoff)
                *it++ = j;
    }
}

template <class PointTraits_, class Index_>
template <class Container>
void BruteForcer<PointTraits_, Index_>::radii_query(Index id, Real radius, Container& ids) const
{
    ids.clear();

    auto it = inserter(ids, ids.end());
    auto distance = PointTraits::distance();

    for (Index i : cutoff_neighs[id])
        if (distance(points[id], points[i]) <= radius)
            *it++ = i;
}

template <class PointTraits_, class Index_>
template <class Graph>
typename BruteForcer<PointTraits_, Index_>::Index
BruteForcer<PointTraits_, Index_>::build_rgraph(Real radius, Graph& g) const
{
    g.clear();
    Index m = 0;

    for (Index i = 0; i < size(); ++i)
    {
        g.emplace_back();
        radii_query(i, radius, g.back());
        m += g.back().size();
    }

    return m;
}
