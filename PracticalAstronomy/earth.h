#ifndef EARTH_H_
#define EARTH_H_

#include "date.h"
#include "earth_nutation.h"
#include "earth_obliquity.h"
#include "radian.h"

namespace PA {
class Earth {
 public:
  constexpr Earth(double jd) noexcept
      : julian_date_(jd), earth_nutation_(jd), earth_obliquity_(jd) {}

  constexpr double GetNutationLongitude() noexcept;
  constexpr double GetNutationObliquity() noexcept;
  constexpr double GetObliquityMean() noexcept;
  constexpr double GetObliquity() noexcept;

 private:
  double julian_date_;
  EarthNutation earth_nutation_;
  EarthObliquity earth_obliquity_;
};

constexpr double Earth::GetNutationLongitude() noexcept {
  return earth_nutation_.GetNutationLongitude();
}

constexpr double Earth::GetNutationObliquity() noexcept {
  return earth_nutation_.GetNutationObliquity();
}

constexpr double Earth::GetObliquityMean() noexcept {
  return earth_obliquity_.GetObliquityMean();
}

constexpr double Earth::GetObliquity() noexcept {
  return earth_obliquity_.GetObliquityMean() +
         earth_nutation_.GetNutationObliquity();
}

}  // namespace PA

#endif  // EARTH_H_
