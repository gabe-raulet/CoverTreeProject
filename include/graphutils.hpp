template <class Index>
Index GraphUtils<Index>::read_graph_file(vector<IndexVector>& graph, const char *fname, bool verbose)
{
    LocalTimer timer;
    timer.start_timer();

    ifstream is;
    string line;
    Index n, m, u, v;

    is.open(fname);
    getline(is, line);

    istringstream header(line);
    header  >> n >> m;

    graph.clear(), graph.resize(n);

    while (getline(is, line))
    {
        istringstream ss(line);
        ss >> u >> v;
        graph[u-1].push_back(v-1);
    }

    is.close();
    timer.stop_timer();

    if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] read file '%s' [num_verts=%lld,num_edges=%lld,size=%s]\n", timer.get_elapsed(), __func__, fname, n, m, PrettyFileSize::str(fname).c_str());

    return m;
}

template <class Index>
template <class NeighborContainer>
bool GraphUtils<Index>::compare_graphs(const vector<NeighborContainer>& g1, Index m1, const vector<NeighborContainer>& g2, Index m2, bool verbose)
{
    LocalTimer timer;
    timer.start_timer();

    Index n1 = g1.size();
    Index n2 = g2.size();

    if (n1 != n2 || m1 != m2)
    {
        timer.stop_timer();
        if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] graphs differ\n", timer.get_elapsed(), __func__);
        return false;
    }

    auto comparator = NeighborComparator<NeighborContainer>();

    for (Index i = 0; i < n1; ++i)
        if (!comparator(g1[i], g2[i]))
        {
            timer.stop_timer();
            if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] graphs differ\n", timer.get_elapsed(), __func__);
            return false;
        }

    timer.stop_timer();
    if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] graphs are identical\n", timer.get_elapsed(), __func__);
    return true;
}
