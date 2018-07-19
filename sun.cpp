#include "sun.h"

// Only for debugging
// #define DEBUG_SUN_PERIGEE_LONGITUDE_USE_J2010
// #define DEBUG_SUN_ECCENTRICITY_USE_J2010

#include "angle.h"
#include "coordinate.h"

using namespace PA;

Sun::Sun(const Date& date)
{
    julian_date_ = date.GetJulianDate();
}

Sun::~Sun()
{
}

Angle Sun::GetMeanEclipticLongitude() const
{
    if(!ecliptic_longitude_mean_is_valid_) {
        ComputeMeanEclipticLongitude();
    }
    return ecliptic_longitude_mean_;
}

Angle Sun::GetPerigeeLongitude() const
{
    if(!perigee_longitude_is_valid_) {
        ComputePerigeeLongitude();
    }
    return perigee_longitude_;
}

Angle Sun::GetEccentricity() const
{
    if(!eccentricity_is_valid_) {
        ComputeEccentricity();
    }
    return eccentricity_;
}

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
    eccentricity_ = 0.01675104 - 0.0000418 * t - 0.000000126 * t * t;
    eccentricity_is_valid_ = true;
}

Coordinate Sun::GetPosition()
{
    // Reference: [Peter11] Section 46, p.103
    double d = julian_date_ - Date::J2010;
    Angle n{d * 2 * M_PI / 365.242191};
    Angle epsilon_g{GetMeanEclipticLongitude()};
    Angle omega_g{GetPerigeeLongitude()};
    Angle mean_anomaly = n + epsilon_g.turn() - omega_g.turn();
    double ecc{GetEccentricity()};
    Angle e_c = 2 * ecc * sin(mean_anomaly);
    Angle l_s = n + e_c + epsilon_g;
    Coordinate coord;
    coord.SetEcliptic(l_s, 0.0, julian_date_);
#ifdef DEBUG
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "D (days): " << d << std::endl;
    std::cout << " N (deg): " << n.deg() << std::endl;
    std::cout << " e (deg): " << epsilon_g.turn().deg() << std::endl;
    std::cout << " o (deg): " << omega_g.turn().deg() << std::endl;
    std::cout << " M (deg): " << mean_anomaly.deg() << std::endl;
    std::cout << "Ec (deg): " << e_c.turn().deg() << std::endl;
    std::cout << "Ls (deg): " << l_s.turn().deg() << std::endl;
#endif
    return coord;
}
