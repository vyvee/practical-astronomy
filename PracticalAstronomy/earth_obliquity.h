#ifndef EARTH_OBLIQUITY_H_
#define EARTH_OBLIQUITY_H_

// J. Lasker's Formula
// Accuracy: 0.01" between 1000 and 3000, and a few arcseconds between -9000 and

#include "angle.h"
#include "date.h"
#include "utils.h"

#include "earth_nutation.h"

namespace PA
{
class EarthObliquity
{
   public:
    constexpr EarthObliquity(double jd) noexcept;

    constexpr double GetObliquity() noexcept;
    constexpr double GetObliquityMean() noexcept;

   private:
    double julian_date_;
    double obliquity_mean_;
    EarthNutation earth_nutation_;
};

constexpr EarthObliquity::EarthObliquity(double jd) noexcept
    : julian_date_(jd)
    , obliquity_mean_(horner_polynomial(
          {+84381.448_arcsec, -4680.93_arcsec, -1.55_arcsec, +1999.25_arcsec,
           -51.38_arcsec, -249.67_arcsec, -39.05_arcsec, +7.12_arcsec,
           +27.87_arcsec, +5.79_arcsec, +2.45_arcsec},
          (julian_date_ - PA::EpochJ2000) / 3652500.0))
    , earth_nutation_(jd)
{
    // References:
    // - http://www.neoprogrammics.com/obliquity_of_the_ecliptic/
    // - [Jean99] Chapter 22, p.147
}

constexpr double EarthObliquity::GetObliquity() noexcept
{
    return obliquity_mean_ + earth_nutation_.GetNutationObliquity();
}

constexpr double EarthObliquity::GetObliquityMean() noexcept
{
    return obliquity_mean_;
}

}  // namespace PA

#endif  // EARTH_OBLIQUITY_H_
