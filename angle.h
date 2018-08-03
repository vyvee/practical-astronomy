#ifndef ANGLE_H_
#define ANGLE_H_

#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

namespace PA
{
class Degree;

class Radian
{
   public:
    constexpr explicit Radian(double rad) : rad_(rad)
    {
    }

    constexpr Radian(const Degree &deg);

    constexpr double Rad() const noexcept
    {
        return rad_;
    }

    constexpr double Deg() const noexcept
    {
        return rad_ * 180.0 / M_PI;
    }

    constexpr Radian operator-() const noexcept
    {
        return Radian(-rad_);
    }

    constexpr Radian &operator+=(const Radian &rhs) noexcept
    {
        rad_ += rhs.rad_;
        return *this;
    }

    constexpr Radian &operator-=(const Radian &rhs) noexcept
    {
        rad_ -= rhs.rad_;
        return *this;
    }

    constexpr Radian &operator*=(double multiplier) noexcept
    {
        rad_ *= multiplier;
        return *this;
    }

    constexpr Radian &operator/=(double divisor) noexcept
    {
        rad_ /= divisor;
        return *this;
    }

    constexpr Radian operator+(const Radian &rhs) noexcept
    {
        return Radian(rad_ + rhs.rad_);
    }

    constexpr Radian operator-(const Radian &rhs) noexcept
    {
        return Radian(rad_ - rhs.rad_);
    }

    constexpr Radian operator*(double multiplier) noexcept
    {
        return Radian(rad_ * multiplier);
    }

   private:
    double rad_;
};

class Degree
{
   public:
    constexpr explicit Degree(double deg) : deg_(deg)
    {
    }

    constexpr Degree(const Radian &rad);

    constexpr double Deg() const noexcept
    {
        return deg_;
    }

    constexpr double ArcMin() const noexcept
    {
        return deg_ * 60.0;
    }

    constexpr double ArcSec() const noexcept
    {
        return deg_ * 3600.0;
    }

    constexpr double Hour() const noexcept
    {
        return deg_ / 15.0;
    }

    constexpr double Rad() const noexcept
    {
        return deg_ * M_PI / 180.0;
    }

    constexpr void Unwind() noexcept
    {
        deg_ -= 360.0 * floor(deg_ / 360.0);
    }

    constexpr Degree GetUnwind() const noexcept
    {
        return Degree(deg_ - 360.0 * floor(deg_ / 360.0));
    }

    constexpr Degree operator-() const noexcept
    {
        return Degree(-deg_);
    }

    constexpr Degree &operator+=(const Degree &rhs) noexcept
    {
        deg_ += rhs.deg_;
        return *this;
    }

    constexpr Degree &operator-=(const Degree &rhs) noexcept
    {
        deg_ -= rhs.deg_;
        return *this;
    }

    constexpr Degree &operator*=(double multiplier) noexcept
    {
        deg_ *= multiplier;
        return *this;
    }

    constexpr Degree &operator/=(double divisor) noexcept
    {
        deg_ /= divisor;
        return *this;
    }

    constexpr Degree operator+(const Degree &rhs) noexcept
    {
        return Degree(deg_ + rhs.deg_);
    }

    constexpr Degree operator-(const Degree &rhs) noexcept
    {
        return Degree(deg_ - rhs.deg_);
    }

    constexpr Degree operator*(double multiplier) noexcept
    {
        return Degree(deg_ * multiplier);
    }

    constexpr Degree operator/(double divisor) noexcept
    {
        return Degree(deg_ / divisor);
    }

    std::string DegStr(int s_prec = 6) const noexcept
    {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(s_prec) << Deg() << '\xb0';
        return ss.str();
    }

    std::string DMSStr(int s_prec = 2) const noexcept
    {
        std::ostringstream ss;
        double d, m, s;
        s = 60.0 * modf(60.0 * modf(Deg(), &d), &m);
        ss << std::setw(3) << d << '\xb0';
        ss << std::setw(2) << fabs(m) << '\'';
        ss << std::fixed << std::setw(s_prec + 3) << std::setprecision(s_prec)
           << fabs(s) << '"';
        return ss.str();
    }

    std::string HourStr(int s_prec = 6) const noexcept
    {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(s_prec) << Hour() << 'h';
        return ss.str();
    }

    std::string HMSStr(int s_prec = 2) const noexcept
    {
        std::ostringstream ss;
        double h, m, s;
        s = 60.0 * modf(60.0 * modf(Hour(), &h), &m);
        ss << std::setw(2) << h << 'h';
        ss << std::setw(2) << m << 'm';
        ss << std::fixed << std::setw(s_prec + 3) << std::setprecision(s_prec)
           << s << 's';
        return ss.str();
    }

   private:
    double deg_;
};

constexpr Degree operator"" _deg(long double d)
{
    return Degree(d);
}

constexpr Degree operator"" _arcmin(long double m)
{
    return Degree(m / 60.0);
}

constexpr Degree operator"" _arcsec(long double s)
{
    return Degree(s / 3600.0);
}

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

constexpr double sin(const Degree &x)
{
    return std::sin(x.Rad());
}

constexpr double cos(const Degree &x)
{
    return std::cos(x.Rad());
}

constexpr double tan(const Degree &x)
{
    return std::tan(x.Rad());
}

constexpr Radian::Radian(const Degree &deg) : rad_(deg.Deg() * M_PI / 180.0)
{
}

constexpr Degree::Degree(const Radian &rad) : deg_(rad.Rad() * 180.0 / M_PI)
{
}

}  // namespace PA

#endif  // ANGLE_H_
