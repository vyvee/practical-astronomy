#include "coordinate.h"

#include "angle.h"
#include "earth.h"

#include <cmath>

#define VERBOSE

#ifdef VERBOSE
#include <iomanip>
#include <iostream>
#endif

using namespace PA;

Coordinate::Coordinate()
{
}

Coordinate::~Coordinate()
{
}

void Coordinate::SetEcliptic(const Degree& lon, const Degree& lat, double jd)
{
    ecliptic_longitude_ = lon;
    ecliptic_latitude_ = lat;
    julian_date_ = jd;
    ecliptic_is_valid_ = true;
    equatorial_is_valid_ = false;
}

Degree Coordinate::GetEclipticLongitude() const
{
    if(!ecliptic_is_valid_) {
        return Degree(0.0);
    }

    return ecliptic_longitude_;
}

Degree Coordinate::GetEclipticLatitude() const
{
    if(!ecliptic_is_valid_) {
        return Degree(0.0);
    }

    return ecliptic_latitude_;
}

Degree Coordinate::GetEquatorialRightAscension() const
{
    if(!equatorial_is_valid_) {
        if(!ComputeEquatorial()) {
            return Degree(0.0);
        }
    }

    return equatorial_right_ascension_;
}

Degree Coordinate::GetEquatorialDeclination() const
{
    if(!equatorial_is_valid_) {
        if(!ComputeEquatorial()) {
            return Degree(0.0);
        }
    }

    return equatorial_declination_;
}

bool Coordinate::ComputeEquatorial() const
{
    if(ecliptic_is_valid_) {
        // Reference: [Peter11] Section 27, p.51.
        Earth earth(julian_date_);
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
