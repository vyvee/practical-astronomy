#ifndef COORDINATE_H_
#define COORDINATE_H_

#include "angle.h"
#include "earth.h"

namespace PA
{
class Coordinate
{
   public:
    constexpr Coordinate() noexcept
    {
    }
    constexpr void SetEcliptic(const Degree& lon,
                               const Degree& lat,
                               double jd) noexcept;

    constexpr Degree GetEclipticLongitude() noexcept;
    constexpr Degree GetEclipticLatitude() noexcept;
    constexpr Degree GetEquatorialRightAscension() noexcept;
    constexpr Degree GetEquatorialDeclination() noexcept;

   private:
    double julian_date_{0.0};

    bool ecliptic_is_valid_{false};
    Degree ecliptic_longitude_{0.0};
    Degree ecliptic_latitude_{0.0};

    constexpr bool ComputeEquatorial() noexcept;
    bool equatorial_is_valid_{false};
    Degree equatorial_right_ascension_{0.0};
    Degree equatorial_declination_{0.0};
};

constexpr void Coordinate::SetEcliptic(const Degree& lon,
                                       const Degree& lat,
                                       double jd) noexcept
{
    ecliptic_longitude_ = lon;
    ecliptic_latitude_ = lat;
    julian_date_ = jd;
    ecliptic_is_valid_ = true;
    equatorial_is_valid_ = false;
}

constexpr Degree Coordinate::GetEclipticLongitude() noexcept
{
    if(!ecliptic_is_valid_) {
        return Degree(0.0);
    }

    return ecliptic_longitude_;
}

constexpr Degree Coordinate::GetEclipticLatitude() noexcept
{
    if(!ecliptic_is_valid_) {
        return Degree(0.0);
    }

    return ecliptic_latitude_;
}

constexpr Degree Coordinate::GetEquatorialRightAscension() noexcept
{
    if(!equatorial_is_valid_) {
        if(!ComputeEquatorial()) {
            return Degree(0.0);
        }
    }

    return equatorial_right_ascension_;
}

constexpr Degree Coordinate::GetEquatorialDeclination() noexcept
{
    if(!equatorial_is_valid_) {
        if(!ComputeEquatorial()) {
            return Degree(0.0);
        }
    }

    return equatorial_declination_;
}

constexpr bool Coordinate::ComputeEquatorial() noexcept
{
    if(ecliptic_is_valid_) {
        // Reference: [Peter11] Section 27, p.51.
        Earth earth{julian_date_};
        Degree obliq{earth.GetObliquity()};
        double sin_delta{sin(ecliptic_latitude_) * cos(obliq) +
                         cos(ecliptic_latitude_) * sin(obliq) *
                             sin(ecliptic_longitude_)};
        equatorial_declination_ = Degree(Radian(asin(sin_delta)).Deg());
        double y{sin(ecliptic_longitude_) * cos(obliq) -
                 tan(ecliptic_latitude_) * sin(obliq)};
        double x{cos(ecliptic_longitude_)};
        equatorial_right_ascension_ =
            Degree(Radian(atan2(y, x)).Deg()).GetUnwind();
        equatorial_is_valid_ = true;
#ifdef DEBUG
        std::cout << "Coordinate::ComputeEquatorial():" << std::endl;
        std::cout << std::setprecision(12);
        std::cout << "    lon.: " << ecliptic_longitude_.Deg() << std::endl;
        std::cout << "    lat.: " << ecliptic_latitude_.Deg() << std::endl;
        std::cout << "  sin(d): " << sin_delta << std::endl;
        std::cout << "   decl.: " << equatorial_declination_.Deg() << std::endl;
        std::cout << "       y: " << y << std::endl;
        std::cout << "       x: " << x << std::endl;
        std::cout << "     ra.: " << equatorial_right_ascension_.Deg()
                  << std::endl;
#endif
        return true;
    }

    return false;
}

}  // namespace PA

#endif  // COORDINATE_H_
