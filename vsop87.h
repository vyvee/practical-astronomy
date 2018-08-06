#ifndef VSOP87_H_
#define VSOP87_H_

#include <cmath>

#include "angle.h"

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
    constexpr double GetPlanetDistanceAU(Planet planet) noexcept;

   private:
    double julian_date_;

    struct PeriodicTerm {
        double a;
        double b;
        double c;
    };

    struct PeriodicTermTable {
        const PeriodicTerm *terms;
        int size;
    };

#include "vsop87_internal.dat"

    constexpr void ComputePlanetPosition(Planet planet) noexcept;
    template <int N>
    static constexpr double PeriodicTermCompute(
        const PeriodicTermTable (&periodic_term_table)[N],
        double tau) noexcept;

    bool planet_position_valid_[8]{false};
    Radian planet_longitude_[8]{Radian(0.0), Radian(0.0), Radian(0.0),
                                Radian(0.0), Radian(0.0), Radian(0.0),
                                Radian(0.0), Radian(0.0)};
    Radian planet_latitude_[8]{Radian(0.0), Radian(0.0), Radian(0.0),
                               Radian(0.0), Radian(0.0), Radian(0.0),
                               Radian(0.0), Radian(0.0)};
    double planet_distance_au_[8]{0.0};

    static inline void ProcessDataFiles();
};

template <int N>
constexpr double VSOP87::PeriodicTermCompute(
    const PeriodicTermTable (&periodic_term_table)[N],
    double tau) noexcept
{
    double values[N]{0.0};
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < periodic_term_table[i].size; j++) {
            const PeriodicTerm &pt{periodic_term_table[i].terms[j]};
            values[i] += pt.a * std::cos(pt.b + pt.c * tau);
        }
    }
    return horner_polynomial(values, tau);
}

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
    double distance_au{0.0};

    switch(planet) {
    case Planet::kMercury:
        longitude = PeriodicTermCompute(mercury_l_table, tau);
        latitude = PeriodicTermCompute(mercury_b_table, tau);
        distance_au = PeriodicTermCompute(mercury_r_table, tau);
        break;

    case Planet::kVenus:
        longitude = PeriodicTermCompute(venus_l_table, tau);
        latitude = PeriodicTermCompute(venus_b_table, tau);
        distance_au = PeriodicTermCompute(venus_r_table, tau);
        break;

    case Planet::kEarth:
        longitude = PeriodicTermCompute(earth_l_table, tau);
        latitude = PeriodicTermCompute(earth_b_table, tau);
        distance_au = PeriodicTermCompute(earth_r_table, tau);
        break;

    case Planet::kMars:
        longitude = PeriodicTermCompute(mars_l_table, tau);
        latitude = PeriodicTermCompute(mars_b_table, tau);
        distance_au = PeriodicTermCompute(mars_r_table, tau);
        break;

    case Planet::kJupiter:
        longitude = PeriodicTermCompute(jupiter_l_table, tau);
        latitude = PeriodicTermCompute(jupiter_b_table, tau);
        distance_au = PeriodicTermCompute(jupiter_r_table, tau);
        break;

    case Planet::kSaturn:
        longitude = PeriodicTermCompute(saturn_l_table, tau);
        latitude = PeriodicTermCompute(saturn_b_table, tau);
        distance_au = PeriodicTermCompute(saturn_r_table, tau);
        break;

    case Planet::kUranus:
        longitude = PeriodicTermCompute(uranus_l_table, tau);
        latitude = PeriodicTermCompute(uranus_b_table, tau);
        distance_au = PeriodicTermCompute(uranus_r_table, tau);
        break;

    case Planet::kNeptune:
        longitude = PeriodicTermCompute(neptune_l_table, tau);
        latitude = PeriodicTermCompute(neptune_b_table, tau);
        distance_au = PeriodicTermCompute(neptune_r_table, tau);
        break;
    }

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
    planet_distance_au_[static_cast<int>(planet)] = distance_au;

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

constexpr double VSOP87::GetPlanetDistanceAU(Planet planet) noexcept
{
    if(!planet_position_valid_[static_cast<int>(planet)]) {
        ComputePlanetPosition(planet);
    }
    return planet_distance_au_[static_cast<int>(planet)];
}

}  // namespace PA

#endif  // VSOP87_H_
