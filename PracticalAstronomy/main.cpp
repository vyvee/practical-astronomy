#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <regex>
#include <vector>

#include "coordinate.h"
#include "date.h"
#include "moon.h"
#include "observer.h"
#include "radian.h"
#include "test.h"

using namespace PA;

void ephemeris(Date date) {
  double jd = date.GetJulianDate();

  {
    std::ios_base::fmtflags fmtflags = std::cout.flags();

    std::cout << "Date & Time:" << std::endl;
    std::cout << "     Julian Date: " << std::fixed << std::setprecision(6)
              << jd << std::endl;
    std::cout << "       Date (TT): " << date.GetTTString() << std::endl;
    std::cout << "         Delta-T: " << std::showpos << std::fixed
              << std::setprecision(2) << date.GetDeltaT() << "s" << std::endl;

    std::cout.flags(fmtflags);
  }

  {
    std::ios_base::fmtflags fmtflags = std::cout.flags();

    Observer observer{jd};

    std::cout << "Earth:" << std::endl;
    std::cout << "     Nutation Lon.: " << std::setw(16)
              << RadToArcSecStr(observer.GetNutationLongitude(), 3)
              << std::endl;
    std::cout << "   Nutation Obliq.: " << std::setw(16)
              << RadToArcSecStr(observer.GetNutationObliquity(), 3)
              << std::endl;
    std::cout << "       Mean Obliq.: " << std::setw(19)
              << RadToDMSStr(observer.GetObliquityMean(), 3) << std::endl;
    std::cout << "        Obliquity.: " << std::setw(19)
              << RadToDMSStr(observer.GetObliquity(), 3) << " ("
              << RadToDegStr(observer.GetObliquity(), 6) << ")" << std::endl;

    std::cout << "Sun:" << std::endl;
    std::cout << "   Geocentric Lon.: " << std::setw(19)
              << RadToDMSStr(observer.GetLongitude(Observer::Body::kSun), 2)
              << " ("
              << RadToDegStr(observer.GetLongitude(Observer::Body::kSun), 6)
              << ")" << std::endl;
    std::cout << "   Geocentric Lat.: " << std::setw(16)
              << RadToArcSecStr(observer.GetLatitude(Observer::Body::kSun), 2)
              << " ("
              << RadToDegStr(observer.GetLatitude(Observer::Body::kSun), 6)
              << ")" << std::endl;
    std::cout << std::setprecision(8);
    std::cout << "     Radius Vector: " << std::setw(11)
              << observer.GetRadiusVectorAU(Observer::Body::kSun) << " AU"
              << std::endl;

    std::cout << "   Aberration Lon.: " << std::setw(16)
              << RadToArcSecStr(
                     observer.GetAberrationLongitude(Observer::Body::kSun), 3)
              << std::endl;
    std::cout << "   Aberration Lat.: " << std::setw(16)
              << RadToArcSecStr(
                     observer.GetAberrationLatitude(Observer::Body::kSun), 3)
              << std::endl;

    std::cout << "     Apparent Lon.: " << std::setw(19)
              << RadToDMSStr(
                     observer.GetApparentLongitude(Observer::Body::kSun), 2)
              << " ("
              << RadToDegStr(
                     observer.GetApparentLongitude(Observer::Body::kSun), 6)
              << ")" << std::endl;
    std::cout << "     Apparent Lat.: " << std::setw(16)
              << RadToArcSecStr(
                     observer.GetApparentLatitude(Observer::Body::kSun), 2)
              << " ("
              << RadToDegStr(observer.GetApparentLatitude(Observer::Body::kSun),
                             6)
              << ")" << std::endl;

    std::cout
        << "     Apparent R.A.: " << std::setw(14)
        << RadToHMSStr(observer.GetApparentRightAscension(Observer::Body::kSun),
                       3)
        << " ("
        << RadToHourStr(
               observer.GetApparentRightAscension(Observer::Body::kSun), 6)
        << " = "
        << RadToDegStr(observer.GetApparentRightAscension(Observer::Body::kSun),
                       6)
        << ")" << std::endl;
    std::cout << "    Apparent Decl.: " << std::setw(19)
              << RadToDMSStr(
                     observer.GetApparentDeclination(Observer::Body::kSun), 2)
              << " ("
              << RadToDegStr(
                     observer.GetApparentDeclination(Observer::Body::kSun), 6)
              << ")" << std::endl;

#if 0
    std::cout << "Venus:" << std::endl;
    std::cout << "   Geocentric Lon.: " << std::setw(19)
              << RadToDMSStr(observer.GetLongitude(Observer::Body::kVenus), 2)
              << " ("
              << RadToDegStr(observer.GetLongitude(Observer::Body::kVenus), 6)
              << ")" << std::endl;
    std::cout << "   Geocentric Lat.: " << std::setw(19)
              << RadToDMSStr(observer.GetLatitude(Observer::Body::kVenus), 2)
              << " ("
              << RadToDegStr(observer.GetLatitude(Observer::Body::kVenus), 6)
              << ")" << std::endl;
    std::cout << std::setprecision(8);
    std::cout << "     Radius Vector: " << std::setw(11)
              << observer.GetRadiusVectorAU(Observer::Body::kVenus) << " AU"
              << std::endl;

    std::cout << "Saturn:" << std::endl;
    std::cout << "   Geocentric Lon.: " << std::setw(19)
              << RadToDMSStr(observer.GetLongitude(Observer::Body::kSaturn), 2)
              << " ("
              << RadToDegStr(observer.GetLongitude(Observer::Body::kSaturn), 6)
              << ")" << std::endl;
    std::cout << "   Geocentric Lat.: " << std::setw(19)
              << RadToDMSStr(observer.GetLatitude(Observer::Body::kSaturn), 2)
              << " ("
              << RadToDegStr(observer.GetLatitude(Observer::Body::kSaturn), 6)
              << ")" << std::endl;
    std::cout << std::setprecision(8);
    std::cout << "     Radius Vector: " << std::setw(11)
              << observer.GetRadiusVectorAU(Observer::Body::kVenus) << " AU"
              << std::endl;
#endif

    std::cout.flags(fmtflags);
  }

  {
    std::ios_base::fmtflags fmtflags = std::cout.flags();

    EarthObliquityOld earth_obliquity{jd};
    double obliquity{earth_obliquity.GetObliquity()};

#if 0
    std::cout << "   Mean Longitude.: " << std::setw(19)
              << RadToDMSStr(sun_mean_longitude, 2) << " ("
              << RadToDegStr(sun_mean_longitude, 6) << ")" << std::endl;
    std::cout << std::fixed << std::setprecision(8);
#endif

    std::cout << "Moon:" << std::endl;

    Moon moon{date.GetJulianDate()};
    double moon_geocentric_longitude{moon.GetGeocentricLongitude()};
    double moon_geocentric_latitude{moon.GetGeocentricLatitude()};
    double moon_distance_km{moon.GetDistanceKm()};
    double moon_apparent_longitude{moon.GetApparentLongitude()};
    double moon_apparent_latitude{moon.GetApparentLatitude()};
    double moon_apparent_ra{Coordinate::EclipticalToEquatorialRightAscension(
        moon_apparent_longitude, moon_apparent_latitude, obliquity)};
    double moon_apparent_decl{Coordinate::EclipticalToEquatorialDeclination(
        moon_apparent_longitude, moon_apparent_latitude, obliquity)};

    std::cout << "   Geocentric Lon.: " << std::setw(19)
              << RadToDMSStr(moon_geocentric_longitude, 0) << " ("
              << RadToDegStr(moon_geocentric_longitude, 6) << ")" << std::endl;
    std::cout << "   Geocentric Lat.: " << std::setw(19)
              << RadToDMSStr(moon_geocentric_latitude, 0) << " ("
              << RadToDegStr(moon_geocentric_latitude, 6) << ")" << std::endl;
    std::cout << std::noshowpos << std::fixed << std::setprecision(1);
    std::cout << "          Distance: " << std::setw(11) << moon_distance_km
              << " km" << std::endl;
    std::cout << "     Apparent Lon.: " << std::setw(19)
              << RadToDMSStr(moon_apparent_longitude, 0) << " ("
              << RadToDegStr(moon_apparent_longitude, 6) << ")" << std::endl;
    std::cout << "     Apparent Lat.: " << std::setw(19)
              << RadToDMSStr(moon_apparent_latitude, 0) << " ("
              << RadToDegStr(moon_apparent_latitude, 6) << ")" << std::endl;
    std::cout << "     Apparent R.A.: " << std::setw(14)
              << RadToHMSStr(moon_apparent_ra, 1) << " ("
              << RadToHourStr(moon_apparent_ra) << " = "
              << RadToDegStr(moon_apparent_ra) << ")" << std::endl;
    std::cout << "    Apparent Decl.: " << std::setw(19)
              << RadToDMSStr(moon_apparent_decl) << " ("
              << RadToDegStr(moon_apparent_decl) << ")" << std::endl;

    /* Equation of Time */
    // For comparison: http://mb-soft.com/public3/equatime.html

    // double eot{RadNormalize(sun_mean_longitude - 20.49552_arcsec -
    //                         0.09033_arcsec - sun_apparent_ra +
    //                         nutation_longitude * cos(obliquity))};

    // std::cout << "Equation of Time: " << std::setw(16) << RadToHMSStr(eot)
    //           << " (" << RadToDegStr(eot) << ")" << std::endl;

    std::cout.flags(fmtflags);
  }
}

int main(void) {
  test_internal();

  std::string line;
  std::cout << "Date (y-m-d-hh:mm:ss or y-m-d), or (n)ow? ";
  std::cin >> line;
  if (line == "n") {
    PA::Date date;
    std::time_t now;
    now =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm* ptm;
    ptm = std::gmtime(&now);
    date.SetCalendarTT(1900 + ptm->tm_year, ptm->tm_mon + 1, ptm->tm_mday,
                       ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    ephemeris(date);
  } else {
    std::smatch match;
    if (std::regex_match(line, match,
                         std::regex("(\\d+)\\-(\\d+)\\-(\\d+)-"
                                    "(\\d+):(\\d+):(\\d+(?:\\.\\d+)?)",
                                    std::regex::ECMAScript))) {
      PA::Date date;
      date.SetCalendarTT(std::stoi(match.str(1)), std::stoi(match.str(2)),
                         std::stoi(match.str(3)), std::stoi(match.str(4)),
                         std::stoi(match.str(5)), std::stof(match.str(6)));
      ephemeris(date);
    } else if (std::regex_match(
                   line, match,
                   std::regex("^(\\d+)\\-(\\d+)\\-(\\d+(?:\\.\\d+)?)$",
                              std::regex::ECMAScript))) {
      PA::Date date;
      date.SetCalendarTT(std::stoi(match.str(1)), std::stoi(match.str(2)),
                         std::stof(match.str(3)));
      ephemeris(date);
    }
  }

#if 0
    {
        // [Peter11] p.166
        // 214.8675028
        // 1.716074358
        // RA: 14h 12m 10s
        // Decl: -11d 34' 52"
        date.SetCalendarTT(2003, 9, 1);
        ELP82JM elp{date.GetJulianDate()};
        elp.GetPosition();
    }
#endif

  return 0;
}
