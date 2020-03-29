#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <regex>
#include <vector>

#include "coordinate.h"
#include "date.h"
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

    Observer::Body bodies[]{Observer::Body::kSun, Observer::Body::kMoon};
    for (auto body : bodies) {
      std::cout << Observer::BodyName(body) << ":" << std::endl;
      std::cout << "   Geocentric Lon.: " << std::setw(19)
                << RadToDMSStr(observer.GetGeocentricLongitude(body), 2) << " ("
                << RadToDegStr(observer.GetGeocentricLongitude(body), 6) << ")"
                << std::endl;
      std::cout << "   Geocentric Lat.: " << std::setw(19)
                << RadToDMSStr(observer.GetGeocentricLatitude(body), 2) << " ("
                << RadToDegStr(observer.GetGeocentricLatitude(body), 6) << ")"
                << std::endl;
      std::cout << std::setprecision(8);
      std::cout << "     Radius Vector: " << std::setw(11)
                << observer.GetRadiusVectorAU(body) << " AU";
      std::cout << std::fixed << std::setprecision(1) << " ("
                << observer.GetRadiusVectorAU(body) * 149597870.7 << " km)"
                << std::endl;
      std::cout << "   Aberration Lon.: " << std::setw(16)
                << RadToArcSecStr(observer.GetAberrationLongitude(body), 3)
                << std::endl;
      std::cout << "   Aberration Lat.: " << std::setw(16)
                << RadToArcSecStr(observer.GetAberrationLatitude(body), 3)
                << std::endl;
      std::cout << "     Apparent Lon.: " << std::setw(19)
                << RadToDMSStr(observer.GetApparentLongitude(body), 2) << " ("
                << RadToDegStr(observer.GetApparentLongitude(body), 6) << ")"
                << std::endl;
      std::cout << "     Apparent Lat.: " << std::setw(19)
                << RadToDMSStr(observer.GetApparentLatitude(body), 2) << " ("
                << RadToDegStr(observer.GetApparentLatitude(body), 6) << ")"
                << std::endl;
      std::cout << "     Apparent R.A.: " << std::setw(14)
                << RadToHMSStr(observer.GetApparentRightAscension(body), 3)
                << " ("
                << RadToHourStr(observer.GetApparentRightAscension(body), 6)
                << " = "
                << RadToDegStr(observer.GetApparentRightAscension(body), 6)
                << ")" << std::endl;
      std::cout << "    Apparent Decl.: " << std::setw(19)
                << RadToDMSStr(observer.GetApparentDeclination(body), 2) << " ("
                << RadToDegStr(observer.GetApparentDeclination(body), 6) << ")"
                << std::endl;
    }

    std::cout.flags(fmtflags);
  }

  {
    std::ios_base::fmtflags fmtflags = std::cout.flags();

#if 0
    std::cout << "   Mean Longitude.: " << std::setw(19)
              << RadToDMSStr(sun_mean_longitude, 2) << " ("
              << RadToDegStr(sun_mean_longitude, 6) << ")" << std::endl;
    std::cout << std::fixed << std::setprecision(8);
#endif

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
