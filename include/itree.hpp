template <class T, class Index>
InsertTree<T, Index>::InsertTree()
    : nlevels(0) {}

template <class T, class Index>
InsertTree<T, Index>::InsertTree(const InsertTree& rhs)
    : vertices(rhs.vertices),
      levels(rhs.levels),
      children(rhs.children),
      nlevels(rhs.nlevels) {}

template <class T, class Index>
Index InsertTree<T, Index>::add_vertex(T item, Index parent)
{
    Index vtx_level;
    Index vtx = num_vertices();

    vertices.push_back(item);
    children.emplace_back();

    if (parent >= 0)
    {
        vtx_level = levels[parent] + 1;
        children[parent].push_back(vtx);
    }
    else vtx_level = 0;

    nlevels = ::max(vtx_level+1, nlevels);
    levels.push_back(vtx_level);

    return vtx;
}

template <class T, class Index>
Index InsertTree<T, Index>::get_children(Index parent, IndexVector& ids) const
{
    ids.resize(children[parent].size());
    copy(children[parent].begin(), children[parent].end(), ids.begin());
    return ids.size();
}

template <class T, class Index>
bool InsertTree<T, Index>::operator==(const InsertTree& rhs) const
{
    const InsertTree& lhs = *this;

    if (lhs.num_vertices() != rhs.num_vertices() || lhs.num_levels() != rhs.num_levels())
        return false;

    IndexVector stack1 = {0};
    IndexVector stack2 = {0};

    while (!stack1.empty())
    {
        Index u1 = stack1.back(); stack1.pop_back();
        Index u2 = stack2.back(); stack2.pop_back();

        if (lhs.vertices[u1] != rhs.vertices[u2])
            return false;

        if (lhs.children[u1].size() != rhs.children[u2].size())
            return false;

        vector<tuple<T, Index>> ch1, ch2;

        for (Index v : lhs.children[u1]) ch1.emplace_back(lhs.vertices[v], v);
        for (Index v : rhs.children[u2]) ch2.emplace_back(rhs.vertices[v], v);

        sort(ch1.begin(), ch1.end());
        sort(ch2.begin(), ch2.end());

        for (const auto& [item, id] : ch1) stack1.push_back(id);
        for (const auto& [item, id] : ch2) stack2.push_back(id);
    }

    return true;
}

template <class T, class Index>
template <class Record, class Op>
void InsertTree<T, Index>::dfs(Index start, vector<Record>& records, Op op) const
{
    records.clear();
    IndexVector stack = {start};

    while (!stack.empty())
    {
        Index u = stack.back(); stack.pop_back();
        records.push_back(op(u, *this));

        for (Index v : children[u])
            stack.push_back(v);
    }
}
