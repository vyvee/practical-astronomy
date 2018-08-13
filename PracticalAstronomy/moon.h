#ifndef MOON_H_
#define MOON_H_

#include "elp82jm.h"

namespace PA
{
class Moon
{
   public:
    constexpr Moon(double julian_date) noexcept;
    constexpr double GetGeocentricLongitude() noexcept;
    constexpr double GetGeocentricLatitude() noexcept;
    constexpr double GetDistanceKm() noexcept;

   private:
    double julian_date_;

    constexpr void ComputeGeocentricPosition() noexcept;
    bool geocentric_position_is_valid_{false};
    double geocentric_longitude_{0.0};
    double geocentric_latitude_{0.0};
    double distance_km_{0.0};
};

constexpr Moon::Moon(double julian_date) noexcept : julian_date_(julian_date)
{
}

constexpr double Moon::GetGeocentricLongitude() noexcept
{
    if(!geocentric_position_is_valid_) {
        ComputeGeocentricPosition();
    }
    return geocentric_longitude_;
}

constexpr double Moon::GetGeocentricLatitude() noexcept
{
    if(!geocentric_position_is_valid_) {
        ComputeGeocentricPosition();
    }
    return geocentric_latitude_;
}

constexpr double Moon::GetDistanceKm() noexcept
{
    if(!geocentric_position_is_valid_) {
        ComputeGeocentricPosition();
    }
    return distance_km_;
}

constexpr void Moon::ComputeGeocentricPosition() noexcept
{
    ELP82JM elp82jm{julian_date_};
    geocentric_longitude_ = elp82jm.GetLongitude();
    geocentric_latitude_ = elp82jm.GetLatitude();
    distance_km_ = elp82jm.GetDistanceKm();
    geocentric_position_is_valid_ = true;
}

}  // namespace PA

#endif  // MOON_H_
