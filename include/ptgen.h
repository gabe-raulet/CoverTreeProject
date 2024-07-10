#ifndef POINT_GENERATOR_H_
#define POINT_GENERATOR_H_

#include <vector>
#include <random>
#include <algorithm>
#include "mpienv.h"
#include "misc.h"

using namespace std;

template <class PointTraits, class Index = int64_t>
class PointGenerator
{
    public:

        using Point = typename PointTraits::Point;
        using Real = typename PointTraits::Real;

        PointGenerator();
        PointGenerator(int seed);

        void generate_points(vector<Point>& points, Index totsize, double var);

    private:

        int seed;

        template <class Iter>
        requires same_as<typename Iter::value_type, typename PointTraits::Point>
        void generate_local_points(Iter first, Iter last, int local_seed, double var);
};

#include "ptgen.hpp"

#endif
