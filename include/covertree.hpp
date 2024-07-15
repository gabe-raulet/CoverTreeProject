template <class PointTraits, class Index>
template <class Iter>
void CoverTree<PointTraits, Index>::build(Iter first, Iter last, Real _base)
{
    base = _base;
    points.assign(first, last);
    init();
}

template <class PointTraits, class Index>
template <class IndexContainer>
Index CoverTree<PointTraits, Index>::radii_query(const Point& query, Real radius, IndexContainer& ids) const
{
    ids.clear();
    IndexVector stack = {0}, children;
    auto distance = PointTraits::distance();

    IndexSet ids_;

    while (!stack.empty())
    {
        Index u = stack.back(); stack.pop_back();

        if (distance(query, points[tree.get_item(u)]) <= radius)
            ids_.insert(tree.get_item(u));

        tree.get_children(u, children);

        for (Index v : children)
            if (distance(query, points[tree.get_item(v)]) <= radius + max_radius * level_radius(tree.get_level(v)))
                stack.push_back(v);
    }

    if constexpr (same_as<IndexContainer, IndexSet>) ids = ::move(ids_);
    else ids.assign(ids_.begin(), ids_.end());

    return ids.size();
}

template <class PointTraits, class Index>
void CoverTree<PointTraits, Index>::init()
{
    using TaskQueue = list<tuple<Index, IndexVector>>; /* tuple = (tree vertex id, cell point ids) */

    TaskQueue tasks;

    tasks.emplace_back(); auto& [root_hub, root_hub_ids] = tasks.front();

    root_hub_ids.resize(size());
    iota(root_hub_ids.begin(), root_hub_ids.end(), static_cast<Index>(0));

    root_hub = tree.add_vertex(root_hub_ids.front(), -1);
    assert(root_hub == 0);

    VoronoiCell vcell;
    PointVector hub_pts;
    IndexVector hub_ids;
    vector<IndexVector> cell_ids;
    Index num_cells, hub;

    while (!tasks.empty())
    {
        tie(hub, hub_ids) = tasks.front(); tasks.pop_front();

        iselect(points, hub_ids, hub_pts);

        vcell.assign(hub_pts);
        vcell.build(base);

        if (hub == 0) max_radius = vcell.get_max_radius();

        if (!vcell.is_leaf())
        {
            vcell.get_cover_cell_ids(cell_ids);

            for (const auto& new_hub_ids : cell_ids)
            {
                tasks.emplace_back(); auto& [child_hub, child_hub_ids] = tasks.back();

                child_hub_ids.resize(new_hub_ids.size());
                transform(new_hub_ids.begin(), new_hub_ids.end(), child_hub_ids.begin(), [&hub_ids](Index id) { return hub_ids[id]; });

                child_hub = tree.add_vertex(child_hub_ids.front(), hub);
            }
        }
        else tree.add_vertices(hub_ids.begin(), hub_ids.end(), hub);
    }
}
