#ifndef SUN_H_
#define SUN_H_

#include "angle.h"
#include "coordinate.h"
#include "date.h"

namespace PA
{
class Sun
{
   public:
    Sun(const Date& date);
    ~Sun();
    Coordinate GetPosition();

    Angle GetMeanEclipticLongitude() const;
    Angle GetPerigeeLongitude() const;
    Angle GetEccentricity() const;

   private:
    double julian_date_;

    // ??? Need to specify which epoch this is for??
    void ComputeMeanEclipticLongitude() const;
    mutable bool ecliptic_longitude_mean_is_valid_;
    mutable Angle ecliptic_longitude_mean_;

    void ComputePerigeeLongitude() const;
    mutable bool perigee_longitude_is_valid_;
    mutable Angle perigee_longitude_;

    void ComputeEccentricity() const;
    mutable bool eccentricity_is_valid_;
    mutable double eccentricity_;
};

}  // namespace PA

#endif  // SUN_H_