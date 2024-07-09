#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <stdio.h>
#include "ptraits.h"
#include "vcell.h"
#include "timer.h"

using namespace std;

using Index = int64_t;
using PointTraits = SelectPoint<FPSIZE, PTDIM>::PointTraits;

using Real = PointTraits::Real;
using Point = PointTraits::Point;
using VCell = VoronoiCell<PointTraits, Index>;

int main(int argc, char *argv[])
{
    Real var = 10.;
    int seed = -1;
    vector<Point> points;

    random_device rd;
    default_random_engine gen(seed < 0? rd() : (seed+3)*351);
    normal_distribution<Real> dist{0.0, sqrt(var)};

    vector<Index> counts = {0, 1, 2, 5, 32, 512, 1111};

    for (Index n : counts)
    {
        points.resize(n);
        PointTraits::fill_random_points(points.begin(), points.end(), gen, dist);
        assert((VCell::verify_correctness(points, true)));
    }

    VCell cell1;
    VCell cell2(points);
    VCell cell3(points, points.size()-1);

    points.resize(50);
    PointTraits::fill_random_points(points.begin(), points.end(), gen, dist);

    VCell cell(points); cell.build(2.0);

    vector<Index> cover_ids;
    vector<vector<Index>> cell_ids;

    cell.get_cover_ids(cover_ids);
    cell.get_cover_cell_ids(cell_ids);

    copy(cover_ids.begin(), cover_ids.end(), ostream_iterator<Index>(cout, ","));
    cout << '\n' << endl;

    for (auto& v : cell_ids)
    {
        copy(v.begin(), v.end(), ostream_iterator<Index>(cout, ","));
        cout << endl;
    }

    return 0;
}
