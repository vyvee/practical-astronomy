#ifndef ELP82JM_H_
#define ELP82JM_H_

// - Algorithms from [Jean99], based on ELP-2000/82 lunar theory
//   - Accuracy: Approx. 10" in longitude, 4" in latitude

#include "angle.h"
#include "date.h"
#include "utils.h"

using namespace PA;

namespace PA
{
class ELP82JM
{
   public:
    constexpr ELP82JM(double jd) noexcept : julian_date_(jd){};
    void GetPosition() noexcept
    {
        ComputePosition();
        std::cout << " lon.: " << RadToDegStr(longitude_, 8) << std::endl;
        std::cout << " lat.: " << RadToDegStr(latitude_, 8) << std::endl;
        std::cout << "dist.: " << distance_km_ << std::endl;
    };

   private:
    double julian_date_;

    constexpr void ComputePosition() noexcept;
    bool position_is_valid_{false};
    double longitude_{0.0};
    double latitude_{0.0};
    double distance_km_{0.0};
};

constexpr void ELP82JM::ComputePosition() noexcept
{
    // References:
    // - [Jean99] Chapter 47, pp.337-344
    double t{(julian_date_ - EpochJ2000) / 36525.0};
    double lp{horner_polynomial({218.3164477, 481267.88123421, -0.0015786,
                                 1.0 / 538841.0, -1.0 / 65194000},
                                t)};
    double d{horner_polynomial({297.8501921, 445267.1114034, -0.0018819,
                                1.0 / 545868.0, -1.0 / 113065000.0},
                               t)};
    double m{horner_polynomial(
        {357.5291092, 35999.0502909, -0.0001536, 1.0 / 24490000.0}, t)};
    double mp{horner_polynomial({134.9633964, 477198.8675055, 0.0087414,
                                 1.0 / 69699.0, -1.0 / 14712000.0},
                                t)};
    double f{horner_polynomial({93.2720950, 483202.0175233, -0.0036539,
                                -1.0 / 3526000.0, 1.0 / 863310000.0},
                               t)};
    double a1{119.75 + 131.849 * t};
    double a2{53.09 + 479264.290 * t};
    double a3{313.45 + 481266.484 * t};
    double e{1 + (-0.002516 - 0.0000074 * t) * t};
    double e2{e * e};
    double es_internal[]{e2, e, 1.0, e, e2};
    double* es{es_internal + 2};
    struct {
        int d;
        int m;
        int mp;
        int f;
        double l;
        double r;
    } periodic_terms_lr[]{
        {0, 0, 1, 0, 6288774, -20905355},
        {2, 0, -1, 0, 1274027, -3699111},
        {2, 0, 0, 0, 658314, -2955968},
        {0, 0, 2, 0, 213618, -569925},
        {0, 1, 0, 0, -185116, 48888},
        {0, 0, 0, 2, -114332, -3149},
        {2, 0, -2, 0, 58793, 246158},
        {2, -1, -1, 0, 57066, -152138},
        {2, 0, 1, 0, 53322, -170733},
        {2, -1, 0, 0, 45758, -204586},
        {0, 1, -1, 0, -40923, -129620},
        {1, 0, 0, 0, -34720, 108743},
        {0, 1, 1, 0, -30383, 104755},
        {2, 0, 0, -2, 15327, 10321},
        {0, 0, 1, 2, -12528, 0},
        {0, 0, 1, -2, 10980, 79661},
        {4, 0, -1, 0, 10675, -34782},
        {0, 0, 3, 0, 10034, -23210},
        {4, 0, -2, 0, 8548, -21636},
        {2, 1, -1, 0, -7888, 24208},
        {2, 1, 0, 0, -6766, 30824},
        {1, 0, -1, 0, -5163, -8379},
        {1, 1, 0, 0, 4987, -16675},
        {2, -1, 1, 0, 4036, -12831},
        {2, 0, 2, 0, 3994, -10445},
        {4, 0, 0, 0, 3861, -11650},
        {2, 0, -3, 0, 3665, 14403},
        {0, 1, -2, 0, -2689, -7003},
        {2, 0, -1, 2, -2602, 0},
        {2, -1, -2, 0, 2390, 10056},
        {1, 0, 1, 0, -2348, 6322},
        {2, -2, 0, 0, 2236, -9884},
        {0, 1, 2, 0, -2120, 5751},
        {0, 2, 0, 0, -2069, 0},
        {2, -2, -1, 0, 2048, -4950},
        {2, 0, 1, -2, -1773, 4130},
        {2, 0, 0, 2, -1595, 0},
        {4, -1, -1, 0, 1215, -3958},
        {0, 0, 2, 2, -1110, 0},
        {3, 0, -1, 0, -892, 3258},
        {2, 1, 1, 0, -810, 2616},
        {4, -1, -2, 0, 759, -1897},
        {0, 2, -1, 0, -713, -2117},
        {2, 2, -1, 0, -700, 2354},
        {2, 1, -2, 0, 691, 0},
        {2, -1, 0, -2, 596, 0},
        {4, 0, 1, 0, 549, -1423},
        {0, 0, 4, 0, 537, -1117},
        {4, -1, 0, 0, 520, -1571},
        {1, 0, -2, 0, -487, -1739},
        {2, 1, 0, -2, -399, 0},
        {0, 0, 2, -2, -381, -4421},
        {1, 1, 1, 0, 351, 0},
        {3, 0, -2, 0, -340, 0},
        {4, 0, -3, 0, 330, 0},
        {2, -1, 2, 0, 327, 0},
        {0, 2, 1, 0, -323, 1165},
        {1, 1, -1, 0, 299, 0},
        {2, 0, 3, 0, 294, 0},
        {2, 0, -1, -2, 0, 8752},
    };
    struct {
        int d;
        int m;
        int mp;
        int f;
        double b;
    } periodic_terms_b[]{
        {0, 0, 0, 1, 5128122}, {0, 0, 1, 1, 280602},  {0, 0, 1, -1, 277693},
        {2, 0, 0, -1, 173237}, {2, 0, -1, 1, 55413},  {2, 0, -1, -1, 46271},
        {2, 0, 0, 1, 32573},   {0, 0, 2, 1, 17198},   {2, 0, 1, -1, 9266},
        {0, 0, 2, -1, 8822},   {2, -1, 0, -1, 8216},  {2, 0, -2, -1, 4324},
        {2, 0, 1, 1, 4200},    {2, 1, 0, -1, -3359},  {2, -1, -1, 1, 2463},
        {2, -1, 0, 1, 2211},   {2, -1, -1, -1, 2065}, {0, 1, -1, -1, -1870},
        {4, 0, -1, -1, 1828},  {0, 1, 0, 1, -1794},   {0, 0, 0, 3, -1749},
        {0, 1, -1, 1, -1565},  {1, 0, 0, 1, -1491},   {0, 1, 1, 1, -1475},
        {0, 1, 1, -1, -1410},  {0, 1, 0, -1, -1344},  {1, 0, 0, -1, -1335},
        {0, 0, 3, 1, 1107},    {4, 0, 0, -1, 1021},   {4, 0, -1, 1, 833},
        {0, 0, 1, -3, 777},    {4, 0, -2, 1, 671},    {2, 0, 0, -3, 607},
        {2, 0, 2, -1, 596},    {2, -1, 1, -1, 491},   {2, 0, -2, 1, -451},
        {0, 0, 3, -1, 439},    {2, 0, 2, 1, 422},     {2, 0, -3, -1, 421},
        {2, 1, -1, 1, -366},   {2, 1, 0, 1, -351},    {4, 0, 0, 1, 331},
        {2, -1, 1, 1, 315},    {2, -2, 0, -1, 302},   {0, 0, 1, 3, -283},
        {2, 1, 1, -1, -229},   {1, 1, 0, -1, 223},    {1, 1, 0, 1, 223},
        {0, 1, -2, -1, -220},  {2, 1, -1, -1, -220},  {1, 0, 1, 1, -185},
        {2, -1, -2, -1, 181},  {0, 1, 2, 1, -177},    {4, 0, -2, -1, 176},
        {4, -1, -1, -1, 166},  {1, 0, 1, -1, -164},   {4, 0, 1, -1, 132},
        {1, 0, -1, -1, -119},  {4, -1, 0, -1, 115},   {2, -2, 0, 1, 107},
    };

    double sum_l{3958 * std::sin(DegToRad(a1)) +
                 1962 * std::sin(DegToRad(lp - f)) +
                 318 * std::sin(DegToRad(a2))};
    double sum_r{385000560.0};
    for(auto& pt : periodic_terms_lr) {
        double arg{DegToRad(pt.d * d + pt.m * m + pt.mp * mp + pt.f * f)};
        sum_l += es[pt.m] * pt.l * std::sin(arg);
        sum_r += es[pt.m] * pt.r * std::cos(arg);
    }
    double sum_b{
        -2235 * std::sin(DegToRad(lp)) + 382 * std::sin(DegToRad(a3)) +
        175 * std::sin(DegToRad(a1 - f)) + 175 * std::sin(DegToRad(a1 + f)) +
        127 * std::sin(DegToRad(lp - mp)) - 115 * std::sin(DegToRad(lp + mp))};
    for(auto& pt : periodic_terms_b) {
        double arg{DegToRad(pt.d * d + pt.m * m + pt.mp * mp + pt.f * f)};
        sum_b += es[pt.m] * pt.b * std::sin(arg);
    }
    longitude_ = RadUnwind(DegToRad(lp + sum_l / 1000000.0));
    latitude_ = RadNormalize(DegToRad(sum_b / 1000000.0));
    distance_km_ = sum_r / 1000.0;
#if 0
    std::cout << std::fixed << std::setprecision(8);
    std::cout << " t: " << t << std::endl;
    std::cout << "lp: " << Degree(lp).GetUnwind().DegStr(8) << std::endl;
    std::cout << " d: " << Degree(d).GetUnwind().DegStr(8) << std::endl;
    std::cout << " m: " << Degree(m).GetUnwind().DegStr(8) << std::endl;
    std::cout << "mp: " << Degree(mp).GetUnwind().DegStr(8) << std::endl;
    std::cout << " f: " << Degree(f).GetUnwind().DegStr(8) << std::endl;
    std::cout << "a1: " << Degree(a1).GetUnwind().DegStr(8) << std::endl;
    std::cout << "a2: " << Degree(a2).GetUnwind().DegStr(8) << std::endl;
    std::cout << "a3: " << Degree(a3).GetUnwind().DegStr(8) << std::endl;
    std::cout << " e: " << e << std::endl;
    std::cout << " sum_l: " << sum_l << std::endl;
    std::cout << " sum_b: " << sum_b << std::endl;
    std::cout << " sum_r: " << sum_r << std::endl;
    std::cout << " lon.: " << longitude_.DegStr(8) << std::endl;
    std::cout << " lat.: " << latitude_.DegStr(8) << std::endl;
    std::cout << "dist.: " << distance_km_ << std::endl;
#endif
}

}  // namespace PA

#endif  // ELP82JM_H_
