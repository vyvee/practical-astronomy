#ifndef SUN_H_
#define SUN_H_

#include "date.h"
#include "radian.h"
#include "utils.h"
#include "vsop87.h"

namespace PA {
class Sun {
 public:
  constexpr Sun(double julian_date) noexcept;
  constexpr double GetGeocentricLongitude() noexcept;
  constexpr double GetGeocentricLatitude() noexcept;
  constexpr double GetRadiusVectorAU() noexcept;
  constexpr double GetAberrationLongitude() noexcept;
  constexpr double GetAberrationLatitude() noexcept;
  constexpr double GetApparentLongitude(
      const EarthNutation& earth_nutation) noexcept;
  constexpr double GetApparentLongitude() noexcept;
  constexpr double GetApparentLatitude(
      const EarthNutation& earth_nutation) noexcept;
  constexpr double GetApparentLatitude() noexcept;
  constexpr double GetMeanLongitude() noexcept;
  constexpr double GetJulianDate() const noexcept { return julian_date_; };

 private:
  double julian_date_;

  constexpr void ComputeGeocentricPositionLowAccuracy() noexcept;
  constexpr void ComputeGeocentricPositionVSOP87() noexcept;
  constexpr void ComputeGeocentricPosition() noexcept;
  bool geocentric_position_is_valid_{false};
  double geocentric_longitude_{0.0};
  double geocentric_latitude_{0.0};
  double radius_vector_au_{0.0};

  constexpr void ComputeAberrationLowAccuracy() noexcept;
  constexpr void ComputeAberrationHighAccuracy() noexcept;
  constexpr void ComputeAberration() noexcept;
  bool aberration_is_valid_{false};
  double aberration_longitude_{0.0};
  double aberration_latitude_{0.0};

  constexpr void ComputeApparentPositionLowAccuracy() noexcept;
  constexpr void ComputeApparentPositionHighAccuracy(
      const EarthNutation& earth_nutation) noexcept;
  constexpr void ComputeApparentPosition(
      const EarthNutation& earth_nutation) noexcept;
  constexpr void ComputeApparentPosition() noexcept;
  bool apparent_position_is_valid_{false};
  double apparent_longitude_{0.0};
  double apparent_latitude_{0.0};

  constexpr void ComputeMeanLongitude() noexcept;
  bool mean_longitude_is_valid_{false};
  double mean_longitude_{0.0};
};

constexpr Sun::Sun(double julian_date) noexcept : julian_date_(julian_date) {}

constexpr double Sun::GetGeocentricLongitude() noexcept {
  if (!geocentric_position_is_valid_) {
    ComputeGeocentricPosition();
  }
  return geocentric_longitude_;
}

constexpr double Sun::GetGeocentricLatitude() noexcept {
  if (!geocentric_position_is_valid_) {
    ComputeGeocentricPosition();
  }
  return geocentric_latitude_;
}

constexpr double Sun::GetRadiusVectorAU() noexcept {
  if (!geocentric_position_is_valid_) {
    ComputeGeocentricPosition();
  }
  return radius_vector_au_;
}

constexpr double Sun::GetAberrationLongitude() noexcept {
  if (!aberration_is_valid_) {
    ComputeAberration();
  }
  return aberration_longitude_;
}

constexpr double Sun::GetAberrationLatitude() noexcept {
  if (!aberration_is_valid_) {
    ComputeAberration();
  }
  return aberration_latitude_;
}

constexpr double Sun::GetApparentLongitude(
    const EarthNutation& earth_nutation) noexcept {
  if (!apparent_position_is_valid_) {
    ComputeApparentPosition(earth_nutation);
  }
  return apparent_longitude_;
}

constexpr double Sun::GetApparentLongitude() noexcept {
  if (!apparent_position_is_valid_) {
    ComputeApparentPosition();
  }
  return apparent_longitude_;
}

constexpr double Sun::GetApparentLatitude(
    const EarthNutation& earth_nutation) noexcept {
  if (!apparent_position_is_valid_) {
    ComputeApparentPosition(earth_nutation);
  }
  return apparent_latitude_;
}

constexpr double Sun::GetApparentLatitude() noexcept {
  if (!apparent_position_is_valid_) {
    ComputeApparentPosition();
  }
  return apparent_latitude_;
}

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

constexpr void Sun::ComputeGeocentricPositionVSOP87() noexcept {
  // VSOP87
  // - Accuracy: < 0.01" in longitude over 1900-2100
  // - Sun moves about 0.04" in longitude per second, so this corresponds to
  //   about 0.25 seconds
  // - [Jean99] p.166
  VSOP87 vsop87{julian_date_};
  geocentric_longitude_ =
      RadUnwind(vsop87.GetPlanetLongitude(VSOP87::Planet::kEarth) + M_PI);
  geocentric_latitude_ = -vsop87.GetPlanetLatitude(VSOP87::Planet::kEarth);
  radius_vector_au_ = vsop87.GetPlanetRadiusVectorAU(VSOP87::Planet::kEarth);
  geocentric_position_is_valid_ = true;
}

constexpr void Sun::ComputeGeocentricPosition() noexcept {
  // ComputeGeocentricPositionLowAccuracy();
  ComputeGeocentricPositionVSOP87();
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

constexpr void Sun::ComputeAberrationHighAccuracy() noexcept {
  // [Jean99] pp.167-168
  // Accuracy: 0".001
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

  double tau{(julian_date_ - EpochJ2000) / 365250.0};
  // For value:
  // - w.r.t. fixed reference frame: Use 3548.193_arcsec
  // - w.r.t. the mean equinox of the date: Use 3548.330_arcsec
  double daily_variation{3548.193_arcsec +
                         PeriodicTermCompute(variation_d_table, tau)};
  aberration_longitude_ = -0.005775518 * radius_vector_au_ * daily_variation;
  // Should be less than 0.00001 arsec
  aberration_latitude_ =
      AberrationLatitude(geocentric_longitude_, geocentric_latitude_,
                         julian_date_, GetGeocentricLongitude());
  aberration_is_valid_ = true;
}

constexpr void Sun::ComputeAberration() noexcept {
  // ComputeAberrationLowAccuracy();
  ComputeAberrationHighAccuracy();
}

constexpr void Sun::ComputeApparentPositionLowAccuracy() noexcept {
  double t{(julian_date_ - EpochJ2000) / 36525.0};
  double omega = 125.04_deg - 1934.136_deg * t;
  apparent_longitude_ =
      GetGeocentricLongitude() - 0.00569_deg - 0.00478_deg * std::sin(omega);
  apparent_latitude_ = GetGeocentricLatitude();
  apparent_position_is_valid_ = true;
}

constexpr void Sun::ComputeApparentPositionHighAccuracy(
    const EarthNutation& earth_nutation) noexcept {
  apparent_longitude_ = GetGeocentricLongitude() +
                        earth_nutation.GetNutationLongitude() +
                        GetAberrationLongitude();
  apparent_latitude_ = GetGeocentricLatitude() + GetAberrationLatitude();
  apparent_position_is_valid_ = true;
}

constexpr void Sun::ComputeApparentPosition(
    const EarthNutation& earth_nutation) noexcept {
  // ComputeApparentPositionLowAccuracy();
  ComputeApparentPositionHighAccuracy(earth_nutation);
}

constexpr void Sun::ComputeApparentPosition() noexcept {
  // ComputeApparentPositionLowAccuracy();
  ComputeApparentPositionHighAccuracy(EarthNutation{julian_date_});
}

}  // namespace PA

#endif  // SUN_H_
