#ifndef MISC_H_
#define MISC_H_

#include <cmath>

#include "angle.h"
#include "date.h"
#include "utils.h"

void VSOP87ProcessDataFiles();

using namespace PA;

constexpr Radian AberrationLongitude(Radian lon,
                                     Radian lat,
                                     double jd,
                                     Radian sunlong)
{
    constexpr double k{(20.49552_arcsec).Rad()};
    double t{(jd - EpochJ2000) / 36525.0};
    double e{horner_polynomial({0.016708634, -0.000042037, -0.0000001267}, t)};
    double pi{horner_polynomial(
        {(102.93735_deg).Rad(), (1.71946_deg).Rad(), (0.00046_deg).Rad()}, t)};
    return Radian((-k * std::cos(sunlong.Rad() - lon.Rad()) +
                   e * k * std::cos(pi - lon.Rad())) /
                  std::cos(lat.Rad()));
}

constexpr Radian AberrationLatitude(Radian lon,
                                    Radian lat,
                                    double jd,
                                    Radian sunlong)
{
    constexpr double k{(20.49552_arcsec).Rad()};
    double t{(jd - EpochJ2000) / 36525.0};
    double e{horner_polynomial({0.016708634, -0.000042037, -0.0000001267}, t)};
    double pi{horner_polynomial(
        {(102.93735_deg).Rad(), (1.71946_deg).Rad(), (0.00046_deg).Rad()}, t)};
    return Radian(
        -k * std::sin(lat.Rad()) *
        (std::sin(sunlong.Rad() - lon.Rad()) - e * std::sin(pi - lon.Rad())));
}

#endif  // MISC_H_
