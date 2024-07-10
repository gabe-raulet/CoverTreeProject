#include <iostream>
#include <random>
#include <vector>
#include <unistd.h>
#include "ptgen.h"
#include "ptraits.h"
#include "timer.h"
#include "misc.h"

using namespace std;

using Index = int64_t;
using PointTraits = SelectPoint<FPSIZE, PTDIM>::PointTraits;
using Real = PointTraits::Real;
using Point = PointTraits::Point;

void read_options(int argc, char *argv[], Index& n, char *&fname, double& var, int& seed);
void generate_points(vector<Point>& points, Index totsize, double var, int seed);
void write_points_file(const vector<Point>& points, const char *fname);

int main(int argc, char *argv[])
{
    Index n;
    char *fname;
    double var = 10.;
    int seed = -1;
    vector<Point> points;

    LocalTimer timer;
    timer.start_timer();

    read_options(argc, argv, n, fname, var, seed);
    generate_points(points, n, var, seed);
    write_points_file(points, fname);

    timer.stop_timer();

    return 0;
}

void read_options(int argc, char *argv[], Index& n, char *&fname, double& var, int& seed)
{
    LocalTimer timer;
    timer.start_timer();

    auto usage = [&argv, &var, &seed] (int err)
    {
        fprintf(stderr, "Usage: %s [options] <numpts> <fname>\n", argv[0]);
        fprintf(stderr, "Options: -V FLOAT variance [%.2f]\n", var);
        fprintf(stderr, "         -s INT   random seed [%s]\n", seed < 0? "random" : to_string(seed).c_str());
        fprintf(stderr, "         -h       help message\n");
        exit(err);
    };

    int c;
    while ((c = getopt(argc, argv, "V:s:h")) >= 0)
    {
        if      (c == 'V') var = atof(optarg);
        else if (c == 's') seed = atoi(optarg);
        else if (c == 'h') usage(0);
    }

    if (argc - optind < 2)
    {
        fprintf(stderr, "[err::%s] missing argument(s)\n", __func__);
        usage(1);
    }

    n = read_integer<Index>(argv[optind++]);
    fname = argv[optind];

    timer.stop_timer();

    fprintf(stderr, "[time=%.3f,msg::%s] :: [n=%lu,fname='%s',var=%.2f,seed=%d]\n", timer.get_elapsed(), __func__, static_cast<size_t>(n), fname, var, seed);
}

void generate_points(vector<Point>& points, Index totsize, double var, int seed)
{

}

void write_points_file(const vector<Point>& points, const char *fname)
{

}
