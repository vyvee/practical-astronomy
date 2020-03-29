#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <string>

#include "coordinate.h"
#include "earth_nutation.h"
#include "earth_obliquity.h"
#include "elp82jm.h"
#include "misc.h"
#include "sun.h"
#include "vsop87.h"

namespace PA {

class Observer {
 public:
  enum class Body {
    kSun,
    kMoon,
    /*
    kMercury,
    kVenus,
    kEarth,
    kMars,
    kJupiter,
    kSaturn,
    kUranus,
    kNeptune,
    kPluto,
    */
    kMax,
  };

  /* Body Name */

  static inline std::string BodyName(Body body) noexcept {
    switch (body) {
      case Body::kSun:
        return std::string("Sun");
      case Body::kMoon:
        return std::string("Moon");
      default:
        return std::string("Unknown");
    }
  }

  // constexpr Observer(double tt, Body observe) noexcept
  //     : tt_(tt), observe_(observe) {
  //   for (int i = 0; i < static_cast<int>(Body::kMax); i++) {
  //     body_position_is_valid_[i] = false;
  //   }
  // }

  // constexpr Observer(Body observe, double tt) noexcept
  //     : Observer(tt, observe) {}

  constexpr Observer(double tt) noexcept : tt_(tt) {}
  // constexpr Observer(double tt) noexcept : Observer(tt, Body::kMax) {}

  // constexpr Observer() noexcept : Observer(EpochJ2000) {}

  // constexpr const Observer& Observe(Body body) noexcept {
  //   observe_ = body;
  //   return *this;
  // }

  constexpr const Observer& At(double tt) noexcept {
    tt_ = tt;
    return *this;
  }

  /* TT */

  constexpr double GetTT() const noexcept;

  /* Nutation */

  enum class NutationAlgorithm {
    kIAU1980MeeusTruncated,
    kIAU1980,
    kIAU2000B,
  };
  constexpr void SetNutationAlgorithm(NutationAlgorithm algorithm) noexcept;
  constexpr double GetNutationLongitude() const noexcept;
  constexpr double GetNutationObliquity() const noexcept;

  /* Obliquity */

  constexpr double GetObliquityMean() const noexcept;
  constexpr double GetObliquity() const noexcept;

  /* Geocentric Position */

  constexpr double GetGeocentricLongitude(Body body) const noexcept;
  constexpr double GetGeocentricLatitude(Body body) const noexcept;
  constexpr double GetRadiusVectorAU(Body body) const noexcept;

  /* Abberation */

  constexpr double GetAberrationLongitude(Body body) const noexcept;
  constexpr double GetAberrationLatitude(Body body) const noexcept;

  /* Apparent Position */

  constexpr double GetApparentLongitude(Body body) const noexcept;
  constexpr double GetApparentLatitude(Body body) const noexcept;

  /* Apparent Right Ascension and Declination */

  constexpr double GetApparentRightAscension(Body body) const noexcept;
  constexpr double GetApparentDeclination(Body body) const noexcept;

 private:
  double tt_{EpochJ2000};
  // Body observe_{Body::kMax};

  constexpr void ComputeNutation() const noexcept;
  NutationAlgorithm nutation_algorithm_{NutationAlgorithm::kIAU2000B};
  mutable bool nutation_is_valid_{false};
  mutable double nutation_longitude_{0.0};
  mutable double nutation_obliquity_{0.0};

  constexpr void ComputeObliquity() const noexcept;
  mutable bool obliquity_is_valid_{false};
  mutable double obliquity_mean_{0.0};
  mutable double obliquity_{0.0};

  constexpr void ComputePosition(Body body) const noexcept;

  constexpr bool LookupBodyPositionIsValid(Body body) const noexcept;
  constexpr void LookupBodyPositionSetIsValid(Body body, bool validity) const
      noexcept;
  constexpr double LookupBodyLongitude(Body body) const noexcept;
  constexpr void LookupBodySetLongitude(Body body, double lon) const noexcept;
  constexpr double LookupBodyLatitude(Body body) const noexcept;
  constexpr void LookupBodySetLatitude(Body body, double lat) const noexcept;
  constexpr double LookupBodyRadiusVectorAU(Body body) const noexcept;
  constexpr void LookupBodySetRadiusVectorAU(Body body,
                                             double radius_vector_au) const
      noexcept;
  mutable double body_position_is_valid_[static_cast<int>(Body::kMax)]{{false}};
  mutable double body_longitude_[static_cast<int>(Body::kMax)]{{0.0}};
  mutable double body_latitude_[static_cast<int>(Body::kMax)]{{0.0}};
  mutable double body_radius_vector_au_[static_cast<int>(Body::kMax)]{{0.0}};
};

constexpr double Observer::GetTT() const noexcept { return tt_; }

/* Nutation */

constexpr void Observer::ComputeNutation() const noexcept {
  if (nutation_is_valid_) return;
  switch (nutation_algorithm_) {
    case NutationAlgorithm::kIAU1980MeeusTruncated:
      EarthNutation::ComputeNutationIAU1980MeeusTruncated(
          tt_, &nutation_longitude_, &nutation_obliquity_);
      break;
    case NutationAlgorithm::kIAU1980:
      EarthNutation::ComputeNutationIAU1980(tt_, &nutation_longitude_,
                                            &nutation_obliquity_);
      break;
    case NutationAlgorithm::kIAU2000B:
      EarthNutation::ComputeNutationIAU2000B(tt_, &nutation_longitude_,
                                             &nutation_obliquity_);
      break;
  }
  nutation_is_valid_ = true;
}

constexpr void Observer::SetNutationAlgorithm(
    NutationAlgorithm algorithm) noexcept {
  if (nutation_algorithm_ != algorithm) {
    nutation_algorithm_ = algorithm;
    nutation_is_valid_ = false;
  }
}

constexpr double Observer::GetNutationLongitude() const noexcept {
  ComputeNutation();
  return nutation_longitude_;
}

constexpr double Observer::GetNutationObliquity() const noexcept {
  ComputeNutation();
  return nutation_obliquity_;
}

/* Obliquity */

constexpr void Observer::ComputeObliquity() const noexcept {
  if (obliquity_is_valid_) return;
  obliquity_mean_ = EarthObliquity::ComputeObliquityMean(tt_);
  obliquity_ = obliquity_mean_ + GetNutationObliquity();
}

constexpr double Observer::GetObliquityMean() const noexcept {
  ComputeObliquity();
  return obliquity_mean_;
}

constexpr double Observer::GetObliquity() const noexcept {
  ComputeObliquity();
  return obliquity_;
}

/* Geocentric Position
 * - [Jean99] p.217 (Positions of the Planets)
 * - [Jean99] p.223 (Elliptic Motion)
 */

constexpr void Observer::ComputePosition(Body body) const noexcept {
  if (LookupBodyPositionIsValid(body)) return;

  switch (body) {
    case Body::kSun: {
      // VSOP87
      // - Accuracy: < 0.01" in longitude over 1900-2100
      // - Sun moves about 0.04" in longitude per second, so this corresponds to
      //   about 0.25 seconds
      // - [Jean99] p.166
      double earth_longitude{0.0}, earth_latitude{0.0},
          earth_radius_vector_au{0.0};
      VSOP87::Compute(tt_, VSOP87::Planet::kEarth, &earth_longitude,
                      &earth_latitude, &earth_radius_vector_au);
      VSOP87::VSOP87DFrameToFK5(tt_, &earth_longitude, &earth_latitude);
      LookupBodySetLongitude(body, RadUnwind(earth_longitude + M_PI));
      LookupBodySetLatitude(body, -earth_latitude);
      LookupBodySetRadiusVectorAU(body, earth_radius_vector_au);
      LookupBodyPositionSetIsValid(body, true);
    } break;

    case Body::kMoon: {
      double moon_longitude{0.0}, moon_latitude{0.0},
          moon_radius_vector_km{0.0};
      ELP82JM::Compute(tt_, &moon_longitude, &moon_latitude,
                       &moon_radius_vector_km);
      // We removed the light-time correction and moved this to the section for
      // aberration and light-time correction
      // - Reference: [Jean99] p.337
      LookupBodySetLongitude(body, moon_longitude + 0.704_arcsec);
      LookupBodySetLatitude(body, moon_latitude);
      LookupBodySetRadiusVectorAU(body, moon_radius_vector_km / 149597870.7);
      LookupBodyPositionSetIsValid(body, true);
    } break;

    default:
      assert(0);
  }
}

constexpr double Observer::GetGeocentricLongitude(Body body) const noexcept {
  ComputePosition(body);
  return LookupBodyLongitude(body);
}

constexpr double Observer::GetGeocentricLatitude(Body body) const noexcept {
  ComputePosition(body);
  return LookupBodyLatitude(body);
}

constexpr double Observer::GetRadiusVectorAU(Body body) const noexcept {
  ComputePosition(body);
  return LookupBodyRadiusVectorAU(body);
}

constexpr bool Observer::LookupBodyPositionIsValid(Body body) const noexcept {
  return body_position_is_valid_[static_cast<int>(body)];
}

constexpr void Observer::LookupBodyPositionSetIsValid(Body body,
                                                      bool validity) const
    noexcept {
  body_position_is_valid_[static_cast<int>(body)] = validity;
}

constexpr double Observer::LookupBodyLongitude(Body body) const noexcept {
  return body_longitude_[static_cast<int>(body)];
}

constexpr void Observer::LookupBodySetLongitude(Body body, double lon) const
    noexcept {
  body_longitude_[static_cast<int>(body)] = lon;
}

constexpr double Observer::LookupBodyLatitude(Body body) const noexcept {
  return body_latitude_[static_cast<int>(body)];
}

constexpr void Observer::LookupBodySetLatitude(Body body, double lat) const
    noexcept {
  body_latitude_[static_cast<int>(body)] = lat;
}

constexpr double Observer::LookupBodyRadiusVectorAU(Body body) const noexcept {
  return body_radius_vector_au_[static_cast<int>(body)];
}

constexpr void Observer::LookupBodySetRadiusVectorAU(
    Body body, double radius_vector_au) const noexcept {
  body_radius_vector_au_[static_cast<int>(body)] = radius_vector_au;
}

/* Aberration */

// Aberration and light-time correction of Moon:
// - https://en.wikipedia.org/wiki/New_moon
// - http://adsabs.harvard.edu/full/1952AJ.....57...46C
// -
// https://books.google.com.my/books?id=uDRBAQAAIAAJ&pg=RA3-PA67&lpg=RA3-PA67&dq=aberration+of+moon&source=bl&ots=Q3Vy0DtmZv&sig=ACfU3U3gTf7paQjVNEYNwq-kPz-9YhX7RA&hl=en&sa=X&ved=2ahUKEwiz7eXVwL3oAhXbZCsKHeAXBFI4ChDoATADegQIChAB#v=onepage&q=aberration%20of%20moon&f=false

constexpr double Observer::GetAberrationLongitude(Body body) const noexcept {
  switch (body) {
    case Body::kSun: {
      // [Jean99] p.167
      // Accuracy: 0".01
      // aberration_longitude_ = -20.4898_arcsec / radius_vector_au_;
      // aberration_latitude_ = 0.0;
      // aberration_is_valid_ = true;
      // return AberrationLongitude(GetGeocentricLongitude(body),
      //                            GetGeocentricLatitude(body),
      //                            GetTT(), GetGeocentricLongitude(body));
      // [Jean99] p.167
      // Accuracy: < 0".001
      return -0.005775518 * GetRadiusVectorAU(body) *
             Sun::GetDailyVariation(GetTT());
    };

    case Body::kMoon: {
      // We apply the light-time correction here
      // - Reference: [Jean99] p.337
      return -0.704_arcsec;
    };

    default:
      // Planets: [Jean99] p.223
      // Pluto: [Jean99] p.263
      // return AberrationLongitude(GetGeocentricLongitude(body),
      // GetGeocentricLatitude(body), GetTT(),
      //                            GetGeocentricLongitude(Body::kSun));
      assert(0);
      return 0.0;
  }
}

constexpr double Observer::GetAberrationLatitude(Body body) const noexcept {
  switch (body) {
    case Body::kSun: {
      // Should be less than 0.00001 arsec
      return AberrationLatitude(GetGeocentricLongitude(body),
                                GetGeocentricLatitude(body), GetTT(),
                                GetGeocentricLongitude(Body::kSun));
    };

    case Body::kMoon: {
      return 0.0;
    };

    default:
      // return AberrationLatitude(GetGeocentricLongitude(body),
      // GetGeocentricLatitude(body), GetTT(),
      //                           GetGeocentricLongitude(Body::kSun));
      assert(0);
      return 0.0;
  }
}

/* Apparent Position
 * - [Jean99] p.149 (Apparent Place of a Star)
 */

constexpr double Observer::GetApparentLongitude(Body body) const noexcept {
  return GetGeocentricLongitude(body) + GetNutationLongitude() +
         GetAberrationLongitude(body);
}

constexpr double Observer::GetApparentLatitude(Body body) const noexcept {
  return GetGeocentricLatitude(body) + GetAberrationLatitude(body);
}

constexpr double Observer::GetApparentRightAscension(Body body) const noexcept {
  // Reference for Sun:
  // - https://www.hko.gov.hk/en/gts/astronomy/sun_ra_dec.htm
  return Coordinate::EclipticalToEquatorialRightAscension(
      GetApparentLongitude(body), GetApparentLatitude(body), GetObliquity());
}

constexpr double Observer::GetApparentDeclination(Body body) const noexcept {
  return Coordinate::EclipticalToEquatorialDeclination(
      GetApparentLongitude(body), GetApparentLatitude(body), GetObliquity());
}

}  // namespace PA

#endif  // OBSERVER_H_