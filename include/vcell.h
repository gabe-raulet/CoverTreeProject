#ifndef VORONOI_CELL_H_
#define VORONOI_CELL_H_

#include <vector>

using namespace std;

template <class PointTraits, class Index>
class VoronoiCell
{
    public:

        using Real = typename PointTraits::Real;
        using Point = typename PointTraits::Point;

        using IndexVector = vector<Index>;
        using PointVector = vector<Point>;
        using RealVector = vector<Real>;

        using IndexVectorVector = vector<IndexVector>;
        using PointVectorVector = vector<PointVector>;

        VoronoiCell();
        VoronoiCell(const PointVector& points);
        VoronoiCell(const PointVector& points, Index seed);

        void build(Real base);
        void assign(const PointVector& points, Index seed = 0);

        Index size() const { return points.size(); }
        Index coversize() const { return cover.size(); }

        Index get_cover_ids(IndexVector& ids) const;
        Index get_cover_pts(PointVector& pts) const;
        Index get_cover_cell_ids(IndexVectorVector& cell_ids) const;

        Real get_max_radius() const { return max_radius; }
        Real get_coverage_radius() const;
        Real get_separation() const;

        static bool verify_correctness(const PointVector& points, bool verbose = false);

        bool is_leaf() const { return (last_radius == 0.); }

    private:

        Real max_radius, last_radius;
        PointVector points;
        RealVector dists;
        IndexVector closest, cover;
};

#include "vcell.hpp"

#endif
