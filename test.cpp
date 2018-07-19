#include "test.h"

#include "coordinate.h"
#include "date.h"
#include "earth.h"
#include "sun.h"

#include <cmath>

#define VERBOSE

#ifdef VERBOSE
#include <iomanip>
#include <iostream>
#endif

using namespace PA;

static bool double_is_close(double x1,
                            double x2,
                            double rel_tol,
                            double abs_tol = 0.0)
{
    return fabs(x1 - x2) <= fmax(rel_tol * fmax(fabs(x1), fabs(x2)), abs_tol);
}

bool test_julian_date()
{
    Date date;
    struct {
        int year;
        int month;
        double day;
        double julian_date;
    } date_test_data[] = {
        {
            -4712,
            1,
            1.5,
            0.0,
        },
        {
            2009,
            6,
            19.75,
            2455002.25,
        },
    };
    for(auto& td : date_test_data) {
        double julian_date;
        date.SetCalendarDate(td.year, td.month, td.day);
        if(!date.GetJulianDate(&julian_date)) {
            return false;
        }
        if(!double_is_close(julian_date, td.julian_date, 1.0e-9, 0.0)) {
            return false;
        }
        int year, month;
        double day;
        date.SetJulianDate(td.julian_date);
        if(!date.GetCalendarDate(&year, &month, &day)) {
            return false;
        }
        if(!(year == td.year && month == td.month &&
             double_is_close(day, td.day, 1.0e-9, 0.0))) {
            return false;
        }
    }
    return true;
}

bool test_coordinate()
{
    std::cout << "Coordinate: EC -> EQ... ";
    {
        // [Peter11] p.54
        Date date{2009, 7, 6};
        Coordinate coord;
        coord.SetEcliptic(139.686111_deg, 4.875278_deg, date);
        Angle ra{coord.GetEquatorialRightAscension()};
        Angle decl{coord.GetEquatorialDeclination()};
#ifdef DEBUG
        std::cout << ra.hms_str() << std::endl;
        std::cout << decl.dms_str() << std::endl;
#endif
        if(!double_is_close(ra.rad(),
                            (9.0_hour + 34.0_minute + 53.4_second).rad(), 0.0,
                            (1.0_second).rad())) {
            return false;
        }
        if(!double_is_close(decl.rad(),
                            (19.0_deg + 32.0_arcmin + 8.52_arcsec).rad(), 0.0,
                            (1.0_arcsec).rad())) {
            return false;
        }
    }
    std::cout << "OK!" << std::endl;

    return true;
}

bool test_earth()
{
    std::cout << "Earth: Nutation... ";
    {
        // [Peter11] p.77
        Earth earth{Date{1988, 9, 1}};
        Angle nutation_longitude{earth.GetNutationLongitude()};
        Angle nutation_obliquity{earth.GetNutationObliquity()};
#ifdef DEBUG
        std::cout << "- Nutation in Longitude: " << nutation_longitude.arcsec()
                  << std::endl;
        std::cout << "- Nutation in Obliquity: " << nutation_obliquity.arcsec()
                  << std::endl;
#endif
        if(!double_is_close(nutation_longitude.rad(), (5.5_arcsec).rad(), 0.0,
                            (0.5_arcsec).rad())) {
            return false;
        }
        if(!double_is_close(nutation_obliquity.rad(), (9.2_arcsec).rad(), 0.0,
                            (0.5_arcsec).rad())) {
            return false;
        }
    }
    {
        // https://www.astro.com/swisseph/ae/2000/ae_2018d.pdf
        Earth earth{Date{2018, 8, 1}};
        Angle nutation_longitude{earth.GetNutationLongitude()};
        Angle nutation_obliquity{earth.GetNutationObliquity()};
#ifdef DEBUG
        std::cout << std::endl << std::fixed << std::setprecision(12);
        std::cout << "- Nutation in Longitude: " << nutation_longitude.arcsec()
                  << std::endl;
        std::cout << "- Nutation in Obliquity: " << nutation_obliquity.arcsec()
                  << std::endl;
#endif
        if(!double_is_close(nutation_longitude.rad(), (-13.0_arcsec).rad(), 0.0,
                            (0.5_arcsec).rad())) {
            return false;
        }
    }
    std::cout << "OK!" << std::endl;

    std::cout << "Earth: Mean Obliquity... ";
    {
        // [Peter11] p.52
        Earth earth{Date{2009, 7, 6}};
        Angle obliquity_mean{earth.GetObliquityMean()};
#ifdef DEBUG
        std::cout << std::endl << std::fixed << std::setprecision(12);
        std::cout << "- Mean Obliquity: " << obliquity_mean.deg() << std::endl;
#endif
        if(!double_is_close(obliquity_mean.rad(), (23.438054979133_deg).rad(),
                            1.0e-9, 0.0)) {
            return false;
        }
    }
    std::cout << "OK!" << std::endl;

    std::cout << "Earth: Obliquity... ";
    {
        // https://www.astro.com/swisseph/ae/2000/ae_2018d.pdf
        // [Peter11] p.54
        Earth earth{Date{2018, 8, 1}};
        Angle obliquity{earth.GetObliquity()};
#ifdef DEBUG
        std::cout << std::endl << std::fixed << std::setprecision(12);
        std::cout << "- Obliquity: " << obliquity.dms_str() << std::endl;
#endif
        if(!double_is_close(obliquity.rad(),
                            (23.0_deg + 26.0_arcmin + 7.0_arcsec).rad(), 0.0,
                            (1.0_arcsec).rad())) {
            return false;
        }
    }
    std::cout << "OK!" << std::endl;

    return true;
}

bool test_sun()
{
    std::cout << "Sun: Position... ";
    {
        // [Peter11] p.105
        Sun sun{Date{2003, 7, 27}};
        Coordinate coord;
        coord = sun.GetPosition();
        Angle ra{coord.GetEquatorialRightAscension()};
        Angle decl{coord.GetEquatorialDeclination()};
#ifdef DEBUG
        std::cout << ra.hms_str() << std::endl;
        std::cout << decl.dms_str() << std::endl;
#endif
        if(!double_is_close(ra.rad(),
                            (8.0_hour + 23.0_minute + 33.0_second).rad(), 0.0,
                            (15.0_second).rad())) {
            return false;
        }
        if(!double_is_close(decl.rad(),
                            (19.0_deg + 21.0_arcmin + 16.0_arcsec).rad(), 0.0,
                            (10.0_arcmin).rad())) {
            return false;
        }
    }
    std::cout << "OK!" << std::endl;

#if 0
    {
        // [Peter11] p.109
        Sun sun{Date(1988, 7, 27)};
    }

    {
        // [Jean99] p.165
        Sun sun{Date(1992, 10, 13)};
    }
#endif
    return true;
}

bool test_internal()
{
    if(!test_julian_date()) {
        return false;
    }
    if(!test_earth()) {
        return false;
    }
    if(!test_coordinate()) {
        return false;
    }
    if(!test_sun()) {
        return false;
    }

    return true;
}
