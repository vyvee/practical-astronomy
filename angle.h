#ifndef ANGLE_H_
#define ANGLE_H_

#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

namespace PA
{
class Angle
{
   public:
    constexpr Angle() : rad_{0.0}
    {
    }
    constexpr Angle(const Angle &angle) : rad_{angle.rad_} {};
    constexpr Angle(double rad) : rad_{rad}
    {
    }
    constexpr Angle(long double rad) : rad_{static_cast<double>(rad)}
    {
    }
    constexpr Angle(int rad) : rad_{static_cast<double>(rad)}
    {
    }

    constexpr Angle turn() const noexcept
    {
        return rad_ - 2 * M_PI * floor(rad_ / (2 * M_PI));
    }

    constexpr explicit operator double()
    {
        return rad_;
    }

    Angle &operator=(Angle that)
    {
        std::swap(rad_, that.rad_);
        return *this;
    }

    constexpr Angle operator-() const
    {
        return Angle{-rad()};
    }

    constexpr Angle &operator+=(const Angle &that)
    {
        rad_ += that.rad_;
        return *this;
    }

    constexpr Angle &operator-=(const Angle &that)
    {
        rad_ -= that.rad_;
        return *this;
    }

    constexpr Angle &operator*=(double that)
    {
        rad_ *= that;
        return *this;
    }

    constexpr Angle &operator/=(double that)
    {
        rad_ /= that;
        return *this;
    }

    friend constexpr Angle operator*(const Angle &x, const double y);
    friend constexpr Angle operator*(const double x, const Angle &y);
    friend constexpr Angle operator/(const Angle &x, const double y);

    constexpr double rad() const noexcept
    {
        return rad_;
    }

    constexpr double deg() const noexcept
    {
        return rad_ * 180.0 / M_PI;
    }

    constexpr double arcsec() const noexcept
    {
        return rad_ * 180.0 * 3600.0 / M_PI;
    }

    std::string dms_str(int s_prec = 2) const noexcept
    {
        std::ostringstream ss;
        double d, m, s;
        s = 60.0 * modf(60.0 * modf(deg(), &d), &m);
        ss << std::setw(4) << d << '\xb0';
        ss << std::setw(2) << fabs(m) << '\'';
        ss << std::fixed << std::setw(s_prec + 3) << std::setprecision(s_prec)
           << fabs(s) << '"';
        return ss.str();
    }

    std::string hms_str(int s_prec = 2) const noexcept
    {
        std::ostringstream ss;
        double h, m, s;
        s = 60.0 * modf(60.0 * modf(deg() / 15.0, &h), &m);
        ss << std::setw(2) << h << 'h';
        ss << std::setw(2) << m << 'm';
        ss << std::fixed << std::setw(s_prec + 3) << std::setprecision(s_prec)
           << s << 's';
        return ss.str();
    }

   private:
    double rad_;
};

constexpr Angle operator"" _rad(long double rad)
{
    return Angle(rad);
}

constexpr Angle operator"" _rad(unsigned long long rad)
{
    return Angle(static_cast<double>(rad));
}

constexpr Angle operator"" _deg(long double d)
{
    return Angle(d * M_PI / 180.0);
}

constexpr Angle operator"" _arcmin(long double m)
{
    return Angle(m * M_PI / (180.0 * 60.0));
}

constexpr Angle operator"" _arcsec(long double s)
{
    return Angle(s * M_PI / (180.0 * 3600.0));
}

constexpr Angle operator"" _hour(long double h)
{
    return Angle(h * M_PI / 12.0);
}

constexpr Angle operator"" _minute(long double m)
{
    return Angle(m * M_PI / (12.0 * 60.0));
}

constexpr Angle operator"" _second(long double s)
{
    return Angle(s * M_PI / (12.0 * 3600.0));
}

constexpr Angle operator"" _deg(unsigned long long deg)
{
    return Angle(static_cast<double>(deg) * M_PI / 180.0);
}

constexpr inline Angle operator+(const Angle &x, const Angle &y)
{
    Angle z(x);
    z += y;
    return z;
}

constexpr inline Angle operator-(const Angle &x, const Angle &y)
{
    Angle z(x);
    z -= y;
    return z;
}

constexpr inline Angle operator*(const Angle &x, const double y)
{
    return Angle{x.rad_ * y};
}

constexpr inline Angle operator*(const double x, const Angle &y)
{
    return Angle{x * y.rad_};
}

constexpr inline Angle operator/(const Angle &x, const double y)
{
    return Angle{x.rad_ / y};
}

constexpr inline double sin(const Angle &x)
{
    return std::sin(x.rad());
}

constexpr inline double cos(const Angle &x)
{
    return std::cos(x.rad());
}

constexpr inline double tan(const Angle &x)
{
    return std::tan(x.rad());
}

}  // namespace PA

#endif  // ANGLE_H_
