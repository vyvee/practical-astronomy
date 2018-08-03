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

    Degree GetObliquity();
    Degree GetObliquityMean();
    Degree GetNutationLongitude();
    Degree GetNutationObliquity();

   private:
    Date date_;

    void ComputeObliquityMean();
    bool obliquity_mean_is_valid_{false};
    Degree obliquity_mean_{0.0};

    void ComputeNutation();
    bool nutation_is_valid_{false};
    Degree nutation_longitude_{0.0};
    Degree nutation_obliquity_{0.0};

    mutable bool obliquity_is_valid_{false};
    mutable Degree obliquity_{0.0};
};

}  // namespace PA

#endif  // EARTH_H_
