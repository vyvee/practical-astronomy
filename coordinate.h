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
    void SetEcliptic(const Angle& lon, const Angle& lat, double jd);

    // bool GetEquatorial(double* p_ra, double* p_decl) const;
    Angle GetEclipticLongitude() const;
    Angle GetEclipticLatitude() const;
    Angle GetEquatorialRightAscension() const;
    Angle GetEquatorialDeclination() const;

   private:
    double julian_date_;

    mutable bool ecliptic_is_valid_{false};
    mutable Angle ecliptic_longitude_;
    mutable Angle ecliptic_latitude_;

    bool ComputeEquatorial() const;
    mutable bool equatorial_is_valid_{false};
    mutable Angle equatorial_right_ascension_;
    mutable Angle equatorial_declination_;
};

}  // namespace PA

#endif  // COORDINATE_H_
