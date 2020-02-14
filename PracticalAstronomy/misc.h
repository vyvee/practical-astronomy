#ifndef MISC_H_
#define MISC_H_

#include <cmath>

#include "date.h"
#include "radian.h"
#include "utils.h"

using namespace PA;

constexpr double AberrationLongitude(double lon, double lat, double jd,
                                     double sunlong) {
  // Reference: [Jean99] p.151
  constexpr double k{20.49552_arcsec};
  double t{(jd - EpochJ2000) / 36525.0};
  double e{horner_polynomial({0.016708634, -0.000042037, -0.0000001267}, t)};
  double pi{horner_polynomial({102.93735_deg, 1.71946_deg, 0.00046_deg}, t)};
  return k * (e * std::cos(pi - lon) - std::cos(sunlong - lon)) / std::cos(lat);
}

constexpr double AberrationLatitude(double lon, double lat, double jd,
                                    double sunlong) {
  // Reference: [Jean99] p.151
  constexpr double k{20.49552_arcsec};
  double t{(jd - EpochJ2000) / 36525.0};
  double e{horner_polynomial({0.016708634, -0.000042037, -0.0000001267}, t)};
  double pi{horner_polynomial({102.93735_deg, 1.71946_deg, 0.00046_deg}, t)};
  return -k * std::sin(lat) *
         (std::sin(sunlong - lon) - e * std::sin(pi - lon));
}

#endif  // MISC_H_
