#ifndef MOON_H_
#define MOON_H_

#include <cmath>

#include "earth_nutation.h"
#include "elp82jm.h"

namespace PA {
class Moon {
 public:
  constexpr Moon(double julian_date) noexcept;
  constexpr double GetGeocentricLongitude() noexcept;
  constexpr double GetGeocentricLatitude() noexcept;
  constexpr double GetDistanceKm() noexcept;
  constexpr double GetApparentLongitude() noexcept;
  constexpr double GetApparentLatitude() noexcept;

 private:
  double julian_date_;

  constexpr void ComputeGeocentricPosition() noexcept;
  bool geocentric_position_is_valid_{false};
  double geocentric_longitude_{0.0};
  double geocentric_latitude_{0.0};
  double distance_km_{0.0};

  constexpr void ComputeApparentPosition(
      const EarthNutation& earth_nutation) noexcept;
  constexpr void ComputeApparentPosition() noexcept;
  bool apparent_position_is_valid_{false};
  double apparent_longitude_{0.0};
  double apparent_latitude_{0.0};
};

constexpr Moon::Moon(double julian_date) noexcept : julian_date_(julian_date) {}

constexpr double Moon::GetGeocentricLongitude() noexcept {
  if (!geocentric_position_is_valid_) {
    ComputeGeocentricPosition();
  }
  return geocentric_longitude_;
}

constexpr double Moon::GetGeocentricLatitude() noexcept {
  if (!geocentric_position_is_valid_) {
    ComputeGeocentricPosition();
  }
  return geocentric_latitude_;
}

constexpr double Moon::GetDistanceKm() noexcept {
  if (!geocentric_position_is_valid_) {
    ComputeGeocentricPosition();
  }
  return distance_km_;
}

constexpr double Moon::GetApparentLongitude() noexcept {
  if (!apparent_position_is_valid_) {
    ComputeApparentPosition();
  }
  return apparent_longitude_;
}

constexpr double Moon::GetApparentLatitude() noexcept {
  if (!apparent_position_is_valid_) {
    ComputeApparentPosition();
  }
  return apparent_latitude_;
}

constexpr void Moon::ComputeGeocentricPosition() noexcept {
  ELP82JM elp82jm{julian_date_};
  geocentric_longitude_ = elp82jm.GetLongitude();
  geocentric_latitude_ = elp82jm.GetLatitude();
  distance_km_ = elp82jm.GetDistanceKm();
  geocentric_position_is_valid_ = true;
}

constexpr void Moon::ComputeApparentPosition(
    const EarthNutation& earth_nutation) noexcept {
  // [Jean99] p.343
  // [Jean99] p.337: ?? The effect of aberration has been accounted for?
  // (-0.70")
  // + GetAberrationLongitude();
  // - Also: http://adsabs.harvard.edu/full/1952AJ.....57...46C
  apparent_longitude_ =
      GetGeocentricLongitude() + earth_nutation.GetNutationLongitude();
  // [Jean99] p.337: ?? The effect of aberration has been accounted for?
  // (-0.70")
  apparent_latitude_ = GetGeocentricLatitude();
  apparent_position_is_valid_ = true;
}

constexpr void Moon::ComputeApparentPosition() noexcept {
  ComputeApparentPosition(EarthNutation{julian_date_});
}

}  // namespace PA

#endif  // MOON_H_
