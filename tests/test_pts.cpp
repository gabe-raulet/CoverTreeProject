#include <iostream>
#include "ptraits.h"
#include "ptutils.h"

using namespace std;

template <class R, int D>
void test();

int main(int argc, char *argv[])
{
    test<float, 1>();
    test<float, 2>();
    test<float, 4>();
    test<float, 7>();
    test<float, 8>();
    test<float, 16>();
    test<float, 23>();
    test<float, 32>();
    test<float, 64>();
    test<float, 128>();

    test<double, 1>();
    test<double, 2>();
    test<double, 4>();
    test<double, 8>();
    test<double, 16>();
    test<double, 23>();
    test<double, 32>();
    test<double, 64>();
    test<double, 128>();

    return 0;
}

template <class R, int D>
void test()
{
    using PointTraits = EuclideanTraits<R, D>;
    using PointUtils = PointUtils<PointTraits>;

    using Real = typename PointTraits::Real;
    using Point = typename PointTraits::Point;

    using RealVector = typename PointTraits::RealVector;
    using PointVector = typename PointTraits::PointVector;
    using PointSet = typename PointTraits::PointSet;

    int seed;
    random_device rd;
    PointVector v1, v2;
    PointSet s1, s2;

    seed = rd();
    seed = seed < 0? -seed : seed;

    PointTraits::generate_random_gaussian(v1, 1000, 10.0, seed, true);
    PointTraits::generate_random_gaussian(v2, 1000, 10.0, seed, true);

    assert((PointUtils::compare_points(v1, v2)));

    copy(v2.begin(), v2.end(), inserter(s1, s1.end()));

    assert((PointUtils::compare_points(v1, s1)));
    assert((PointUtils::compare_points(s1, v1)));

    {
        default_random_engine gen(seed);
        shuffle(v1.begin(), v1.end(), gen);

        assert(!(PointUtils::compare_points(v1, v2)));
        assert((PointUtils::compare_points(v1, s1)));
        assert(!(PointUtils::compare_points(v1, s2)));

        s2 = s1;
        assert((PointUtils::compare_points(v1, s2)));
        assert((PointUtils::compare_points(v1, v1)));
    }

    {
        default_random_engine gen(seed);
        shuffle(v2.begin(), v2.end(), gen);

        assert((PointUtils::compare_points(v1, v2)));
        assert((PointUtils::compare_points(v1, s1)));
        assert((PointUtils::compare_points(v1, s2)));
        assert((PointUtils::compare_points(v2, s1)));
        assert((PointUtils::compare_points(v2, s2)));
    }

    assert((PointUtils::compare_points(s1, v1)));

    auto pt = v1.back(); v1.pop_back();
    assert(!(PointUtils::compare_points(s1, v1)));

    auto item = s1.extract(pt);
    assert((PointUtils::compare_points(s1, v1)));

    PointUtils::write_points_file(s1, "s1", true);
    PointUtils::write_points_file(s2, "s2", true);

    PointUtils::read_points_file(s2, "s1", true);
    PointUtils::read_points_file(s1, "s2", true);

    assert((PointUtils::compare_points(s1, v2)));
    assert((PointUtils::compare_points(s2, v1)));

    PointTraits::generate_random_gaussian(v1, 100000, 10.0, -1, true);

    PointUtils::write_points_file(v1, "v1", true);
    PointUtils::read_points_file(v2, "v1", true);
    PointUtils::read_points_file(s1, "v1", true);
    PointUtils::read_points_file(s2, "v1", true);

    assert((PointUtils::compare_points(v2, v1)));
    assert((PointUtils::compare_points(s1, v1)));
    assert((PointUtils::compare_points(s2, v1)));
    assert((PointUtils::compare_points(s2, s1)));
}
