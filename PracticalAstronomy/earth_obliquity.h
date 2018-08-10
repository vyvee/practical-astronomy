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
    , obliquity_mean_(DegToRad(
          horner_polynomial({+84381.448, -4680.93, -1.55, +1999.25, -51.38,
                             -249.67, -39.05, +7.12, +27.87, +5.79, +2.45},
                            (julian_date_ - PA::EpochJ2000) / 3652500.0) /
          3600.0))
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
