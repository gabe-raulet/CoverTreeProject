template <float_type R, int D> requires is_pos_int<D>
R Traits<R, D>::Distance::operator()(const Point& p, const Point& q)
{
    if constexpr (D == 1) return p < q? q - p : p - q;
    else
    {
        Real sum = 0;
        for (int i = 0; i < D; ++i) sum += (p[i] - q[i])*(p[i] - q[i]);
        return ::sqrt(sum);
    }
}

template <float_type R, int D> requires is_pos_int<D>
void Traits<R, D>::pack_point(PointRecord& record, const Point& p)
{
    constexpr int dim = D;

    char *dim_dest = record.data();
    char *pt_dest = dim_dest + sizeof(dim);
    char *pt_src;

    if constexpr (D == 1) pt_src = (char*)&p;
    else pt_src = (char*)p.data();

    ::memcpy(dim_dest, &dim, sizeof(dim));
    ::memcpy(pt_dest, pt_src, sizeof(p));
}

template <float_type R, int D> requires is_pos_int<D>
void Traits<R, D>::unpack_point(const PointRecord& record, Point& p)
{
    int dim;

    const char *dim_src = record.data();
    const char *pt_src = record.data() + sizeof(int);
    char *pt_dest;

    if constexpr (D == 1) pt_dest = (char*)&p;
    else pt_dest = (char*)p.data();

    ::memcpy(&dim, dim_src, sizeof(dim));
    assert(dim == D);
    ::memcpy(pt_dest, pt_src, sizeof(p));
}

template <float_type R, int D> requires is_pos_int<D>
template <class Iter>
void Traits<R, D>::write_to_file(Iter first, Iter last, const char *fname)
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

template <float_type R, int D> requires is_pos_int<D>
template <class Iter>
void Traits<R, D>::read_from_file(Iter d_first, const char *fname)
{
    Point p;
    ifstream is;
    PointRecord record;
    size_t filesize, n;

    filesize = get_file_size(fname);
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

template <float_type R, int D> requires is_pos_int<D>
template <class RandomGen, class RandomDist>
void Traits<R, D>::fill_random_point(Point& point, RandomGen& gen, RandomDist& dist)
{
    if constexpr (D == 1) point = dist(gen);
    else generate(point.begin(), point.end(), [&gen, &dist] () { return dist(gen); });
}

template <float_type R, int D> requires is_pos_int<D>
template <class Iter, class RandomGen, class RandomDist>
void Traits<R, D>::fill_random_points(Iter first, Iter last, RandomGen& gen, RandomDist& dist)
{
    for_each(first, last, [&gen, &dist](Point& point) { fill_random_point(point, gen, dist); });
}

template <float_type R, int D> requires is_pos_int<D>
string Traits<R, D>::info()
{
    stringstream ss;
    ss << "[dim=" << D << ",real=" << sizeof(R)*8 << "]";
    return ss.str();
}

template <float_type R, int D> requires is_pos_int<D>
string Traits<R, D>::repr(const Point& point, int precision)
{
    stringstream ss;
    ss << fixed << showpoint << showpos << setprecision(precision);

    if constexpr (D == 1) ss << point;
    else
    {
        ss << "[";
        copy(point.begin(), point.end()-1, ostream_iterator<Real>(ss, ","));
        ss << point.back() << "]";
    }

    return ss.str();
}

template <float_type R, int D> requires is_pos_int<D>
size_t Traits<R, D>::PointHash::operator()(const Point& p) const noexcept
{
    if constexpr (same_as<R, Point>) return hash<R>{}(p);
    else
    {
        size_t h = 0;
        for (const R& e : p) h ^= hash<R>{}(e) + (h << 6) + (h >> 2);
        return h;
    }
}
