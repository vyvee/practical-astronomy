#ifndef EARTH_H_
#define EARTH_H_

#include "angle.h"
#include "date.h"

namespace PA
{
class Earth
{
   public:
    Earth(double jd);
    Earth(const Date& date);
    ~Earth(){};

    Angle GetObliquity() const;

    Angle GetObliquityMean() const;
    Angle GetNutationLongitude() const;
    Angle GetNutationObliquity() const;

   private:
    double julian_date_;

    void ComputeObliquityMean() const;
    mutable bool obliquity_mean_is_valid_{false};
    mutable Angle obliquity_mean_;

    void ComputeNutation() const;
    mutable bool nutation_is_valid_{false};
    mutable Angle nutation_longitude_;
    mutable Angle nutation_obliquity_;

    mutable bool obliquity_is_valid_{false};
    mutable Angle obliquity_;
};

}  // namespace PA

#endif  // EARTH_H_
