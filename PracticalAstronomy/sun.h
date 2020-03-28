#ifndef SUN_H_
#define SUN_H_

#include "radian.h"
#include "utils.h"

namespace PA {
class Sun {
 public:
  static constexpr double GetDailyVariation(double tt) noexcept;

  // constexpr double GetMeanLongitude() noexcept;

 private:
  constexpr Sun() noexcept;

  // constexpr void ComputeMeanLongitude() noexcept;
  // bool mean_longitude_is_valid_{false};
  // double mean_longitude_{0.0};
};

constexpr double Sun::GetDailyVariation(double tt) noexcept {
  // [Jean99] pp.167-168
  // Accuracy: <= 0".1
  constexpr struct PeriodicTerm variation_d0[]{
      // Eccentricity of Earth's orbit
      {118.568_arcsec, 87.5287_deg, 359993.7286_deg},
      // Eccentricity of Earth's orbit
      {2.476_arcsec, 85.0561_deg, 719987.4571_deg},
      // Due to Moon
      {1.376_arcsec, 27.8502_deg, 4452671.1152_deg},
      // Due to Venus
      {0.119_arcsec, 73.1375_deg, 450368.8564_deg},
      // Due to Jupiter
      {0.114_arcsec, 337.2264_deg, 329644.6718_deg},
      // Due to Jupiter
      {0.086_arcsec, 222.5400_deg, 659289.3436_deg},
      // Due to Moon
      {0.078_arcsec, 162.8136_deg, 9224659.7915_deg},
      // Eccentricity of Earth's orbit
      {0.054_arcsec, 82.5823_deg, 1079981.1857_deg},
      // Due to Venus
      {0.052_arcsec, 171.5189_deg, 225184.4282_deg},
      // Due to Moon
      {0.034_arcsec, 30.3214_deg, 4092677.3866_deg},
      // Due to Mars
      {0.033_arcsec, 119.8105_deg, 337181.4711_deg},
      // Due to Jupiter
      {0.023_arcsec, 247.5418_deg, 299295.6151_deg},
      // Due to Venus
      {0.023_arcsec, 325.1526_deg, 315559.5560_deg},
      // Due to Venus
      {0.021_arcsec, 155.1241_deg, 675553.2846_deg},
  };
  constexpr struct PeriodicTerm variation_d1[]{
      // Eccentricity of Earth's orbit
      {7.311_arcsec, 333.4515_deg, 359993.7286_deg},
      // Eccentricity of Earth's orbit
      {0.305_arcsec, 330.9814_deg, 719987.4571_deg},
      // Eccentricity of Earth's orbit
      {0.010_arcsec, 328.5170_deg, 1079981.1857_deg},
  };
  constexpr struct PeriodicTerm variation_d2[]{
      // Eccentricity of Earth's orbit
      {0.309_arcsec, 241.4518_deg, 359993.7286_deg},
      // Eccentricity of Earth's orbit
      {0.021_arcsec, 205.0482_deg, 719987.4571_deg},
      // Due to Moon
      {0.004_arcsec, 297.8610_deg, 4452671.1152_deg},
  };
  constexpr struct PeriodicTerm variation_d3[]{
      // Eccentricity of Earth's orbit
      {0.010_arcsec, 154.7066_deg, 359993.7286_deg},
  };
  const struct PeriodicTermTableDegree variation_d_table_degrees[]{
      {variation_d0, sizeof(variation_d0) / sizeof(variation_d0[0])},
      {variation_d1, sizeof(variation_d1) / sizeof(variation_d1[0])},
      {variation_d2, sizeof(variation_d2) / sizeof(variation_d2[0])},
      {variation_d3, sizeof(variation_d3) / sizeof(variation_d3[0])},
  };
  const struct PeriodicTermTable variation_d_table {
    .degrees = variation_d_table_degrees,
    .size = sizeof(variation_d_table_degrees) /
            sizeof(variation_d_table_degrees[0]),
    .method = PeriodicTermTable::Method::kSin,
  };

  double tau{(tt - EpochJ2000) / 365250.0};
  // For value:
  // - w.r.t. fixed reference frame: Use 3548.193_arcsec
  // - w.r.t. the mean equinox of the date: Use 3548.330_arcsec
  return 3548.193_arcsec + PeriodicTermCompute(variation_d_table, tau);
}

#if 0
constexpr void Sun::ComputeGeocentricPositionLowAccuracy() noexcept {
  // Low Accuracy
  // - Accuracy: < 0.01 degree in longitude
  //   - This corresponds to about 15 minutes
  // - [Jean99] p.163
  double t{(julian_date_ - EpochJ2000) / 36525.0};
  double l0 = 280.46646_deg + (36000.76983_deg + 0.0003032_deg * t) * t;
  double m = 357.52911_deg + (35999.05029_deg + 0.0001537_deg * t) * t;
  double e = 0.016708634 + (-0.000042037 - 0.0000001267 * t) * t;
  double c =
      (1.914602_deg + (-0.004817_deg - 0.000014_deg * t) * t) * std::sin(m) +
      (0.019993_deg - 0.000101_deg * t) * std::sin(2 * m) +
      0.000289_deg * std::sin(3 * m);
  double true_lon = l0 + c;
  double v = m + c;
  geocentric_longitude_ = RadUnwind(true_lon);
  geocentric_latitude_ = 0.0;
  radius_vector_au_ = 1.000001018 * (1 - e * e) / (1 + e * std::cos(v));
  geocentric_position_is_valid_ = true;
}

constexpr double Sun::GetMeanLongitude() noexcept {
  if (!mean_longitude_is_valid_) {
    ComputeMeanLongitude();
  }
  return mean_longitude_;
}

constexpr void Sun::ComputeMeanLongitude() noexcept {
  double tau{(julian_date_ - PA::EpochJ2000) / 365250.0};
  mean_longitude_ = RadUnwind(horner_polynomial(
      {
          280.4664567_deg,
          360007.6982779_deg,
          0.03032028_deg,
          1.0_deg / 49931,
          -1.0_deg / 15300,
          -1.0_deg / 2000000,
      },
      tau));
  mean_longitude_is_valid_ = true;
}  // namespace PA

#include "misc.h"

constexpr void Sun::ComputeAberrationLowAccuracy() noexcept {
  // [Jean99] pp.167
  // Accuracy: 0".01
  aberration_longitude_ = -20.4898_arcsec / radius_vector_au_;
  aberration_latitude_ = 0.0;
  aberration_is_valid_ = true;
}

constexpr void Sun::ComputeApparentPositionLowAccuracy() noexcept {
  double t{(julian_date_ - EpochJ2000) / 36525.0};
  double omega = 125.04_deg - 1934.136_deg * t;
  apparent_longitude_ =
      GetGeocentricLongitude() - 0.00569_deg - 0.00478_deg * std::sin(omega);
  apparent_latitude_ = GetGeocentricLatitude();
  apparent_position_is_valid_ = true;
}
#endif

}  // namespace PA

#endif  // SUN_H_
