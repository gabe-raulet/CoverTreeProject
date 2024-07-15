#ifndef POINT_TRAITS_H_
#define POINT_TRAITS_H_

#include <fstream>
#include <sstream>
#include <unordered_set>
#include <random>
#include "fileinfo.h"
#include "timer.h"

using namespace std;

/*
 * Euclidean metric space.
 */

template <class Real>
concept real_type = same_as<Real, float> || same_as<Real, double>;

template <int D>
concept is_pos_int = (D >= 1);

template <real_type Real, int D> requires is_pos_int<D>
struct real_point_type
{
    using Point = array<Real, D>;
};

template <real_type Real>
struct real_point_type<Real, 1>
{
    using Point = Real;
};

template <real_type Real_, int D> requires is_pos_int<D>
struct EuclideanTraits
{
    using Real = Real_;

    using Point = typename real_point_type<Real, D>::Point;
    using PointRecord = array<char, sizeof(int) + sizeof(Point)>;

    using RealVector = vector<Real>;
    using PointVector = vector<Point>;

    struct Distance
    {
        Real operator()(const Point& p, const Point& q);
    };

    static Distance distance() { return Distance(); }
    static consteval int dimension() { return D; }

    static void pack_point(PointRecord& record, const Point& p);
    static void unpack_point(const PointRecord& record, Point& p);

    template <class Iter>
    static void read_from_file(Iter d_first, const char *fname);

    template <class Iter>
    static void write_to_file(Iter first, Iter last, const char *fname);

    template <class RandomGen, class RandomDist>
    static void fill_random_point(Point& p, RandomGen& gen, RandomDist& dist);

    template <class Iter, class RandomGen, class RandomDist>
    static void fill_random_points(Iter first, Iter last, RandomGen& gen, RandomDist& dist);

    template <class Index>
    static double generate_random_gaussian(PointVector& points, Index size, Real var, int seed, bool verbose = false);

    static string repr(const Point& p, int precision=3);
    struct PointHash { size_t operator()(const Point& p) const noexcept; };

    using PointSet = unordered_set<Point, PointHash>;
};

template <int FP>
struct select_real { using Real = float; };

template <>
struct select_real<64> { using Real = double; };

template <int DIM>
struct select_dim;

template <int DIM> requires (DIM >= 1)
struct select_dim<DIM>
{
    static consteval int dim() { return DIM; }
};

template <int DIM> requires (DIM <= 0)
struct select_dim<DIM>
{
    static consteval int dim() { return 2; }
};

template <int FP, int DIM>
struct SelectPoint
{
    using PointTraits = EuclideanTraits<typename select_real<FP>::Real, select_dim<DIM>::dim()>;
};

#include "ptraits.hpp"

#endif
