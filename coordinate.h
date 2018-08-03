#ifndef COORDINATE_H_
#define COORDINATE_H_

#include "angle.h"

namespace PA
{
class Coordinate
{
   public:
    Coordinate();
    ~Coordinate();
    void SetEcliptic(const Degree& lon, const Degree& lat, double jd);

    // bool GetEquatorial(double* p_ra, double* p_decl) const;
    Degree GetEclipticLongitude() const;
    Degree GetEclipticLatitude() const;
    Degree GetEquatorialRightAscension() const;
    Degree GetEquatorialDeclination() const;

   private:
    double julian_date_;

    mutable bool ecliptic_is_valid_{false};
    mutable Degree ecliptic_longitude_{0.0};
    mutable Degree ecliptic_latitude_{0.0};

    bool ComputeEquatorial() const;
    mutable bool equatorial_is_valid_{false};
    mutable Degree equatorial_right_ascension_{0.0};
    mutable Degree equatorial_declination_{0.0};
};

}  // namespace PA

#endif  // COORDINATE_H_
