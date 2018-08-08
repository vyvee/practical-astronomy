#ifndef EARTH_H_
#define EARTH_H_

#include "angle.h"
#include "date.h"

#include "earth_nutation.h"

namespace PA
{
class Earth
{
   public:
    constexpr Earth(double jd) noexcept : julian_date_(jd), earth_nutation_(jd)
    {
    }

    constexpr Degree GetObliquity() noexcept;
    constexpr Degree GetObliquityMean() noexcept;
    constexpr Degree GetNutationLongitude() noexcept;
    constexpr Degree GetNutationObliquity() noexcept;

   private:
    double julian_date_;
    EarthNutation earth_nutation_;

    constexpr void ComputeObliquityMean() noexcept;
    bool obliquity_mean_is_valid_{false};
    Degree obliquity_mean_{0.0};
    bool obliquity_is_valid_{false};
    Degree obliquity_{0.0};
};

constexpr Degree Earth::GetObliquity() noexcept
{
    if(!obliquity_mean_is_valid_) {
        ComputeObliquityMean();
    }
    return obliquity_mean_ + earth_nutation_.GetNutationObliquity();
}

constexpr Degree Earth::GetObliquityMean() noexcept
{
    if(!obliquity_mean_is_valid_) {
        ComputeObliquityMean();
    }
    return obliquity_mean_;
}

constexpr Degree Earth::GetNutationLongitude() noexcept
{
    return earth_nutation_.GetNutationLongitude();
}

constexpr Degree Earth::GetNutationObliquity() noexcept
{
    return earth_nutation_.GetNutationObliquity();
}

constexpr void Earth::ComputeObliquityMean() noexcept
{
    // References:
    // - http://www.neoprogrammics.com/obliquity_of_the_ecliptic/
    // - [Jean99] Chapter 22, p.147
    double u{(julian_date_ - PA::EpochJ2000) / 3652500.0};
    obliquity_mean_ =
        Degree(horner_polynomial({+84381.448, -4680.93, -1.55, +1999.25, -51.38,
                                  -249.67, -39.05, +7.12, +27.87, +5.79, +2.45},
                                 u) /
               3600.0);
    obliquity_mean_is_valid_ = true;
}

}  // namespace PA

#endif  // EARTH_H_
