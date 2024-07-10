#include <iostream>
#include <random>
#include <vector>
#include <unistd.h>
#include "pindex.h"
#include "ptraits.h"
#include "timer.h"
#include "misc.h"

using namespace std;

using Index = int64_t;
using PointTraits = SelectPoint<FPSIZE, PTDIM>::PointTraits;
using PIndex = PointIndex<PointTraits, Index>;
using Real = PointTraits::Real;
using Point = PointTraits::Point;
using PointVector = vector<Point>;
using IndexSetVector = vector<unordered_set<Index>>;

void read_options(int argc, char *argv[], char *&fname, double& cutoff, int& iters, double& damping, char *&oprefix);
void read_points_file(PointVector& points, const char *fname);
void build_rgraph(PIndex& ptidx, double radius, IndexSetVector& rgraph, int iter);
void write_rgraph_file(const IndexSetVector& rgraph, const char *oprefix, int iter);

int main(int argc, char *argv[])
{
    LocalTimer timer;
    timer.start_timer();

    timer.stop_timer();
    main_msg(argc, argv, timer.get_elapsed());
    return 0;
}
