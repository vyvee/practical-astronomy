#ifndef VSOP87_H_
#define VSOP87_H_

#include <cmath>

#include "angle.h"
#include "date.h"
#include "utils.h"

namespace PA
{
class VSOP87
{
   public:
    enum class Planet {
        kMercury,
        kVenus,
        kEarth,
        kMars,
        kJupiter,
        kSaturn,
        kUranus,
        kNeptune,
    };

    constexpr VSOP87(double julian_date) noexcept : julian_date_(julian_date)
    {
    }

    constexpr Radian GetPlanetLongitude(Planet planet) noexcept;
    constexpr Radian GetPlanetLatitude(Planet planet) noexcept;
    constexpr double GetPlanetRadiusVectorAU(Planet planet) noexcept;

   private:
    double julian_date_;

#include "vsop87_internal.dat"

    static constexpr PeriodicTermTable periodic_term_l_tables[]{
        [static_cast<int>(Planet::kMercury)] = mercury_l_table,
        [static_cast<int>(Planet::kVenus)] = venus_l_table,
        [static_cast<int>(Planet::kEarth)] = earth_l_table,
        [static_cast<int>(Planet::kMars)] = mars_l_table,
        [static_cast<int>(Planet::kJupiter)] = jupiter_l_table,
        [static_cast<int>(Planet::kSaturn)] = saturn_l_table,
        [static_cast<int>(Planet::kUranus)] = uranus_l_table,
        [static_cast<int>(Planet::kNeptune)] = neptune_l_table,
    };

    static constexpr PeriodicTermTable periodic_term_b_tables[]{
        [static_cast<int>(Planet::kMercury)] = mercury_b_table,
        [static_cast<int>(Planet::kVenus)] = venus_b_table,
        [static_cast<int>(Planet::kEarth)] = earth_b_table,
        [static_cast<int>(Planet::kMars)] = mars_b_table,
        [static_cast<int>(Planet::kJupiter)] = jupiter_b_table,
        [static_cast<int>(Planet::kSaturn)] = saturn_b_table,
        [static_cast<int>(Planet::kUranus)] = uranus_b_table,
        [static_cast<int>(Planet::kNeptune)] = neptune_b_table,
    };

    static constexpr PeriodicTermTable periodic_term_r_tables[]{
        [static_cast<int>(Planet::kMercury)] = mercury_r_table,
        [static_cast<int>(Planet::kVenus)] = venus_r_table,
        [static_cast<int>(Planet::kEarth)] = earth_r_table,
        [static_cast<int>(Planet::kMars)] = mars_r_table,
        [static_cast<int>(Planet::kJupiter)] = jupiter_r_table,
        [static_cast<int>(Planet::kSaturn)] = saturn_r_table,
        [static_cast<int>(Planet::kUranus)] = uranus_r_table,
        [static_cast<int>(Planet::kNeptune)] = neptune_r_table,
    };

    constexpr void ComputePlanetPosition(Planet planet) noexcept;

    bool planet_position_valid_[8]{false};
    Radian planet_longitude_[8]{Radian(0.0), Radian(0.0), Radian(0.0),
                                Radian(0.0), Radian(0.0), Radian(0.0),
                                Radian(0.0), Radian(0.0)};
    Radian planet_latitude_[8]{Radian(0.0), Radian(0.0), Radian(0.0),
                               Radian(0.0), Radian(0.0), Radian(0.0),
                               Radian(0.0), Radian(0.0)};
    double planet_radius_vector_au_[8]{0.0};

    static inline void ProcessDataFiles();
};

constexpr void VSOP87::ComputePlanetPosition(Planet planet) noexcept
{
    // References:
    // - [Jean99] Chapter 32 (Positions of Planets)
    // - [Jean99] Chapter 25 (Solar Coordinates)
    // - [Jean99] Appendix III (Planets: Periodic Terms)
    // - ftp://ftp.imcce.fr/pub/ephem/planets/vsop87
    // - http://neoprogrammics.com/vsop87/

    double t{(julian_date_ - EpochJ2000) / 36525.0};
    double tau{t / 10.0};
    double longitude{0.0};
    double latitude{0.0};
    double radius_vector_au{0.0};

    longitude = PeriodicTermCompute(
        periodic_term_l_tables[static_cast<int>(planet)], tau);
    latitude = PeriodicTermCompute(
        periodic_term_b_tables[static_cast<int>(planet)], tau);
    radius_vector_au = PeriodicTermCompute(
        periodic_term_r_tables[static_cast<int>(planet)], tau);

    // Conversion of reference frame to FK5
    // [Jean99] p.219
    double lp{longitude +
              (-Radian(1.397_deg).Rad() - Radian(0.00031_deg).Rad() * t) * t};
    double diff_l{Radian(-0.09033_arcsec).Rad() +
                  Radian(0.03916_arcsec).Rad() * (std::cos(lp) + std::sin(lp)) *
                      std::tan(latitude)};
    double diff_b{Radian(0.03916_arcsec).Rad() * (std::cos(lp) - std::sin(lp))};

    longitude += diff_l;
    latitude += diff_b;

    planet_longitude_[static_cast<int>(planet)] = Radian(longitude).GetUnwind();
    planet_latitude_[static_cast<int>(planet)] =
        Radian(latitude).GetNormalize();
    planet_radius_vector_au_[static_cast<int>(planet)] = radius_vector_au;

    planet_position_valid_[static_cast<int>(planet)] = true;
}

constexpr Radian VSOP87::GetPlanetLongitude(Planet planet) noexcept
{
    if(!planet_position_valid_[static_cast<int>(planet)]) {
        ComputePlanetPosition(planet);
    }
    return planet_longitude_[static_cast<int>(planet)];
}

constexpr Radian VSOP87::GetPlanetLatitude(Planet planet) noexcept
{
    if(!planet_position_valid_[static_cast<int>(planet)]) {
        ComputePlanetPosition(planet);
    }
    return planet_latitude_[static_cast<int>(planet)];
}

constexpr double VSOP87::GetPlanetRadiusVectorAU(Planet planet) noexcept
{
    if(!planet_position_valid_[static_cast<int>(planet)]) {
        ComputePlanetPosition(planet);
    }
    return planet_radius_vector_au_[static_cast<int>(planet)];
}

}  // namespace PA

#endif  // VSOP87_H_
