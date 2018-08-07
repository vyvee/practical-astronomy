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
    constexpr Radian GetAberrationLongitude() noexcept;
    constexpr Radian GetAberrationLatitude() noexcept;
    constexpr Radian GetApparentLongitude() noexcept;

   private:
    double julian_date_;

    constexpr void ComputePosition() noexcept;
    bool position_is_valid_{false};
    Radian geometric_longitude_{0.0};
    Radian geometric_latitude_{0.0};
    Radian apparent_longitude_{0.0};
    Radian apparent_latitude_{0.0};
    double radius_vector_au_{0.0};

    constexpr void ComputeAberration() noexcept;
    bool aberration_is_valid_{false};
    Radian aberration_longitude_{0.0};
    Radian aberration_latitude_{0.0};
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

constexpr Radian Sun::GetAberrationLongitude() noexcept
{
    if(!aberration_is_valid_) {
        ComputeAberration();
    }
    return aberration_longitude_;
}

constexpr Radian Sun::GetAberrationLatitude() noexcept
{
    if(!aberration_is_valid_) {
        ComputeAberration();
    }
    return aberration_latitude_;
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
    radius_vector_au_ = vsop87.GetPlanetRadiusVectorAU(VSOP87::Planet::kEarth);

    Earth earth{Date(julian_date_)};
    apparent_longitude_ = geometric_longitude_;
    apparent_longitude_ += earth.GetNutationLongitude();
    apparent_longitude_ += GetAberrationLongitude();

    apparent_latitude_ = geometric_latitude_;
    apparent_latitude_ += GetAberrationLatitude();
}

constexpr void Sun::ComputeAberration() noexcept
{
    constexpr struct PeriodicTerm variation_d0[]{
        {(3548.193_arcsec).Rad(), M_PI_2, 0.0},
        {(118.568_arcsec).Rad(), (87.5287_deg).Rad(), (359993.7286_deg).Rad()},
        {(2.476_arcsec).Rad(), (85.0561_deg).Rad(), (719987.4571_deg).Rad()},
        {(1.376_arcsec).Rad(), (27.8502_deg).Rad(), (4452671.1152_deg).Rad()},
        {(0.119_arcsec).Rad(), (73.1375_deg).Rad(), (450368.8564_deg).Rad()},
        {(0.114_arcsec).Rad(), (337.2264_deg).Rad(), (329644.6718_deg).Rad()},
        {(0.086_arcsec).Rad(), (222.5400_deg).Rad(), (659289.3436_deg).Rad()},
        {(0.078_arcsec).Rad(), (162.8136_deg).Rad(), (9224659.7915_deg).Rad()},
        {(0.054_arcsec).Rad(), (82.5823_deg).Rad(), (1079981.1857_deg).Rad()},
        {(0.052_arcsec).Rad(), (171.5189_deg).Rad(), (225184.4282_deg).Rad()},
        {(0.034_arcsec).Rad(), (30.3214_deg).Rad(), (4092677.3866_deg).Rad()},
        {(0.033_arcsec).Rad(), (119.8105_deg).Rad(), (337181.4711_deg).Rad()},
        {(0.023_arcsec).Rad(), (247.5418_deg).Rad(), (299295.6151_deg).Rad()},
        {(0.023_arcsec).Rad(), (325.1526_deg).Rad(), (315559.5560_deg).Rad()},
        {(0.021_arcsec).Rad(), (155.1241_deg).Rad(), (675553.2846_deg).Rad()},
    };
    constexpr struct PeriodicTerm variation_d1[]{
        {(7.311_arcsec).Rad(), (333.4515_deg).Rad(), (359993.7286_deg).Rad()},
        {(0.305_arcsec).Rad(), (330.9814_deg).Rad(), (719987.4571_deg).Rad()},
        {(0.010_arcsec).Rad(), (328.5170_deg).Rad(), (1079981.1857_deg).Rad()},
    };
    constexpr struct PeriodicTerm variation_d2[]{
        {(0.309_arcsec).Rad(), (241.4518_deg).Rad(), (359993.7286_deg).Rad()},
        {(0.021_arcsec).Rad(), (205.0482_deg).Rad(), (719987.4571_deg).Rad()},
        {(0.004_arcsec).Rad(), (297.8610_deg).Rad(), (4452671.1152_deg).Rad()},
    };
    constexpr struct PeriodicTerm variation_d3[]{
        {(0.010_arcsec).Rad(), (154.7066_deg).Rad(), (359993.7286_deg).Rad()},
    };
    const struct PeriodicTermTableDegree variation_d_table_degrees[]{
        {variation_d0, 15},
        {variation_d1, 3},
        {variation_d2, 3},
        {variation_d3, 1},
    };
    const struct PeriodicTermTable variation_d_table {
        .degrees = variation_d_table_degrees, .size = 4,
        .method = PeriodicTermTable::Method::kSin,
    };

    double tau{(julian_date_ - EpochJ2000) / 365250.0};
    double daily_variation{PeriodicTermCompute(variation_d_table, tau)};
    aberration_longitude_ =
        Radian(-0.005775518 * radius_vector_au_ * daily_variation);
    aberration_latitude_ = Radian(0.0);
    // Low Accuracy:
    // apparent_longitude_ += -20.4898_arcsec / radius_vector_au_;
}

}  // namespace PA

#endif  // SUN_H_
