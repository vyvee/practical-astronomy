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

    static constexpr double EclipticalToEquatorialRightAscension(
        double lon,
        double lat,
        double obliquity) noexcept;
    static constexpr double EclipticalToEquatorialDeclination(
        double lon,
        double lat,
        double obliquity) noexcept;

   private:
    double julian_date_{0.0};
};

constexpr double Coordinate::EclipticalToEquatorialRightAscension(
    const double lon,
    const double lat,
    const double obliquity) noexcept
{
    return Radian(std::atan2(std::sin(lon) * std::cos(obliquity) -
                                 std::tan(lat) * std::sin(obliquity),
                             std::cos(lon)))
        .GetUnwind()
        .Rad();
}

constexpr double Coordinate::EclipticalToEquatorialDeclination(
    const double lon,
    const double lat,
    const double obliquity) noexcept
{
    return std::asin(std::sin(lat) * std::cos(obliquity) +
                     std::cos(lat) * std::sin(obliquity) * std::sin(lon));
}

}  // namespace PA

#endif  // COORDINATE_H_
