template <class PointTraits_, class Index_>
template <class PointContainer>
double PointUtils<PointTraits_, Index_>::read_points_file(PointContainer& points, const char *fname, bool verbose)
{
    double elapsed;
    LocalTimer timer;
    timer.start_timer();

    points.clear();
    PointTraits::read_from_file(inserter(points, points.end()), fname);

    timer.stop_timer();
    elapsed = timer.get_elapsed();

    if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] :: read %lu points from file '%s' :: [filesize=%s]\n", elapsed, __func__, (size_t)points.size(), fname, FileInfo(fname).get_file_size_str());
    return elapsed;
}

template <class PointTraits_, class Index_>
template <class PointContainer>
double PointUtils<PointTraits_, Index_>::write_points_file(const PointContainer& points, const char *fname, bool verbose)
{
    double elapsed;
    LocalTimer timer;
    timer.start_timer();

    PointTraits::write_to_file(points.begin(), points.end(), fname);

    timer.stop_timer();
    elapsed = timer.get_elapsed();

    if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] :: wrote %lu points to file '%s' :: [filesize=%s]\n", elapsed, __func__, (size_t)points.size(), fname, FileInfo(fname).get_file_size_str());
    return elapsed;
}

template <class PointTraits_, class Index_>
template <class PointContainer1, class PointContainer2>
bool PointUtils<PointTraits_, Index_>::compare_points(const PointContainer1& lhs, const PointContainer2& rhs)
{
    using PointSet = typename PointTraits::PointSet;

    if      constexpr (same_as<PointContainer1, PointContainer2>) { return (lhs == rhs); }
    else if constexpr (same_as<PointContainer1, PointSet>)        { return (lhs == PointSet(rhs.begin(), rhs.end())); }
    else if constexpr (same_as<PointContainer2, PointSet>)        { return (PointSet(lhs.begin(), lhs.end()) == rhs); }
    else return false;
}

template <class PointTraits_, class Index_>
template <class PointContainer, class Kind>
double PointUtils<PointTraits_, Index_>::build_point_index(PointIndex<Kind>& ptidx, const PointContainer& points, bool verbose)
{
    double elapsed;
    LocalTimer timer;
    timer.start_timer();

    ptidx.build(points.begin(), points.end());

    timer.stop_timer();
    elapsed = timer.get_elapsed();

    if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] :: [indexer=%s]\n", elapsed, __func__, ptidx.repr());
    return elapsed;
}

template <class PointTraits_, class Index_>
template <class Graph, class Kind>
double PointUtils<PointTraits_, Index_>::build_rgraph(PointIndex<Kind>& ptidx, Real radius, Graph& g, bool verbose)
{
    double elapsed;
    LocalTimer timer;
    timer.start_timer();

    size_t m = ptidx.build_rgraph(radius, g);

    timer.stop_timer();
    elapsed = timer.get_elapsed();

    if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] :: built %.3f-graph :: [indexer=%s,verts=%lu,edges=%lu,meandeg=%.3f]\n", elapsed, __func__, radius, ptidx.repr(), g.size(), m, (m+0.0)/(g.size()+0.0));
    return elapsed;
}
