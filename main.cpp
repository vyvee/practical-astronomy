#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

#include "date.h"
#include "earth.h"
#include "sun.h"
#include "test.h"

using namespace PA;

void ephemeris_now(void)
{
    PA::Date date;

    {
        std::cout << "Date & Time:" << std::endl;
        std::time_t now;
        now = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now());
        struct tm *ptm;
        ptm = std::gmtime(&now);
        date.SetCalendarTT(1900 + ptm->tm_year, ptm->tm_mon + 1, ptm->tm_mday,
                           ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
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
        Angle nutation_longitude{earth.GetNutationLongitude()};
        Angle nutation_obliquity{earth.GetNutationObliquity()};
        Angle obliquity_mean{earth.GetObliquityMean()};
        Angle obliquity{earth.GetObliquity()};
        std::cout << "   Nutation Lon.: " << nutation_longitude.arcsec() << '\"'
                  << std::endl;
        std::cout << " Nutation Obliq.: " << nutation_obliquity.arcsec() << '\"'
                  << std::endl;
        std::cout << "     Mean Obliq.: " << obliquity_mean.dms_str()
                  << std::endl;
        std::cout << "      Obliquity.: " << obliquity.dms_str() << std::endl;
    }

    {
        std::cout << "Sun:" << std::endl;
        Sun sun{date};
        Coordinate coord;
        coord = sun.GetPosition();
        Angle lon{coord.GetEclipticLongitude()};
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "   Ecliptic Lon.: " << lon.dms_str() << std::endl;
        Angle ra{coord.GetEquatorialRightAscension()};
        Angle decl{coord.GetEquatorialDeclination()};
        std::cout << " Equatorial R.A.: " << ra.hms_str() << " ("
                  << ra.hour_str() << ")" << std::endl;
        std::cout << " Equatorial Dec.: " << decl.dms_str() << " ("
                  << decl.deg_str() << ")" << std::endl;
    }
}

int main(void)
{
    if(test_internal()) {
        std::cout << "Internal Test OK!" << std::endl;
    } else {
        std::cout << "Error: Internal Test Failed!" << std::endl;
        return 0;
    }

    ephemeris_now();

    return 0;
}
