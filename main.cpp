#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>

#include "date.h"
#include "earth.h"
#include "sun.h"
#include "test.h"
#include "vsop87.h"

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
        Earth earth{date};
        Degree nutation_longitude{earth.GetNutationLongitude()};
        Degree nutation_obliquity{earth.GetNutationObliquity()};
        Degree obliquity_mean{earth.GetObliquityMean()};
        Degree obliquity{earth.GetObliquity()};
        std::cout << "   Nutation Lon.: " << nutation_longitude.ArcSecStr(3)
                  << std::endl;
        std::cout << " Nutation Obliq.: " << nutation_obliquity.ArcSecStr(3)
                  << std::endl;
        std::cout << "     Mean Obliq.: " << obliquity_mean.DMSStr(2)
                  << std::endl;
        std::cout << "      Obliquity.: " << obliquity.DMSStr(2) << std::endl;
    }

    {
        std::cout << "Sun:" << std::endl;
        Sun sun{date.GetJulianDate()};
        Degree geometric_longitude{sun.GetGeometricLongitude()};
        Degree apparent_longitude{sun.GetApparentLongitude()};
        Degree geometric_latitude{sun.GetGeometricLatitude()};
        double radius_vector_au{sun.GetRadiusVectorAU()};
        std::cout << "  Geometric Lon.: " << geometric_longitude.DMSStr()
                  << " (" << geometric_longitude.DegStr() << ")" << std::endl;
        std::cout << "   Apparent Lon.: " << apparent_longitude.DMSStr() << " ("
                  << apparent_longitude.DegStr() << ")" << std::endl;
        std::cout << "  Geometric Lat.: " << geometric_latitude.ArcSecStr()
                  << std::endl;
        std::cout << std::fixed << std::setprecision(8);
        std::cout << "   Radius Vector: " << radius_vector_au << " AU"
                  << std::endl;
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

    // VSOP87ProcessDataFiles();

    PA::Date date;
    {
#if 1
        std::time_t now;
        now = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now());
        struct tm *ptm;
        ptm = std::gmtime(&now);
        date.SetCalendarTT(1900 + ptm->tm_year, ptm->tm_mon + 1, ptm->tm_mday,
                           ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
#else
        date.SetCalendarTT(1992, 10, 13);
        // date.SetCalendarTT(1992, 12, 20);
#endif
        ephemeris(date);
    }

    return 0;
}
