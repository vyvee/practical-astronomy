#ifndef ANGLE_H_
#define ANGLE_H_

#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

namespace PA
{
constexpr double operator"" _deg(long double deg)
{
    return deg * M_PI / 180.0;
}

constexpr double operator"" _deg(unsigned long long int deg)
{
    return deg * M_PI / 180.0;
}

constexpr double operator"" _arcmin(long double arcmin)
{
    return arcmin * M_PI / (180.0 * 60.0);
}

constexpr double operator"" _arcmin(unsigned long long int arcmin)
{
    return arcmin * M_PI / (180.0 * 60.0);
}

constexpr double operator"" _arcsec(long double arcsec)
{
    return arcsec * M_PI / (180.0 * 3600.0);
}

constexpr double operator"" _arcsec(unsigned long long int arcsec)
{
    return arcsec * M_PI / (180.0 * 3600.0);
}

/*
constexpr Degree operator"" _hour(long double h)
{
    return Degree(h * 15.0);
}

constexpr Degree operator"" _minute(long double m)
{
    return Degree(m * 15.0 / 60.0);
}

constexpr Degree operator"" _second(long double s)
{
    return Degree(s * 15.0 / 3600.0);
}
*/

constexpr double RadUnwind(double rad) noexcept
{
    return rad - 2 * M_PI * floor(rad / (2 * M_PI));
}

constexpr double RadNormalize(double rad) noexcept
{
    return rad - 2 * M_PI * floor((rad + M_PI) / (2 * M_PI));
}

constexpr double RadToDeg(double rad) noexcept
{
    return rad * 180.0 / M_PI;
}

constexpr double DegToRad(double deg) noexcept
{
    return deg * M_PI / 180.0;
}

constexpr double RadToArcMin(double rad) noexcept
{
    return rad * (180.0 * 60.0) / M_PI;
}

constexpr double RadToArcSec(double rad) noexcept
{
    return rad * (180.0 * 3600.0) / M_PI;
}

constexpr double RadToHour(double rad) noexcept
{
    return rad * (180.0 / 15.0) / M_PI;
}

inline std::string RadToDegStr(double rad, int s_prec = 6) noexcept
{
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(s_prec) << RadToDeg(rad) << '\xb0';
    return ss.str();
}

inline std::string RadToArcSecStr(double rad, int s_prec = 2) noexcept
{
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(s_prec) << RadToArcSec(rad) << '\"';
    return ss.str();
}

inline std::string RadToDMSStr(double rad, int s_prec = 2) noexcept
{
    std::ostringstream ss;
    double d, m, s;
    s = 60.0 * modf(60.0 * modf(RadToDeg(rad), &d), &m);
    ss << std::setw(3) << d << '\xb0';
    ss << std::setw(2) << fabs(m) << '\'';
    ss << std::fixed << std::setw(s_prec + 3) << std::setprecision(s_prec)
       << fabs(s) << '"';
    return ss.str();
}

inline std::string RadToHourStr(double rad, int s_prec = 6) noexcept
{
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(s_prec) << RadToHour(rad) << 'h';
    return ss.str();
}

inline std::string RadToHMSStr(double rad, int s_prec = 2) noexcept
{
    std::ostringstream ss;
    double h, m, s;
    s = 60.0 * modf(60.0 * modf(RadToHour(rad), &h), &m);
    ss << std::setw(2) << h << 'h';
    ss << std::setw(2) << m << 'm';
    ss << std::fixed << std::setw(s_prec + 3) << std::setprecision(s_prec) << s
       << 's';
    return ss.str();
}

}  // namespace PA

#endif  // ANGLE_H_
