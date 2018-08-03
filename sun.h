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
    // Coordinate GetPosition() const;

    // Degree GetMeanEclipticLongitude() const;
    // Degree GetPerigeeLongitude() const;
    // double GetEccentricity() const;

   private:
    double julian_date_;

    // ??? Need to specify which epoch this is for??
    void ComputeMeanEclipticLongitude() const;
    mutable bool ecliptic_longitude_mean_is_valid_{false};
    mutable Degree ecliptic_longitude_mean_{0.0};

    void ComputePerigeeLongitude() const;
    mutable bool perigee_longitude_is_valid_{false};
    mutable Degree perigee_longitude_{0.0};

    void ComputeEccentricity() const;
    mutable bool eccentricity_is_valid_{false};
    mutable double eccentricity_{0.0};
};

}  // namespace PA

#endif  // SUN_H_
