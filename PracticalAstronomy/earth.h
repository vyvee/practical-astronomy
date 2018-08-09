#ifndef EARTH_H_
#define EARTH_H_

#include "angle.h"
#include "date.h"

#include "earth_nutation.h"
#include "earth_obliquity.h"

namespace PA
{
class Earth
{
   public:
    constexpr Earth(double jd) noexcept
        : julian_date_(jd), earth_nutation_(jd), earth_obliquity_(jd)
    {
    }

    constexpr Degree GetNutationLongitude() noexcept;
    constexpr Degree GetNutationObliquity() noexcept;
    constexpr Degree GetObliquityMean() noexcept;
    constexpr Degree GetObliquity() noexcept;

   private:
    double julian_date_;
    EarthNutation earth_nutation_;
    EarthObliquity earth_obliquity_;
};

constexpr Degree Earth::GetNutationLongitude() noexcept
{
    return earth_nutation_.GetNutationLongitude();
}

constexpr Degree Earth::GetNutationObliquity() noexcept
{
    return earth_nutation_.GetNutationObliquity();
}

constexpr Degree Earth::GetObliquityMean() noexcept
{
    return earth_obliquity_.GetObliquityMean();
}

constexpr Degree Earth::GetObliquity() noexcept
{
    return earth_obliquity_.GetObliquityMean() +
           earth_nutation_.GetNutationObliquity();
}

}  // namespace PA

#endif  // EARTH_H_