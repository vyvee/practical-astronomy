#include "test.h"

#include "coordinate.h"
#include "date.h"
#include "earth.h"
#include "solver.h"
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
    std::cout << "Date Conversion: TT <-> Julian Date... ";
    Date date;
    struct {
        int year;
        int month;
        double day;
        double julian_date;
    } date_test_data[] = {
        {1899, 12, 31.5, 2415020.0},  {2000, 1, 1.5, 2451545.0},
        {2009, 12, 31, 2455196.5},    {-4712, 1, 1.5, 0.0},
        {2009, 6, 19.75, 2455002.25},
    };
    for(auto& td : date_test_data) {
        double julian_date;
        date.SetTT(td.year, td.month, td.day);
        if(!date.GetJulianDate(&julian_date)) {
            return false;
        }
        if(!double_is_close(julian_date, td.julian_date, 1.0e-9, 0.0)) {
            return false;
        }
        int year, month;
        double day;
        date.SetJulianDate(td.julian_date);
        if(!date.GetTT(&year, &month, &day)) {
            return false;
        }
        if(!(year == td.year && month == td.month &&
             double_is_close(day, td.day, 1.0e-9, 0.0))) {
            return false;
        }
    }
    std::cout << "OK!" << std::endl;

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
                            (1.0_arcmin).rad())) {
            return false;
        }
    }

    {
        // [Peter11] p.109
        Sun sun{Date(1988, 7, 27)};
        Coordinate coord;
        coord = sun.GetPosition();
        Angle ra{coord.GetEquatorialRightAscension()};
        Angle decl{coord.GetEquatorialDeclination()};
#ifdef DEBUG
        std::cout << ra.hms_str() << std::endl;
        std::cout << decl.dms_str() << std::endl;
#endif
        if(!double_is_close(ra.rad(),
                            (8.0_hour + 26.0_minute + 3.0_second).rad(), 0.0,
                            (15.0_second).rad())) {
            return false;
        }
        if(!double_is_close(decl.rad(),
                            (19.0_deg + 12.0_arcmin + 43.0_arcsec).rad(), 0.0,
                            (1.0_arcmin).rad())) {
            return false;
        }
    }

    {
        // [Jean99] p.165
        // http://www.astro.com/swisseph/ae/1900/ae_1992d.pdf
        Sun sun{Date(1992, 10, 13)};
        Coordinate coord;
        coord = sun.GetPosition();
        Angle ra{coord.GetEquatorialRightAscension()};
        Angle decl{coord.GetEquatorialDeclination()};
#ifdef DEBUG
        std::cout << coord.GetEclipticLongitude().dms_str() << std::endl;
        std::cout << ra.hms_str() << std::endl;
        std::cout << decl.dms_str() << std::endl;
#endif
        if(!double_is_close(coord.GetEclipticLongitude().rad(),
                            (199.0_deg + 54.0_arcmin + 24.0_arcsec).rad(), 0.0,
                            (15.0_arcsec).rad())) {
            return false;
        }
        if(!double_is_close(decl.rad(), (-(7.0_deg + 47.0_arcmin)).rad(), 0.0,
                            (1.0_arcmin).rad())) {
            return false;
        }
    }
    std::cout << "OK!" << std::endl;

    return true;
}

bool test_solver()
{
    std::cout << "Solver: Kepler... ";
    {
        struct {
            double ecc;
            double m;
        } kepler_test_data[] = {
            {0.0167076, -40.4695}, {0.0167139, -134.702}, {0.0167121, -108.228},
            {0.9673, 5.31},        {0.9673, 6.108598},    {0.98, 6.28},
            {0.99, 6.29},
        };
        for(auto& td : kepler_test_data) {
            double e{Solver::solve_kepler(td.ecc, td.m)};
            if(!double_is_close(td.m, e - td.ecc * std::sin(e), 1.0e-9, 0.0)) {
                return false;
            }
        }
        for(double ecc_100 = 0.0; ecc_100 < 100.0; ecc_100 += 1.0) {
            for(double m_100 = 0.0; m_100 < 700.0; m_100 += 1.0) {
                double e{Solver::solve_kepler(ecc_100 / 100.0, m_100 / 100.0)};
                if(!double_is_close(m_100 / 100.0,
                                    e - ecc_100 / 100.0 * std::sin(e), 1.0e-9,
                                    0.0)) {
                    return false;
                }
            }
        }
    }
    std::cout << "OK!" << std::endl;

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
    if(!test_solver()) {
        return false;
    }

    return true;
}
