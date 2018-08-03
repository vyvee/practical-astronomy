#include "sun.h"

// Only for debugging
// #define DEBUG_SUN_PERIGEE_LONGITUDE_USE_J2010
// #define DEBUG_SUN_ECCENTRICITY_USE_J2010
// #define DEBUG_SUN_TRUE_ANOMALY_SINGLE_STEP

#include "angle.h"
#include "coordinate.h"
#include "date.h"
#include "solver.h"

using namespace PA;

Sun::Sun(const Date& date)
{
    julian_date_ = date.GetJulianDate();
}

#if 0
Degree Sun::GetMeanEclipticLongitude() const
{
    if(!ecliptic_longitude_mean_is_valid_) {
        ComputeMeanEclipticLongitude();
    }
    return ecliptic_longitude_mean_;
}

Degree Sun::GetPerigeeLongitude() const
{
    if(!perigee_longitude_is_valid_) {
        ComputePerigeeLongitude();
    }
    return perigee_longitude_;
}

double Sun::GetEccentricity() const
{
    if(!eccentricity_is_valid_) {
        ComputeEccentricity();
    }
    return eccentricity_;
}
#endif

#if 0
void Sun::ComputeMeanEclipticLongitude() const
{
    double t{(Date::J2010 - Date::J1900_JAN_0_5) / 36525.0};
    ecliptic_longitude_mean_ =
        279.6966778_deg + (36000.76892_deg + 0.0003025_deg * t) * t;
    ecliptic_longitude_mean_is_valid_ = true;
}

void Sun::ComputePerigeeLongitude() const
{
#ifdef DEBUG_SUN_PERIGEE_LONGITUDE_USE_J2010
    double t{(Date::J2010 - Date::J1900_JAN_0_5) / 36525.0};
#else
    double t{(julian_date_ - Date::J1900_JAN_0_5) / 36525.0};
#endif
    perigee_longitude_ =
        281.2208444_deg + (1.719175_deg + 0.000452778_deg * t) * t;
    perigee_longitude_is_valid_ = true;
}

void Sun::ComputeEccentricity() const
{
#ifdef DEBUG_SUN_ECCENTRICITY_USE_J2010
    double t{(Date::J2010 - Date::J1900_JAN_0_5) / 36525.0};
#else
    double t{(julian_date_ - Date::J1900_JAN_0_5) / 36525.0};
#endif
    eccentricity_ = 0.01675104 + (-0.0000418 - 0.000000126 * t) * t;
    eccentricity_is_valid_ = true;
}
#endif

#define DEBUG

#if 0
Coordinate Sun::GetPosition() const
{
    // Reference: [Peter11] Section 46, p.103
    double d{julian_date_ - Date::J2010};
    Degree n{d * 360.0 / 365.242191};
    Degree epsilon_g{GetMeanEclipticLongitude()};
    Degree omega_g{GetPerigeeLongitude()};
    Degree mean_anomaly{n + epsilon_g - omega_g};
    double ecc{GetEccentricity()};
#ifdef DEBUG
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "D (days): " << d << std::endl;
    std::cout << " N (deg): " << n.GetUnwind().DegStr() << std::endl;
    std::cout << "eg (deg): " << epsilon_g.GetUnwind().DegStr() << std::endl;
    std::cout << "    N+eg: " << (n + epsilon_g).GetUnwind().DegStr()
              << std::endl;
    std::cout << "og (deg): " << omega_g.GetUnwind().DegStr() << std::endl;
    std::cout << " M (deg): " << mean_anomaly.GetUnwind().DegStr() << std::endl;
    std::cout << "     ecc: " << ecc << std::endl;
#endif

#ifdef DEBUG_SUN_TRUE_ANOMALY_SINGLE_STEP
    Degree e_c{360.0 / M_PI * ecc * sin(mean_anomaly)};
    Degree l_s{n + e_c + epsilon_g};
#ifdef DEBUG
    std::cout << "Ec (deg): " << e_c.GetUnwind().DegStr() << std::endl;
    std::cout << "Ls (deg): " << l_s.GetUnwind().DegStr() << std::endl;
#endif

#else
#ifdef DEBUG
    std::cout << " M (rad): " << mean_anomaly.GetUnwind().Rad() << std::endl;
#endif
    Degree e{Solver::solve_kepler(ecc, mean_anomaly)};
    Degree v{std::atan(std::sqrt((1 + ecc) / (1 - ecc)) * tan(e / 2.0)) * 2.0};
    Degree l_s{v + omega_g};
#ifdef DEBUG
    std::cout << "----------" << std::endl;
    std::cout << "tan(v/2): " << std::sqrt((1 + ecc) / (1 - ecc)) * tan(e / 2.0)
              << std::endl;
    std::cout << " v (deg): " << v.GetUnwind().DegStr() << std::endl;
    std::cout << "Ls (deg): " << l_s.GetUnwind().DegStr() << std::endl;
#endif
#endif

    Coordinate coord;
    coord.SetEcliptic(l_s, Degree(0.0), julian_date_);
    return coord;
}
#endif
