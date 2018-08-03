#ifndef EARTH_H_
#define EARTH_H_

#include "angle.h"
#include "date.h"

namespace PA
{
class Earth
{
   public:
    constexpr Earth(double jd) : date_(Date(jd))
    {
    }
    constexpr Earth(const Date& date) : date_(date)
    {
    }
    ~Earth(){};

    Degree GetObliquity() const;
    Degree GetObliquityMean() const;
    Degree GetNutationLongitude() const;
    Degree GetNutationObliquity() const;

   private:
    Date date_;

    void ComputeObliquityMean() const;
    mutable bool obliquity_mean_is_valid_{false};
    mutable Degree obliquity_mean_{0.0};

    void ComputeNutation() const;
    mutable bool nutation_is_valid_{false};
    mutable Degree nutation_longitude_{0.0};
    mutable Degree nutation_obliquity_{0.0};

    mutable bool obliquity_is_valid_{false};
    mutable Degree obliquity_{0.0};
};

}  // namespace PA

#endif  // EARTH_H_
