template <class PointTraits, class Index>
PointGenerator<PointTraits, Index>::PointGenerator()
    : PointGenerator(-1, Comm::comm_self()) {}

template <class PointTraits, class Index>
PointGenerator<PointTraits, Index>::PointGenerator(int seed)
    : PointGenerator(seed, Comm::comm_self()) {}

template <class PointTraits, class Index>
PointGenerator<PointTraits, Index>::PointGenerator(int seed, Comm comm)
    : seed(seed), comm(comm) { /* TODO: distribute */ assert(comm == Comm::comm_self()); }

template <class PointTraits, class Index>
template <class Iter> requires same_as<typename Iter::value, typename PointTraits::Point>
void PointGenerator<PointTraits, Index>::generate_local_points(Iter first, Iter last, int local_seed, double var)
{
    random_device rd;
    default_random_engine gen(local_seed < 0? rd() : (local_seed+3)*17);
    normal_distribution<Real> dist{0.0, static_cast<Real>(var)};
    PointTraits::fill_random_points(first, last, gen, dist);
}

template <class PointTraits, class Index>
void PointGenerator<PointTraits, Index>::generate_points(vector<Point>& points, Index totsize, double var)
{
    /* TODO: distribute */
    points.resize(totsize);
    generate_local_points(points.begin(), points.end(), var);
}

