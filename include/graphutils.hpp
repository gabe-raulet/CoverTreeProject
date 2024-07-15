template <class Index_>
template <class Graph>
typename GraphUtils<Index_>::Index
GraphUtils<Index_>::num_edges(const Graph& g)
{
    Index m = 0;
    for_each(g.begin(), g.end(), [&m](const auto& es) { m += es.size(); });
    return m;
}

template <class Index_>
template <class Graph>
double GraphUtils<Index_>::read_graph_file(Graph& g, const char *fname, bool verbose)
{
    double elapsed;
    LocalTimer timer;
    timer.start_timer();

    ifstream is;
    string line;
    Index n, m, u, v;

    is.open(fname);
    getline(is, line);
    istringstream(line) >> n >> m;
    g.clear(), g.resize(n);

    m = 0;
    while (getline(is, line))
    {
        istringstream(line) >> u >> v;
        auto& es = g[u-1];
        es.insert(es.end(), v-1);
        m++;
    }

    is.close();

    timer.stop_timer();
    elapsed = timer.get_elapsed();

    if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] :: read graph file '%s' :: [verts=%lu,edges=%lu,meandeg=%.3f,filesize=%s]\n", elapsed, __func__, fname, (size_t)n, (size_t)m, (m+0.0)/(n+0.0), FileInfo(fname).get_file_size_str());

    return elapsed;
}

template <class Index_>
template <class Graph>
double GraphUtils<Index_>::write_graph_file(const Graph& g, const char *fname, bool verbose)
{
    double elapsed;
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
    elapsed = timer.get_elapsed();

    if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] :: wrote graph file '%s' :: [verts=%lu,edges=%lu,meandeg=%.3f,filesize=%s]\n", elapsed, __func__, fname, (size_t)n, (size_t)m, (m+0.0)/(n+0.0), FileInfo(fname).get_file_size_str());

    return elapsed;
}

template <class Index_>
template <class Graph1, class Graph2>
bool GraphUtils<Index_>::compare_graphs(const Graph1& g1, const Graph2& g2, bool verbose)
{
    LocalTimer timer;
    timer.start_timer();

    size_t n1, n2, m1, m2;

    n1 = g1.size();
    n2 = g2.size();

    if (n1 != n2)
    {
        timer.stop_timer();
        if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] :: graphs differ :: [n1=%lu,n2=%lu]\n", timer.get_elapsed(), __func__, n1, n2);
        return false;
    }

    m1 = num_edges(g1);
    m2 = num_edges(g2);

    if (m1 != m2)
    {
        timer.stop_timer();
        if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] :: graphs differ :: [m1=%lu,m2=%lu]\n", timer.get_elapsed(), __func__, m1, m2);
        return false;
    }

    timer.stop_timer();

    bool correct = true;

    for (size_t i = 0; i < n1; ++i)
    {
        if constexpr (same_as<Graph1, GraphV> && same_as<Graph2, GraphV>)
        {
            correct = is_permutation(g1[i].begin(), g1[i].end(), g2[i].begin());
        }
        else if constexpr (same_as<Graph1, GraphV> && same_as<Graph2, GraphS>)
        {
            IndexSet tmp(g1[i].begin(), g1[i].end());
            correct = (tmp == g2[i]);
        }
        else if constexpr (same_as<Graph1, GraphS> && same_as<Graph2, GraphV>)
        {
            IndexSet tmp(g2[i].begin(), g2[i].end());
            correct = (tmp == g1[i]);
        }
        else correct = (g1[i] == g2[i]);

        if (!correct)
        {
            timer.stop_timer();
            if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] :: graphs differ :: [g1[%lu]!=g2[%lu]]\n", timer.get_elapsed(), __func__, i+1, i+1);
            return false;
        }
    }

    timer.stop_timer();
    if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] :: graphs identical\n", timer.get_elapsed(), __func__);
    return true;
}

template <class Index_>
template <class Graph, class Real, class RandomGen>
double GraphUtils<Index_>::erdos_renyi(Graph& g, Index n, Real p, RandomGen& gen, bool verbose)
{
    double elapsed;
    LocalTimer timer;
    timer.start_timer();

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

    timer.stop_timer();
    elapsed = timer.get_elapsed();

    if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] :: generated G(%lu,%.2f)-graph :: [verts=%lu,edges=%lu,meandeg=%.3f]\n", elapsed, __func__, (size_t)n, p, (size_t)n, (size_t)m, (m+0.0)/(n+0.0));
    return elapsed;
}

template <class Index_>
void GraphUtils<Index_>::shuffle_vector_graph(GraphV& g, int seed)
{
    random_device rd;
    default_random_engine gen(seed < 0? rd() : seed*17);
    for (auto& es : g) shuffle(es.begin(), es.end(), gen);
}
