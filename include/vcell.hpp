template <class PointTraits, class Index>
VoronoiCell<PointTraits, Index>::VoronoiCell()
    : max_radius(0.),
      last_radius(0.) {}

template <class PointTraits, class Index>
VoronoiCell<PointTraits, Index>::VoronoiCell(const PointVector& points)
    : VoronoiCell(points, 0) {}

template <class PointTraits, class Index>
VoronoiCell<PointTraits, Index>::VoronoiCell(const PointVector& points, Index seed)
    : max_radius(0.),
      last_radius(0.),
      points(points),
      dists(size(), numeric_limits<Real>::max()),
      closest(size(), seed),
      cover({seed}) {}

template <class PointTraits, class Index>
void VoronoiCell<PointTraits, Index>::build(Real base)
{
    auto distance = PointTraits::distance();

    for (Index i = 0; i < size(); ++i)
    {
        Point tail = points[cover.back()];

        for (Index j = 0; j < size(); ++j)
        {
            Real lastdist = dists[j];
            Real curdist = distance(points[j], tail);

            if (curdist <= lastdist)
            {
                dists[j] = curdist;
                closest[j] = j != cover.back()? cover.back() : -1;
            }
        }

        Index next = ::distance(dists.begin(), max_element(dists.begin(), dists.end()));

        last_radius = dists[next];
        max_radius = ::max(max_radius, last_radius);

        if (dists[next] <= (max_radius / base))
            break;

        cover.push_back(next);
    }
}

template <class PointTraits, class Index>
void VoronoiCell<PointTraits, Index>::assign(const PointVector& _points, Index seed)
{
    points.assign(_points.begin(), _points.end());
    dists.assign(size(), numeric_limits<Real>::max());
    closest.assign(size(), seed);
    cover.assign({seed});
    max_radius = last_radius = 0.;
}

template <class PointTraits, class Index>
Index VoronoiCell<PointTraits, Index>::get_cover_ids(IndexVector& ids) const
{
    ids.assign(cover.begin(), cover.end());
    return ids.size();
}

template <class PointTraits, class Index>
Index VoronoiCell<PointTraits, Index>::get_cover_pts(PointVector& pts) const
{
    pts.clear();
    for (Index i : cover) pts.push_back(points[i]);
    return pts.size();
}

template <class PointTraits, class Index>
Index VoronoiCell<PointTraits, Index>::get_cover_cell_ids(IndexVectorVector& cell_ids) const
{
    unordered_map<Index, Index> idmap;
    cell_ids.clear();

    for (Index seed : cover)
    {
        idmap.insert({seed, idmap.size()});
        cell_ids.push_back({seed});
    }

    for (Index i = 0; i < size(); ++i)
    {
        Index cell = closest[i];

        if (cell >= 0)
        {
            Index loc = idmap.find(cell)->second;
            cell_ids[loc].push_back(i);
        }
    }

    return cell_ids.size();
}

template <class PointTraits, class Index>
typename PointTraits::Real VoronoiCell<PointTraits, Index>::get_coverage_radius() const
{
    /*
     * The coverage radius is the minimal r such that the cover points are an r-net for
     * the total set of points.
     *
     * Go through each point, calculate its minimum distance to a cover point, and keep
     * a running maximum of each of these values.
     */

    auto distance = PointTraits::distance();

    PointVector cover_pts;
    RealVector cover_dists;

    Index m = get_cover_pts(cover_pts);
    cover_dists.resize(m);

    Real coverage_radius = 0.;

    for (Point p : points)
    {
        transform(cover_pts.begin(), cover_pts.end(), cover_dists.begin(), [&](const Point& q) { return distance(p, q); });
        coverage_radius = ::max(coverage_radius, *min_element(cover_dists.begin(), cover_dists.end()));
    }

    return coverage_radius;
}

template <class PointTraits, class Index>
typename PointTraits::Real VoronoiCell<PointTraits, Index>::get_separation() const
{
    auto distance = PointTraits::distance();

    PointVector cover_pts;
    Index m = get_cover_pts(cover_pts);

    Real separation = numeric_limits<Real>::max();

    for (Index i = 0; i < m; ++i)
        for (Index j = 0; j < i; ++j)
            separation = ::min(separation, distance(cover_pts[i], cover_pts[j]));

    return separation;
}

template <class PointTraits, class Index>
bool VoronoiCell<PointTraits, Index>::verify_correctness(const PointVector& points, bool verbose)
{
    for (Index seed = 0; seed < points.size(); ++seed)
    {
        for (Real base = 3.0; base > 1.0; base -= 0.25)
        {
            VoronoiCell cell(points, seed);
            cell.build(base);

            Real max_radius = cell.get_max_radius();
            Real coverage_radius = cell.get_coverage_radius();
            Real separation = cell.get_separation();

            if (verbose)
            {
                fprintf(stderr, "[pts=%lld,cover_pts=%lld,ratio=%.3f,maxrad=%.3f,covrad=%.3f,sep=%.3f,covratio=%.3f,sepratio=%.3f]\n",
                        cell.size(), cell.coversize(), 1./base, max_radius, coverage_radius, separation, coverage_radius/max_radius, separation/max_radius);
            }

            if (coverage_radius > separation)
                return false;
        }
    }

    return true;
}
