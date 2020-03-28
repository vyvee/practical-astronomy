#ifndef EARTH_OBLIQUITY_H_
#define EARTH_OBLIQUITY_H_

#include "radian.h"
#include "utils.h"

namespace PA {

class EarthObliquity {
 public:
  static constexpr double ComputeObliquityMean(double tt) noexcept;

 protected:
  constexpr EarthObliquity() noexcept {}
};

constexpr double EarthObliquity::ComputeObliquityMean(double tt) noexcept {
  // References:
  // - http://www.neoprogrammics.com/obliquity_of_the_ecliptic/
  // - [Jean99] Chapter 22, p.147
  // J. Lasker's Formula
  // Accuracy: 0".01 after 1000 years (i.e., A.D. 1000 and 3000),
  //           a few seconds of arc after 10000 years
  return horner_polynomial(
      {+84381.448_arcsec, -4680.93_arcsec, -1.55_arcsec, +1999.25_arcsec,
       -51.38_arcsec, -249.67_arcsec, -39.05_arcsec, +7.12_arcsec,
       +27.87_arcsec, +5.79_arcsec, +2.45_arcsec},
      (tt - PA::EpochJ2000) / 3652500.0);
}

/*--- ??? Obsolete ---*/

class EarthObliquityOld {
 public:
  constexpr EarthObliquityOld(double jd) noexcept;

  constexpr double GetObliquity() noexcept;

 private:
  double julian_date_;
  EarthNutationOld earth_nutation_;
};

constexpr EarthObliquityOld::EarthObliquityOld(double jd) noexcept
    : julian_date_(jd), earth_nutation_(jd) {}

constexpr double EarthObliquityOld::GetObliquity() noexcept {
  return EarthObliquity::ComputeObliquityMean(julian_date_) +
         earth_nutation_.GetNutationObliquity();
}

}  // namespace PA

#endif  // EARTH_OBLIQUITY_H_
