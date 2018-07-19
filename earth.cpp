#include "earth.h"

#include "angle.h"
#include "date.h"

// Only for debugging
// #define DEBUG_EARTH_OBLIQUITY_EXCLUDE_NUTATION

using namespace PA;

Earth::Earth(double jd) : julian_date_(jd)
{
}

Earth::Earth(const Date& date)
{
    date.GetJulianDate(&julian_date_);
}

#include <iostream>

void Earth::ComputeNutation() const
{
    // Reference: [Peter11] Section 35, p.76
    // ?? May improve accuracy, as implemented by the NutatObl() macro
    double t{(julian_date_ - PA::Date::J1900_JAN_0_5) / 36525.0};
    double a{100.0021358 * t};
    Angle l1{279.6967_deg + 0.000303_deg * t * t};
    Angle l{l1 + 2 * M_PI * (a - floor(a))};
    double b{5.372617 * t};
    Angle n{259.1833_deg - 2 * M_PI * (b - floor(b))};
    nutation_longitude_ = -17.2_arcsec * sin(n) - 1.3_arcsec * sin(2 * l);
    nutation_obliquity_ = 9.2_arcsec * cos(n) + 0.5_arcsec * cos(2 * l);
    nutation_is_valid_ = true;
#ifdef DEBUG
    std::cout << "Earth::ComputeNutation():" << std::endl;
    std::cout << std::setprecision(12);
    std::cout << "       T: " << t << std::endl;
    std::cout << "       A: " << a << std::endl;
    std::cout << "L1 (deg): " << deg_turn(rad_to_deg(l1)) << std::endl;
    std::cout << " L (deg): " << deg_turn(rad_to_deg(l)) << std::endl;
    std::cout << "       B: " << b << std::endl;
    std::cout << " N (deg): " << deg_turn(rad_to_deg(n)) << std::endl;
    std::cout << "  NL (\"): " << nutation_longitude_.arcsec() << std::endl;
    std::cout << "  NO (\"): " << nutation_obliquity_.arcsec() << std::endl;
#endif
}

void Earth::ComputeObliquityMean() const
{
    // Reference: [Peter11] Section 27
    double t{(julian_date_ - PA::Date::J2000_JAN_1_5) / 36525.0};
    obliquity_mean_ =
        -((46.815_arcsec + (0.0006_arcsec - 0.00181_arcsec * t) * t) * t);
#ifdef DEBUG
    std::cout << "Earth::GetObliquity():" << std::endl;
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "      JD: " << julian_date_ << std::endl;
    std::cout << "       T: " << t << std::endl;
    std::cout << "   DE(\"): " << -obliquity_mean.arcsec() << std::endl;
    std::cout << "   DE(\"): " << -obliquity_mean.deg() << std::endl;
#endif
    obliquity_mean_ += 23.0_deg + 26.0_arcmin + 21.45_arcsec;
#ifdef DEBUG
    std::cout << " epsilon: " << obliquity_mean.deg() << std::endl;
#endif
    obliquity_mean_is_valid_ = true;
}

Angle Earth::GetNutationLongitude() const
{
    if(!nutation_is_valid_) {
        ComputeNutation();
    }
    return nutation_longitude_;
}

Angle Earth::GetNutationObliquity() const
{
    if(!nutation_is_valid_) {
        ComputeNutation();
    }
    return nutation_obliquity_;
}

Angle Earth::GetObliquityMean() const
{
    if(!obliquity_mean_is_valid_) {
        ComputeObliquityMean();
    }
    return obliquity_mean_;
}

Angle Earth::GetObliquity() const
{
    if(!obliquity_mean_is_valid_) {
        ComputeObliquityMean();
    }
#ifdef DEBUG_EARTH_OBLIQUITY_EXCLUDE_NUTATION
    return obliquity_mean_;
#else
    if(!nutation_is_valid_) {
        ComputeNutation();
    }
    return obliquity_mean_ + nutation_obliquity_;
#endif
}