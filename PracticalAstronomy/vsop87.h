#ifndef VSOP87_H_
#define VSOP87_H_

#include <cmath>

#include "radian.h"
#include "utils.h"

namespace PA {

class VSOP87 {
 public:
  enum class Planet {
    kMercury,
    kVenus,
    kEarth,
    kMars,
    kJupiter,
    kSaturn,
    kUranus,
    kNeptune,
    kMax,
  };

  static constexpr void Compute(double tt, Planet planet, double *p_longitude, double *p_latitude,
                                double *radius_vector_au) noexcept;
  static constexpr void VSOP87DFrameToFK5(double tt, double *p_longitude,
                                          double *p_latitude) noexcept;

 private:
  constexpr VSOP87() noexcept {}

#include "vsop87_internal.dat"

  static constexpr PeriodicTermTable periodic_term_l_tables[]{
      [static_cast<int>(Planet::kMercury)] = mercury_l_table,
      [static_cast<int>(Planet::kVenus)] = venus_l_table,
      [static_cast<int>(Planet::kEarth)] = earth_l_table,
      [static_cast<int>(Planet::kMars)] = mars_l_table,
      [static_cast<int>(Planet::kJupiter)] = jupiter_l_table,
      [static_cast<int>(Planet::kSaturn)] = saturn_l_table,
      [static_cast<int>(Planet::kUranus)] = uranus_l_table,
      [static_cast<int>(Planet::kNeptune)] = neptune_l_table,
  };

  static constexpr PeriodicTermTable periodic_term_b_tables[]{
      [static_cast<int>(Planet::kMercury)] = mercury_b_table,
      [static_cast<int>(Planet::kVenus)] = venus_b_table,
      [static_cast<int>(Planet::kEarth)] = earth_b_table,
      [static_cast<int>(Planet::kMars)] = mars_b_table,
      [static_cast<int>(Planet::kJupiter)] = jupiter_b_table,
      [static_cast<int>(Planet::kSaturn)] = saturn_b_table,
      [static_cast<int>(Planet::kUranus)] = uranus_b_table,
      [static_cast<int>(Planet::kNeptune)] = neptune_b_table,
  };

  static constexpr PeriodicTermTable periodic_term_r_tables[]{
      [static_cast<int>(Planet::kMercury)] = mercury_r_table,
      [static_cast<int>(Planet::kVenus)] = venus_r_table,
      [static_cast<int>(Planet::kEarth)] = earth_r_table,
      [static_cast<int>(Planet::kMars)] = mars_r_table,
      [static_cast<int>(Planet::kJupiter)] = jupiter_r_table,
      [static_cast<int>(Planet::kSaturn)] = saturn_r_table,
      [static_cast<int>(Planet::kUranus)] = uranus_r_table,
      [static_cast<int>(Planet::kNeptune)] = neptune_r_table,
  };
};

constexpr void VSOP87::Compute(double tt, VSOP87::Planet planet, double *p_longitude,
                               double *p_latitude, double *p_radius_vector_au) noexcept {
  // References:
  // - [Jean99] p.217: Chapter 32 (Positions of Planets)
  // - [Jean99] Chapter 25 (Solar Coordinates)
  // - [Jean99] p.413: Appendix III (Planets: Periodic Terms)
  // - ftp://ftp.imcce.fr/pub/ephem/planets/vsop87
  // - http://neoprogrammics.com/vsop87/

  double tau{(tt - EpochJ2000) / 365250.0};
  double longitude{0.0};
  double latitude{0.0};
  double radius_vector_au{0.0};

  longitude = PeriodicTermCompute(periodic_term_l_tables[static_cast<int>(planet)], tau);
  latitude = PeriodicTermCompute(periodic_term_b_tables[static_cast<int>(planet)], tau);
  radius_vector_au = PeriodicTermCompute(periodic_term_r_tables[static_cast<int>(planet)], tau);

  if (p_longitude) *p_longitude = longitude;
  if (p_latitude) *p_latitude = latitude;
  if (p_radius_vector_au) *p_radius_vector_au = radius_vector_au;
}

constexpr void VSOP87::VSOP87DFrameToFK5(double tt, double *p_longitude,
                                         double *p_latitude) noexcept {
  // Conversion: Mean *dynamical* equinox and ecliptic of the date to FK5
  // - Reference [Jean99] p.219
  double t{(tt - EpochJ2000) / 36525.0};
  double lp{*p_longitude + (-1.397_deg - 0.00031_deg * t) * t};
  *p_longitude +=
      -0.09033_arcsec + 0.03916_arcsec * (std::cos(lp) + std::sin(lp)) * std::tan(*p_latitude);
  *p_latitude += 0.03916_arcsec * (std::cos(lp) - std::sin(lp));
}

}  // namespace PA

#endif  // VSOP87_H_
