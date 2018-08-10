#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>

#include "angle.h"
#include "coordinate.h"
#include "date.h"

#include "elp82jm.h"

#include "earth.h"
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
        Degree nutation_longitude{earth.GetNutationLongitude()};
        Degree nutation_obliquity{earth.GetNutationObliquity()};
        Degree obliquity_mean{earth.GetObliquityMean()};
        Degree obliquity{earth.GetObliquity()};
        std::cout << "   Nutation Lon.: " << nutation_longitude.ArcSecStr(3)
                  << std::endl;
        std::cout << " Nutation Obliq.: " << nutation_obliquity.ArcSecStr(3)
                  << std::endl;
        std::cout << "     Mean Obliq.: " << obliquity_mean.DMSStr(2) << " ("
                  << obliquity_mean.DegStr(9) << ")" << std::endl;
        std::cout << "      Obliquity.: " << obliquity.DMSStr(2) << " ("
                  << obliquity.DegStr(9) << ")" << std::endl;
    }

    {
        std::cout << "Sun:" << std::endl;
        Sun sun{date.GetJulianDate()};
        EarthObliquity earth_obliquity{date.GetJulianDate()};
        Degree geometric_longitude{sun.GetGeometricLongitude()};
        Degree geometric_latitude{sun.GetGeometricLatitude()};
        Degree aberration_longitude{sun.GetAberrationLongitude()};
        Degree aberration_latitude{sun.GetAberrationLatitude()};
        Degree apparent_longitude{sun.GetApparentLongitude()};
        Degree apparent_latitude{sun.GetApparentLatitude()};
        Degree obliquity{earth_obliquity.GetObliquity()};
        Degree apparent_ra{Coordinate::EclipticalToEquatorialRightAscension(
            apparent_longitude, apparent_latitude, obliquity)};
        Degree apparent_decl{Coordinate::EclipticalToEquatorialDeclination(
            apparent_longitude, apparent_latitude, obliquity)};

        double radius_vector_au{sun.GetRadiusVectorAU()};
        std::cout << "  Geometric Lon.: " << geometric_longitude.DMSStr()
                  << " (" << geometric_longitude.DegStr() << ")" << std::endl;
        std::cout << "  Geometric Lat.: " << geometric_latitude.ArcSecStr()
                  << std::endl;
        std::cout << std::fixed << std::setprecision(9);
        std::cout << "   Radius Vector: " << radius_vector_au << " AU"
                  << std::endl;
        std::cout << " Aberration Lon.: " << aberration_longitude.ArcSecStr(9)
                  << " (" << aberration_longitude.DegStr(9) << ")" << std::endl;
        std::cout << " Aberration Lat.: " << aberration_latitude.ArcSecStr(9)
                  << " (" << aberration_latitude.DegStr(9) << ")" << std::endl;
        std::cout << "   Apparent Lon.: " << apparent_longitude.DMSStr() << " ("
                  << apparent_longitude.DegStr() << ")" << std::endl;
        std::cout << "   Apparent Lat.: " << apparent_latitude.DMSStr() << " ("
                  << apparent_latitude.DegStr() << ")" << std::endl;
        std::cout << "   Apparent R.A.: " << apparent_ra.HMSStr(3) << " ("
                  << apparent_ra.HourStr() << " = " << apparent_ra.DegStr()
                  << ")" << std::endl;
        std::cout << "  Apparent Decl.: " << apparent_decl.DMSStr() << " ("
                  << apparent_decl.DegStr() << " = " << apparent_decl.DegStr()
                  << ")" << std::endl;
        std::cout << std::fixed << std::setprecision(8);
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
        date.SetCalendarTT(1992, 10, 13);
        // date.SetCalendarTT(1992, 12, 20);
#endif
        ephemeris(date);
    }

#if 0
    {
        date.SetCalendarTT(1992, 4, 12);
        ELP82JM elp{date.GetJulianDate()};
        elp.GetPosition();
    }

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
