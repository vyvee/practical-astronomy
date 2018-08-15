#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>

#include "coordinate.h"
#include "date.h"
#include "radian.h"

#include "elp82jm.h"

#include "earth.h"
#include "moon.h"
#include "sun.h"
#include "test.h"

using namespace PA;

void ephemeris(Date date)
{
    {
        std::cout << "Date & Time:" << std::endl;
        std::cout << "     Julian Date: " << std::fixed << std::setprecision(6)
                  << date.GetJulianDate() << std::endl;
        std::cout << "       Date (TT): " << date.GetTTString() << std::endl;
        std::cout << "         Delta-T: " << std::showpos << std::fixed
                  << std::setprecision(2) << date.GetDeltaT() << "s"
                  << std::endl;
    }

    {
        std::cout << "Earth:" << std::endl;
        Earth earth{date.GetJulianDate()};
        double nutation_longitude{earth.GetNutationLongitude()};
        double nutation_obliquity{earth.GetNutationObliquity()};
        double obliquity_mean{earth.GetObliquityMean()};
        double obliquity{earth.GetObliquity()};
        std::cout << "   Nutation Lon.: " << std::setw(13)
                  << RadToArcSecStr(nutation_longitude, 3) << std::endl;
        std::cout << " Nutation Obliq.: " << std::setw(13)
                  << RadToArcSecStr(nutation_obliquity, 3) << std::endl;
        std::cout << "     Mean Obliq.: " << std::setw(13)
                  << RadToDMSStr(obliquity_mean, 2) << " ("
                  << RadToDegStr(obliquity_mean, 9) << ")" << std::endl;
        std::cout << "      Obliquity.: " << std::setw(13)
                  << RadToDMSStr(obliquity, 2) << " ("
                  << RadToDegStr(obliquity, 9) << ")" << std::endl;
    }

    {
        std::cout << "Sun:" << std::endl;
        Sun sun{date.GetJulianDate()};
        EarthObliquity earth_obliquity{date.GetJulianDate()};
        double geocentric_longitude{sun.GetGeocentricLongitude()};
        double geocentric_latitude{sun.GetGeocentricLatitude()};
        double aberration_longitude{sun.GetAberrationLongitude()};
        double aberration_latitude{sun.GetAberrationLatitude()};
        double apparent_longitude{sun.GetApparentLongitude()};
        double apparent_latitude{sun.GetApparentLatitude()};
        double obliquity{earth_obliquity.GetObliquity()};
        double apparent_ra{Coordinate::EclipticalToEquatorialRightAscension(
            apparent_longitude, apparent_latitude, obliquity)};
        double apparent_decl{Coordinate::EclipticalToEquatorialDeclination(
            apparent_longitude, apparent_latitude, obliquity)};
        double radius_vector_au{sun.GetRadiusVectorAU()};

        std::cout << " Geocentric Lon.: " << std::setw(13)
                  << RadToDMSStr(geocentric_longitude) << " ("
                  << RadToDegStr(geocentric_longitude) << ")" << std::endl;
        std::cout << " Geocentric Lat.: " << std::setw(13)
                  << RadToArcSecStr(geocentric_latitude) << " ("
                  << RadToDegStr(geocentric_latitude) << ")" << std::endl;
        std::cout << std::fixed << std::setprecision(8);
        std::cout << "   Radius Vector: " << std::setw(13) << radius_vector_au
                  << " AU" << std::endl;
        std::cout << " Aberration Lon.: " << std::setw(13)
                  << RadToArcSecStr(aberration_longitude, 8) << " ("
                  << RadToDegStr(aberration_longitude, 8) << ")" << std::endl;
        std::cout << " Aberration Lat.: " << std::setw(13)
                  << RadToArcSecStr(aberration_latitude, 8) << " ("
                  << RadToDegStr(aberration_latitude, 8) << ")" << std::endl;
        std::cout << "   Apparent Lon.: " << std::setw(13)
                  << RadToDMSStr(apparent_longitude) << " ("
                  << RadToDegStr(apparent_longitude) << ")" << std::endl;
        std::cout << "   Apparent Lat.: " << std::setw(13)
                  << RadToArcSecStr(apparent_latitude) << " ("
                  << RadToDegStr(apparent_latitude) << ")" << std::endl;
        std::cout << "   Apparent R.A.: " << std::setw(13)
                  << RadToHMSStr(apparent_ra, 3) << " ("
                  << RadToHourStr(apparent_ra) << " = "
                  << RadToDegStr(apparent_ra) << ")" << std::endl;
        std::cout << "  Apparent Decl.: " << std::setw(13)
                  << RadToDMSStr(apparent_decl) << " ("
                  << RadToDegStr(apparent_decl) << ")" << std::endl;
        std::cout << std::fixed << std::setprecision(8);
    }

    {
        std::cout << "Moon:" << std::endl;
        Moon moon{date.GetJulianDate()};
        EarthObliquity earth_obliquity{date.GetJulianDate()};
        double geocentric_longitude{moon.GetGeocentricLongitude()};
        double geocentric_latitude{moon.GetGeocentricLatitude()};
        double distance_km{moon.GetDistanceKm()};
        double apparent_longitude{moon.GetApparentLongitude()};
        double apparent_latitude{moon.GetApparentLatitude()};
        double obliquity{earth_obliquity.GetObliquity()};
        double apparent_ra{Coordinate::EclipticalToEquatorialRightAscension(
            apparent_longitude, apparent_latitude, obliquity)};
        double apparent_decl{Coordinate::EclipticalToEquatorialDeclination(
            apparent_longitude, apparent_latitude, obliquity)};

        std::cout << " Geocentric Lon.: " << std::setw(13)
                  << RadToDMSStr(geocentric_longitude) << " ("
                  << RadToDegStr(geocentric_longitude) << ")" << std::endl;
        std::cout << " Geocentric Lat.: " << std::setw(13)
                  << RadToDMSStr(geocentric_latitude) << " ("
                  << RadToDegStr(geocentric_latitude) << ")" << std::endl;
        std::cout << std::noshowpos << std::fixed << std::setprecision(3);
        std::cout << "        Distance: " << std::setw(13) << distance_km
                  << " km" << std::endl;
        std::cout << "   Apparent Lon.: " << std::setw(13)
                  << RadToDMSStr(apparent_longitude) << " ("
                  << RadToDegStr(apparent_longitude) << ")" << std::endl;
        std::cout << "   Apparent Lat.: " << std::setw(13)
                  << RadToDMSStr(apparent_latitude) << " ("
                  << RadToDegStr(apparent_latitude) << ")" << std::endl;
        std::cout << "   Apparent R.A.: " << std::setw(13)
                  << RadToHMSStr(apparent_ra, 3) << " ("
                  << RadToHourStr(apparent_ra) << " = "
                  << RadToDegStr(apparent_ra) << ")" << std::endl;
        std::cout << "  Apparent Decl.: " << std::setw(13)
                  << RadToDMSStr(apparent_decl) << " ("
                  << RadToDegStr(apparent_decl) << ")" << std::endl;
    }

#if 0
    {
        Degree ra{coord.GetEquatorialRightAscension()};
        Degree decl{coord.GetEquatorialDeclination()};
        std::cout << " Equatorial R.A.: " << ra.HMSStr() << " (" << ra.HourStr()
                  << ")" << std::endl;
        std::cout << " Equatorial Dec.: " << decl.DMSStr() << " ("
                  << decl.DegStr() << ")" << std::endl;
    }
#endif
}

int main(void)
{
    if(test_internal()) {
        std::cout << "Internal Test OK!" << std::endl;
    } else {
        std::cout << "Error: Internal Test Failed!" << std::endl;
        // return 0;
    }

    PA::Date date;
    {
#if 1
        std::time_t now;
        now = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now());
        struct tm* ptm;
        ptm = std::gmtime(&now);
        date.SetCalendarTT(1900 + ptm->tm_year, ptm->tm_mon + 1, ptm->tm_mday,
                           ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
#else
        // date.SetCalendarTT(
        //     1993, 5, 21 + 14.0 / 24.0 + 20.0 / (24.0 * 60.0) + 14.0 /
        //     86400.0);
        date.SetCalendarTT(1992, 4, 12.0);
#endif
        ephemeris(date);
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
