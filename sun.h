#ifndef SUN_H_
#define SUN_H_

#include "angle.h"
#include "coordinate.h"
#include "date.h"
#include "vsop87.h"

namespace PA
{
class Sun
{
   public:
    constexpr Sun(double julian_date) noexcept;
    constexpr Radian GetGeometricLongitude() noexcept;
    constexpr Radian GetGeometricLatitude() noexcept;
    constexpr double GetRadiusVectorAU() noexcept;
    constexpr Radian GetApparentLongitude() noexcept;

   private:
    double julian_date_;

    constexpr void ComputePosition() noexcept;
    bool position_is_valid_{false};
    Radian geometric_longitude_{0.0};
    Radian geometric_latitude_{0.0};
    Radian apparent_longitude_{0.0};
    double radius_vector_au_{0.0};
};

constexpr Sun::Sun(double julian_date) noexcept : julian_date_(julian_date)
{
}

constexpr Radian Sun::GetGeometricLongitude() noexcept
{
    if(!position_is_valid_) {
        ComputePosition();
    }
    return geometric_longitude_;
}

constexpr Radian Sun::GetGeometricLatitude() noexcept
{
    if(!position_is_valid_) {
        ComputePosition();
    }
    return geometric_latitude_;
}

constexpr double Sun::GetRadiusVectorAU() noexcept
{
    if(!position_is_valid_) {
        ComputePosition();
    }
    return radius_vector_au_;
}

constexpr Radian Sun::GetApparentLongitude() noexcept
{
    if(!position_is_valid_) {
        ComputePosition();
    }
    return apparent_longitude_;
}

constexpr void Sun::ComputePosition() noexcept
{
    VSOP87 vsop87{julian_date_};
    geometric_longitude_ =
        (vsop87.GetPlanetLongitude(VSOP87::Planet::kEarth) + Radian(M_PI))
            .GetUnwind();
    geometric_latitude_ = -vsop87.GetPlanetLatitude(VSOP87::Planet::kEarth);
    radius_vector_au_ = vsop87.GetPlanetDistanceAU(VSOP87::Planet::kEarth);
    Earth earth{Date(julian_date_)};
    apparent_longitude_ = geometric_longitude_;
    apparent_longitude_ += earth.GetNutationLongitude();
    // TODO: Calculate more accurate aberration
    apparent_longitude_ += -20.4898_arcsec / radius_vector_au_;
    position_is_valid_ = true;
}

}  // namespace PA

#endif  // SUN_H_
