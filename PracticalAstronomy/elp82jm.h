#ifndef ELP82JM_H_
#define ELP82JM_H_

#include "radian.h"
#include "utils.h"

using namespace PA;

// - Algorithms from [Jean99], based on ELP-2000/82 lunar theory
//   - Accuracy: Approx. 10" in longitude, 4" in latitude
//   - Moon moves about 0.5" in longitude per second, so this corresponds to
//     about 20 seconds

namespace PA {
class ELP82JM {
 public:
  static constexpr void Compute(double tt, double* p_longitude,
                                double* p_latitude,
                                double* p_radius_vector_km) noexcept;

 private:
  constexpr ELP82JM() noexcept {};
};

constexpr void ELP82JM::Compute(double tt, double* p_longitude,
                                double* p_latitude,
                                double* p_radius_vector_km) noexcept {
  // References:
  // - [Jean99] Chapter 47, pp.337-344
  double t{(tt - EpochJ2000) / 36525.0};
  // Moon's mean longitude, referred to the mean equinox of the date, and
  // including the constant term of the effect of the light-time (-0.70")
  double lp{
      horner_polynomial({218.3164477_deg, 481267.88123421_deg, -0.0015786_deg,
                         1.0_deg / 538841.0, -1.0_deg / 65194000},
                        t)};
  // Mean elongation of the Moon
  double d{
      horner_polynomial({297.8501921_deg, 445267.1114034_deg, -0.0018819_deg,
                         1.0_deg / 545868.0, -1.0_deg / 113065000.0},
                        t)};
  // Sun's mean anomaly
  double m{horner_polynomial({357.5291092_deg, 35999.0502909_deg,
                              -0.0001536_deg, 1.0_deg / 24490000.0},
                             t)};
  // Moon's mean anomaly
  double mp{
      horner_polynomial({134.9633964_deg, 477198.8675055_deg, 0.0087414_deg,
                         1.0_deg / 69699.0, -1.0_deg / 14712000.0},
                        t)};
  // Moon's argument of latitude (mean distance of the Moon from its ascending
  // node)
  double f{
      horner_polynomial({93.2720950_deg, 483202.0175233_deg, -0.0036539_deg,
                         -1.0_deg / 3526000.0, 1.0_deg / 863310000.0},
                        t)};
  double a1{119.75_deg + 131.849_deg * t};
  double a2{53.09_deg + 479264.290_deg * t};
  double a3{313.45_deg + 481266.484_deg * t};
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
      {0, 0, 1, 0, 6288774_deg, -20905355},
      {2, 0, -1, 0, 1274027_deg, -3699111},
      {2, 0, 0, 0, 658314_deg, -2955968},
      {0, 0, 2, 0, 213618_deg, -569925},
      {0, 1, 0, 0, -185116_deg, 48888},
      {0, 0, 0, 2, -114332_deg, -3149},
      {2, 0, -2, 0, 58793_deg, 246158},
      {2, -1, -1, 0, 57066_deg, -152138},
      {2, 0, 1, 0, 53322_deg, -170733},
      {2, -1, 0, 0, 45758_deg, -204586},
      {0, 1, -1, 0, -40923_deg, -129620},
      {1, 0, 0, 0, -34720_deg, 108743},
      {0, 1, 1, 0, -30383_deg, 104755},
      {2, 0, 0, -2, 15327_deg, 10321},
      {0, 0, 1, 2, -12528_deg, 0},
      {0, 0, 1, -2, 10980_deg, 79661},
      {4, 0, -1, 0, 10675_deg, -34782},
      {0, 0, 3, 0, 10034_deg, -23210},
      {4, 0, -2, 0, 8548_deg, -21636},
      {2, 1, -1, 0, -7888_deg, 24208},
      {2, 1, 0, 0, -6766_deg, 30824},
      {1, 0, -1, 0, -5163_deg, -8379},
      {1, 1, 0, 0, 4987_deg, -16675},
      {2, -1, 1, 0, 4036_deg, -12831},
      {2, 0, 2, 0, 3994_deg, -10445},
      {4, 0, 0, 0, 3861_deg, -11650},
      {2, 0, -3, 0, 3665_deg, 14403},
      {0, 1, -2, 0, -2689_deg, -7003},
      {2, 0, -1, 2, -2602_deg, 0},
      {2, -1, -2, 0, 2390_deg, 10056},
      {1, 0, 1, 0, -2348_deg, 6322},
      {2, -2, 0, 0, 2236_deg, -9884},
      {0, 1, 2, 0, -2120_deg, 5751},
      {0, 2, 0, 0, -2069_deg, 0},
      {2, -2, -1, 0, 2048_deg, -4950},
      {2, 0, 1, -2, -1773_deg, 4130},
      {2, 0, 0, 2, -1595_deg, 0},
      {4, -1, -1, 0, 1215_deg, -3958},
      {0, 0, 2, 2, -1110_deg, 0},
      {3, 0, -1, 0, -892_deg, 3258},
      {2, 1, 1, 0, -810_deg, 2616},
      {4, -1, -2, 0, 759_deg, -1897},
      {0, 2, -1, 0, -713_deg, -2117},
      {2, 2, -1, 0, -700_deg, 2354},
      {2, 1, -2, 0, 691_deg, 0},
      {2, -1, 0, -2, 596_deg, 0},
      {4, 0, 1, 0, 549_deg, -1423},
      {0, 0, 4, 0, 537_deg, -1117},
      {4, -1, 0, 0, 520_deg, -1571},
      {1, 0, -2, 0, -487_deg, -1739},
      {2, 1, 0, -2, -399_deg, 0},
      {0, 0, 2, -2, -381_deg, -4421},
      {1, 1, 1, 0, 351_deg, 0},
      {3, 0, -2, 0, -340_deg, 0},
      {4, 0, -3, 0, 330_deg, 0},
      {2, -1, 2, 0, 327_deg, 0},
      {0, 2, 1, 0, -323_deg, 1165},
      {1, 1, -1, 0, 299_deg, 0},
      {2, 0, 3, 0, 294_deg, 0},
      {2, 0, -1, -2, 0_deg, 8752},
  };
  struct {
    int d;
    int m;
    int mp;
    int f;
    double b;
  } periodic_terms_b[]{
      {0, 0, 0, 1, 5128122_deg}, {0, 0, 1, 1, 280602_deg},
      {0, 0, 1, -1, 277693_deg}, {2, 0, 0, -1, 173237_deg},
      {2, 0, -1, 1, 55413_deg},  {2, 0, -1, -1, 46271_deg},
      {2, 0, 0, 1, 32573_deg},   {0, 0, 2, 1, 17198_deg},
      {2, 0, 1, -1, 9266_deg},   {0, 0, 2, -1, 8822_deg},
      {2, -1, 0, -1, 8216_deg},  {2, 0, -2, -1, 4324_deg},
      {2, 0, 1, 1, 4200_deg},    {2, 1, 0, -1, -3359_deg},
      {2, -1, -1, 1, 2463_deg},  {2, -1, 0, 1, 2211_deg},
      {2, -1, -1, -1, 2065_deg}, {0, 1, -1, -1, -1870_deg},
      {4, 0, -1, -1, 1828_deg},  {0, 1, 0, 1, -1794_deg},
      {0, 0, 0, 3, -1749_deg},   {0, 1, -1, 1, -1565_deg},
      {1, 0, 0, 1, -1491_deg},   {0, 1, 1, 1, -1475_deg},
      {0, 1, 1, -1, -1410_deg},  {0, 1, 0, -1, -1344_deg},
      {1, 0, 0, -1, -1335_deg},  {0, 0, 3, 1, 1107_deg},
      {4, 0, 0, -1, 1021_deg},   {4, 0, -1, 1, 833_deg},
      {0, 0, 1, -3, 777_deg},    {4, 0, -2, 1, 671_deg},
      {2, 0, 0, -3, 607_deg},    {2, 0, 2, -1, 596_deg},
      {2, -1, 1, -1, 491_deg},   {2, 0, -2, 1, -451_deg},
      {0, 0, 3, -1, 439_deg},    {2, 0, 2, 1, 422_deg},
      {2, 0, -3, -1, 421_deg},   {2, 1, -1, 1, -366_deg},
      {2, 1, 0, 1, -351_deg},    {4, 0, 0, 1, 331_deg},
      {2, -1, 1, 1, 315_deg},    {2, -2, 0, -1, 302_deg},
      {0, 0, 1, 3, -283_deg},    {2, 1, 1, -1, -229_deg},
      {1, 1, 0, -1, 223_deg},    {1, 1, 0, 1, 223_deg},
      {0, 1, -2, -1, -220_deg},  {2, 1, -1, -1, -220_deg},
      {1, 0, 1, 1, -185_deg},    {2, -1, -2, -1, 181_deg},
      {0, 1, 2, 1, -177_deg},    {4, 0, -2, -1, 176_deg},
      {4, -1, -1, -1, 166_deg},  {1, 0, 1, -1, -164_deg},
      {4, 0, 1, -1, 132_deg},    {1, 0, -1, -1, -119_deg},
      {4, -1, 0, -1, 115_deg},   {2, -2, 0, 1, 107_deg},
  };

  double sum_l{3958_deg * std::sin(a1) + 1962_deg * std::sin(lp - f) +
               318_deg * std::sin(a2)};
  double sum_r{385000560.0};
  for (auto& pt : periodic_terms_lr) {
    double arg{pt.d * d + pt.m * m + pt.mp * mp + pt.f * f};
    sum_l += es[pt.m] * pt.l * std::sin(arg);
    sum_r += es[pt.m] * pt.r * std::cos(arg);
  }
  double sum_b{-2235_deg * std::sin(lp) + 382_deg * std::sin(a3) +
               175_deg * std::sin(a1 - f) + 175_deg * std::sin(a1 + f) +
               127_deg * std::sin(lp - mp) - 115_deg * std::sin(lp + mp)};
  for (auto& pt : periodic_terms_b) {
    double arg{pt.d * d + pt.m * m + pt.mp * mp + pt.f * f};
    sum_b += es[pt.m] * pt.b * std::sin(arg);
  }

  if (p_longitude) *p_longitude = RadUnwind(lp + sum_l / 1000000.0);
  if (p_latitude) *p_latitude = RadNormalize(sum_b / 1000000.0);
  if (p_radius_vector_km) *p_radius_vector_km = sum_r / 1000.0;
}

}  // namespace PA

#endif  // ELP82JM_H_
