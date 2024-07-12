template <class Index>
template <class Graph>
Index GraphUtils<Index>::num_edges(const Graph& g)
{
    Index m = 0;
    for_each(g.begin(), g.end(), [&m](const auto& es) { m += es.size(); });
    return m;
}

template <class Index>
template <class Graph, class Real>
void GraphUtils<Index>::get_info(const Graph& g, Index& verts, Index& edges, Real& avg_deg)
{
    verts = g.size();
    edges = num_edges(g);
    avg_deg = (edges+0.0)/(verts+0.0);
}

template <class Index>
template <class Graph>
Index GraphUtils<Index>::read_graph_file(Graph& g, const char *fname, bool verbose)
{
    LocalTimer timer;
    timer.start_timer();

    ifstream is;
    string line;
    Index n, m, u, v;

    is.open(fname);
    getline(is, line);
    istringstream(line) >> n >> m;

    g.clear(), g.resize(n);

    while (getline(is, line))
    {
        istringstream(line) >> u >> v;
        auto& es = g[u-1];
        es.insert(es.end(), v-1);
    }

    is.close();
    timer.stop_timer();

    if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] read file '%s' [num_verts=%lu,num_edges=%lu,filesize=%s]\n", timer.get_elapsed(), __func__, fname, (size_t)n, (size_t)m, FileInfo(fname).get_file_size_str());

    return m;
}

template <class Index>
template <class Graph>
void GraphUtils<Index>::write_graph_file(const Graph& g, const char *fname, bool verbose)
{
    LocalTimer timer;
    timer.start_timer();

    ofstream os;
    os.open(fname, ios::out);

    Index n = g.size();
    Index m = num_edges(g);

    os << n << " " << m << "\n";

    for (Index u = 0; const auto& neighs : g)
    {
        for (Index v : neighs)
            os << u+1 << " " << v+1 << "\n";

        u++;
    }

    os.close();
    timer.stop_timer();

    if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] :: wrote file '%s' [filesize=%s]\n", timer.get_elapsed(), __func__, fname, FileInfo(fname).get_file_size_str());
}

template <class Index>
template <class Graph>
bool GraphUtils<Index>::compare_graphs(const Graph& g1, const Graph& g2, bool verbose)
{
    LocalTimer timer;
    timer.start_timer();

    if (g1.size() != g2.size())
    {
        timer.stop_timer();
        if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] graphs differ\n", timer.get_elapsed(), __func__);
        return false;
    }

    for (Index i = 0; i < g1.size(); ++i)
        if (g1[i] != g2[i])
        {
            timer.stop_timer();
            if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] graphs differ\n", timer.get_elapsed(), __func__);
            return false;
        }

    timer.stop_timer();
    if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] graphs are identical\n", timer.get_elapsed(), __func__);
    return true;
}

template <class Index>
template <class Graph, class Real>
Index GraphUtils<Index>::erdos_renyi(Graph& g, Index n, Real p, int seed)
{
    random_device rd;
    default_random_engine gen(seed < 0? rd() : seed*17);
    return erdos_renyi(g, n, p, gen);
}

template <class Index>
template <class Graph, class Real, class RandomGen>
Index GraphUtils<Index>::erdos_renyi(Graph& g, Index n, Real p, RandomGen& gen)
{
    g.clear();

    Index m = 0;
    vector<bool> bits(n);
    uniform_real_distribution<Real> dist{0.0, 1.0};

    for (Index i = 0; i < n; ++i)
    {
        generate(bits.begin(), bits.end(), [&gen, &dist, &p] () { return dist(gen) <= p; });

        g.emplace_back();
        auto& dest = g.back();

        for (Index i = 0; i < n; ++i)
            if (bits[i])
                dest.insert(dest.end(), i);

        m += dest.size();
    }

    return m;
}

template <class Index>
void GraphUtils<Index>::shuffle_vector_graph(VecGraph& g, int seed)
{
    random_device rd;
    default_random_engine gen(seed < 0? rd() : seed*17);
    shuffle_vector_graph(g, gen);
}

template <class Index>
template <class RandomGen>
void GraphUtils<Index>::shuffle_vector_graph(VecGraph& g, RandomGen& gen)
{
    for (auto& es : g) shuffle(es.begin(), es.end(), gen);
}
