#ifndef POINT_TRAITS_H_
#define POINT_TRAITS_H_

#include <fstream>
#include <sstream>
#include <assert.h>
#include "misc.h"
#include "fileinfo.h"

using namespace std;

template <class R>
concept float_type = same_as<R, float> || same_as<R, double>;

template <int D>
concept is_pos_int = (D >= 1);

template <class R, int D>
struct point_type { using Point = array<R, D>; };

template <class R>
struct point_type<R, 1> { using Point = R; };

template <float_type R, int D> requires is_pos_int<D>
struct Traits
{
    using Real = R;
    using Point = typename point_type<Real, D>::Point;
    using PointRecord = array<char, sizeof(int) + sizeof(Point)>;

    struct Distance { R operator()(const Point& p, const Point& q); };
    static Distance distance() { return Distance(); }
    static consteval int dimension() { return D; }

    static void pack_point(PointRecord& record, const Point& p);
    static void unpack_point(const PointRecord& record, Point& p);

    template <class Iter>
    static void read_from_file(Iter d_first, const char *fname);

    template <class Iter>
    static void write_to_file(Iter first, Iter last, const char *fname);

    template <class RandomGen, class RandomDist>
    static void fill_random_point(Point& point, RandomGen& gen, RandomDist& dist);

    template <class Iter, class RandomGen, class RandomDist>
    static void fill_random_points(Iter first, Iter last, RandomGen& gen, RandomDist& dist);

    static string info();
    static string repr(const Point& point, int precision=3);

    struct PointHash { size_t operator()(const Point& p) const noexcept; };
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
    using PointTraits = Traits<typename select_real<FP>::Real, select_dim<DIM>::dim()>;
};

#include "ptraits.hpp"

#endif
