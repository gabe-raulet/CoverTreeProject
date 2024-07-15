template <real_type Real_, int D> requires is_pos_int<D>
typename EuclideanTraits<Real_, D>::Real
EuclideanTraits<Real_, D>::Distance::operator()(const Point& p, const Point& q)
{
    if constexpr (D == 1) return p < q? q - p : p - q;
    else
    {
        Real sum = 0;
        for (int i = 0; i < D; ++i) sum += (p[i]-q[i])*(p[i]-q[i]);
        return ::sqrt(sum);
    }
}

template <real_type Real_, int D> requires is_pos_int<D>
void EuclideanTraits<Real_, D>::pack_point(PointRecord& record, const Point& p)
{
    static constexpr int dim = D;

    char *dim_dest = record.data();
    char *pt_dest = dim_dest + sizeof(dim);
    char *pt_src;

    if constexpr (D == 1) pt_src = (char*)&p;
    else pt_src = (char*)p.data();

    ::memcpy(dim_dest, &dim, sizeof(dim));
    ::memcpy(pt_dest, pt_src, sizeof(p));
}

template <real_type Real_, int D> requires is_pos_int<D>
void EuclideanTraits<Real_, D>::unpack_point(const PointRecord& record, Point& p)
{
    int dim;

    const char *dim_src = record.data();
    const char *pt_src = record.data() + sizeof(int);
    char *pt_dest;

    if constexpr (D == 1) pt_dest = (char*)&p;
    else pt_dest = (char*)p.data();

    ::memcpy(&dim, dim_src, sizeof(dim)); assert(dim == D);
    ::memcpy(pt_dest, pt_src, sizeof(p));
}

template <real_type Real_, int D> requires is_pos_int<D>
template <class Iter>
void EuclideanTraits<Real_, D>::read_from_file(Iter d_first, const char *fname)
{
    Point p;
    ifstream is;
    PointRecord record;
    size_t filesize, n;

    filesize = FileInfo(fname).get_file_size();
    is.open(fname, ios::binary | ios::in);

    assert(filesize % sizeof(record) == 0);
    n = filesize / sizeof(record);

    for (size_t i = 0; i < n; ++i)
    {
        is.read(record.data(), sizeof(record));
        unpack_point(record, p);
        *d_first++ = p;
    }

    is.close();
}

template <real_type Real_, int D> requires is_pos_int<D>
template <class Iter>
void EuclideanTraits<Real_, D>::write_to_file(Iter first, Iter last, const char *fname)
{
    ofstream os;
    PointRecord record;

    os.open(fname, ios::binary | ios::out);

    while (first != last)
    {
        pack_point(record, *first++);
        os.write(record.data(), sizeof(record));
    }

    os.close();
}

template <real_type Real_, int D> requires is_pos_int<D>
template <class RandomGen, class RandomDist>
void EuclideanTraits<Real_, D>::fill_random_point(Point& p, RandomGen& gen, RandomDist& dist)
{
    if constexpr (D == 1) p = dist(gen);
    else generate(p.begin(), p.end(), [&gen, &dist] () { return dist(gen); });
}

template <real_type Real_, int D> requires is_pos_int<D>
template <class Iter, class RandomGen, class RandomDist>
void EuclideanTraits<Real_, D>::fill_random_points(Iter first, Iter last, RandomGen& gen, RandomDist& dist)
{
    for_each(first, last, [&gen, &dist](Point& point) { fill_random_point(point, gen, dist); });
}

template <real_type Real_, int D> requires is_pos_int<D>
template <class Index>
double EuclideanTraits<Real_, D>::generate_random_gaussian(PointVector& points, Index size, Real var, int seed, bool verbose)
{
    double elapsed;
    LocalTimer timer;
    timer.start_timer();

    if (seed < 0)
    {
        random_device rd;
        default_random_engine tgen(rd());
        uniform_int_distribution tdist{0, numeric_limits<int>::max()};
        seed = tdist(tgen);
    }

    default_random_engine gen(seed*17);
    normal_distribution<Real> dist{0.0, ::sqrt(var)};

    points.resize(size);
    fill_random_points(points.begin(), points.end(), gen, dist);

    timer.stop_timer();
    elapsed = timer.get_elapsed();

    if (verbose) fprintf(stderr, "[time=%.3f,msg::%s] :: randomly generated %lu points :: [dim=%d,fp=%lu,var=%.2f,seed=%d]\n", elapsed, __func__, (size_t)size, D, sizeof(Real)<<3, var, seed);

    return elapsed;
}

template <real_type Real_, int D> requires is_pos_int<D>
string EuclideanTraits<Real_, D>::repr(const Point& p, int precision)
{
    stringstream ss;
    ss << fixed << showpoint << showpos << setprecision(precision);

    if constexpr (D == 1) ss << p;
    else
    {
        ss << "[";
        copy(p.begin(), p.end()-1, ostream_iterator<Real>(ss, ","));
        ss << p.back() << "]";
    }

    return ss.str();
}

template <real_type Real_, int D> requires is_pos_int<D>
size_t EuclideanTraits<Real_, D>::PointHash::operator()(const Point& p) const noexcept
{
    if constexpr (same_as<Real_, Point>) return hash<Real_>{}(p);
    else
    {
        size_t h = 0;
        for (const Real_& e : p) h ^= hash<Real_>{}(e) + (h << 6) + (h >> 2);
        return h;
    }
}
