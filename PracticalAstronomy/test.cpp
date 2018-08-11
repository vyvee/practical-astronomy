#include "test.h"

#include "coordinate.h"
#include "date.h"
#include "earth.h"
#include "radian.h"
#include "solver.h"
#include "sun.h"

#include <cmath>

#define VERBOSE

#ifdef VERBOSE
#include <iostream>
#endif

using namespace PA;

static bool is_close(double x1, double x2, double rel_tol, double abs_tol = 0.0)
{
    return fabs(x1 - x2) <= fmax(rel_tol * fmax(fabs(x1), fabs(x2)), abs_tol);
}

bool test_julian_date()
{
    std::cout << "Date: TT <-> Julian Date... ";
    {
        Date date;

        struct {
            double julian_date_1;
            double julian_date_2;
        } const_date_test_data[] = {
            {EpochJ1900, 2415020.0}, {EpochJ2000, 2451545.0},
            /*
            {Date::J2010, 2455196.5},
             */
        };
        for(auto& td : const_date_test_data) {
            if(!is_close(td.julian_date_1, td.julian_date_2, 1.0e-9, 0.0)) {
                return false;
            }
        }

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
            date.SetCalendarTT(td.year, td.month, td.day);
            if(!date.GetJulianDate(&julian_date)) {
                return false;
            }
            if(!is_close(julian_date, td.julian_date, 1.0e-9, 0.0)) {
                return false;
            }
            int year, month;
            double day;
            date.SetJulianDate(td.julian_date);
            if(!date.GetCalendarTT(&year, &month, &day)) {
                return false;
            }
            if(!(year == td.year && month == td.month &&
                 is_close(day, td.day, 1.0e-9, 0.0))) {
                return false;
            }
        }
    }
    std::cout << "OK!" << std::endl;

    std::cout << "Date: Delta-T... ";
    {
        struct {
            int year;
            double delta_t;
            double standard_error;
        } delta_t_test_data[] = {
            /* {-500, 17190, 430}, */
            {-400, 15530, 390},
            {-300, 14080, 360},
            {-200, 12790, 330},
            {-100, 11640, 290},
            {0, 10580, 260},
            {100, 9600, 240},
            {200, 8640, 210},
            {300, 7680, 180},
            {400, 6700, 160},
            {500, 5710, 140},
            {600, 4740, 120},
            {700, 3810, 100},
            {800, 2960, 80},
            {900, 2200, 70},
            {1000, 1570, 55},
            {1100, 1090, 40},
            {1200, 740, 30},
            {1300, 490, 20},
            {1400, 320, 20},
            {1500, 200, 20},
            {1600, 120, 20},
            {1700, 9, 5},
            {1750, 13, 2},
            {1800, 14, 1},
            {1850, 7, 1 /*<1.0*/},
            {1900, -3, 1 /*<1.0*/},
            {1950, 29, 0.1 /*<0.1*/},
            {1955, +31.1, 1.0},
            {1960, +33.2, 1.0},
            {1965, +35.7, 1.0},
            {1970, +40.2, 1.0},
            {1975, +45.5, 1.0},
            {1980, +50.5, 1.0},
            {1985, +54.3, 1.0},
            {1990, +56.9, 1.0},
            {1995, +60.8, 1.0},
            {2000, +63.8, 1.0},
            {2005, +64.7, 1.0},
            {2010, +66.0, 1.0},
            {2015, +67.6, 1.0},
        };
        for(auto& td : delta_t_test_data) {
            Date date{td.year, 1, 1};
            if(std::fabs(date.GetDeltaT() - td.delta_t) > 5.0) {
                std::cout << td.year << '\t' << date.GetDeltaT() << '\t'
                          << date.GetDeltaT() - td.delta_t << std::endl;
                return false;
            }
        }
    }
    std::cout << "OK!" << std::endl;

    return true;
}

#if 0
bool test_coordinate()
{
    std::cout << "Coordinate: EC -> EQ... ";
    {
        // [Peter11] p.54
        Date date{2009, 7, 6};
        Coordinate coord;
        coord.SetEcliptic(139.686111_deg, 4.875278_deg, date.GetJulianDate());
        Degree ra{coord.GetEquatorialRightAscension()};
        Degree decl{coord.GetEquatorialDeclination()};
#if 0
        std::cout << ra.hms_str() << std::endl;
        std::cout << decl.dms_str() << std::endl;
#endif
        if(!is_close(ra.Deg(), (9.0_hour + 34.0_minute + 53.4_second).Deg(),
                     0.0, (1.0_second).Deg())) {
            return false;
        }
        if(!is_close(decl.Deg(), (19.0_deg + 32.0_arcmin + 8.52_arcsec).Deg(),
                     0.0, (1.0_arcsec).Deg())) {
            return false;
        }
    }
    std::cout << "OK!" << std::endl;

    return true;
}
#endif

bool test_earth()
{
    std::cout << "Earth: Nutation... ";
    {
        Date date{1987, 4, 10};
        // [Jean99] p.148
        // https://www.erdrotation.de/sofa_001.html
        Earth earth{date.GetJulianDate()};
        double nutation_longitude{earth.GetNutationLongitude()};
        double nutation_obliquity{earth.GetNutationObliquity()};
#if 0
        std::cout << nutation_longitude.ArcSecStr(3) << std::endl;
        std::cout << nutation_obliquity.ArcSecStr(3) << std::endl;
#endif
        if(!is_close(nutation_longitude, -0.0000183306021758759992, 0.0,
                     0.003_arcsec)) {
            return false;
        }
        if(!is_close(nutation_obliquity, 0.0000457927984038134921, 0.0,
                     0.003_arcsec)) {
            return false;
        }
    }
    {
        Date date{1988, 9, 1};
        // [Peter11] p.77
        Earth earth{date.GetJulianDate()};
        double nutation_longitude{earth.GetNutationLongitude()};
        double nutation_obliquity{earth.GetNutationObliquity()};
#if 0
        std::cout << nutation_longitude.ArcSecStr() << std::endl;
        std::cout << nutation_obliquity.ArcSecStr() << std::endl;
#endif
        if(!is_close(nutation_longitude, 5.1_arcsec, 0.0, 0.1_arcsec)) {
            return false;
        }
        if(!is_close(nutation_obliquity, 9.2_arcsec, 0.0, 0.1_arcsec)) {
            return false;
        }
    }
    {
        Date date{2018, 8, 1};
        // https://www.astro.com/swisseph/ae/2000/ae_2018d.pdf
        Earth earth{date.GetJulianDate()};
        double nutation_longitude{earth.GetNutationLongitude()};
        // double nutation_obliquity{earth.GetNutationObliquity()};
#if 0
        std::cout << nutation_longitude.ArcSecStr() << std::endl;
        std::cout << nutation_obliquity.ArcSecStr() << std::endl;
#endif
        if(!is_close(nutation_longitude, -13.0_arcsec, 0.0, 0.5_arcsec)) {
            return false;
        }
    }
    std::cout << "OK!" << std::endl;

    std::cout << "Earth: Mean Obliquity... ";
    {
        Date date{1987, 4, 10.0};
        // [Jean99] p.148
        Earth earth{date.GetJulianDate()};
        double obliquity_mean{earth.GetObliquityMean()};
#if 0
        std::cout << obliquity_mean.DMSStr() << std::endl;
#endif
        if(!is_close(obliquity_mean, 23_deg + 26_arcmin + 27.41_arcsec, 0.0,
                     0.01_arcsec)) {
            return false;
        }
    }
    std::cout << "OK!" << std::endl;

    std::cout << "Earth: Obliquity... ";
    {
        Date date{1987, 4, 10.0};
        // [Jean99] p.148
        Earth earth{date.GetJulianDate()};
        double obliquity{earth.GetObliquity()};
#if 0
        std::cout << obliquity.DMSStr() << std::endl;
#endif
        if(!is_close(obliquity, 23_deg + 26_arcmin + 36.85_arcsec, 0.0,
                     0.01_arcsec)) {
            return false;
        }
    }
    {
        Date date{2018, 8, 1};
        // https://www.astro.com/swisseph/ae/2000/ae_2018d.pdf
        // [Peter11] p.54
        Earth earth{date.GetJulianDate()};
        double obliquity{earth.GetObliquity()};
#if 0
        std::cout << obliquity.DMSStr() << std::endl;
#endif
        if(!is_close(obliquity, 23_deg + 26_arcmin + 7_arcsec, 0.0, 1_arcsec)) {
            return false;
        }
    }
    std::cout << "OK!" << std::endl;

    return true;
}

#if 0
bool test_sun()
{
    std::cout << "Sun: Position... ";
    {
        // [Peter11] p.105
        Sun sun{Date{2003, 7, 27}};
        Coordinate coord;
        coord = sun.GetPosition();
        Degree ra{coord.GetEquatorialRightAscension()};
        Degree decl{coord.GetEquatorialDeclination()};
#ifdef DEBUG
        std::cout << ra.hms_str() << std::endl;
        std::cout << is_close << std::endl;
#endif
        if(!is_close(ra.Deg(), (8.0_hour + 23.0_minute + 33.0_second).Deg(),
                     0.0, (15.0_second).Deg())) {
            return false;
        }
        if(!is_close(decl.Deg(), (19.0_deg + 21.0_arcmin + 16.0_arcsec).Deg(),
                     0.0, (1.0_arcmin).Deg())) {
            return false;
        }
    }

    {
        // [Peter11] p.109
        Sun sun{Date(1988, 7, 27)};
        Coordinate coord;
        coord = sun.GetPosition();
        Degree ra{coord.GetEquatorialRightAscension()};
        Degree decl{coord.GetEquatorialDeclination()};
#ifdef DEBUG
        std::cout << ra.hms_str() << std::endl;
        std::cout << decl.dms_str(is_close
#endif
        if(!is_close(ra.Deg(),
                            (8.0_hour + 26.0_minute + 3.0_second).Deg(), 0.0,
                            (15.0_second).Deg())) {
            return false;
            }
        if(!is_close(decl.Deg(),
                                    (19.0_deg + 12.0_arcmin + 43.0_arcsec)
                                        .Deg(),
                                    0.0, (1.0_arcmin).Deg()))
            {
            return false;
            }
    }

    {
        // [Jean99] p.165
        // http://www.astro.com/swisseph/ae/1900/ae_1992d.pdf
        Sun sun{Date(1992, 10, 13)};
        Coordinate coord;
        coord = sun.GetPosition();
        Degree ra{coord.GetEquatorialRightAscension()};
        Degree decl{coord.GetEquatorialDeclination()};
#ifdef DEBUG
        std::cout << coord.GetEclipticLongitude().dms_str() << std::endl;
        std::cout << ra.hms_str() << std::endl;
        std::cout << decl.dms_str() << std::endl;
#endif
        if(!is_close(coord.GetEclipticLongitude().Deg(),
                     (199.0_deg + 54.0_arcmin + 24.0_arcsec).Deg(), 0.0,
                     (15.0_arcsec).Deg())) {
            return false;
        }
        if(!is_close(decl.Deg(), (-(7.0_deg + 47.0_arcmin)).Deg(), 0.0,
                     (1.0_arcmin).Deg())) {
            return false;
        }
    }
    std::cout << "OK!" << std::endl;

    return true;
}
#endif

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
            if(!is_close(td.m, e - td.ecc * std::sin(e), 1.0e-9, 0.0)) {
                return false;
            }
        }
        for(double ecc_100 = 0.0; ecc_100 < 100.0; ecc_100 += 1.0) {
            for(double m_100 = 0.0; m_100 < 700.0; m_100 += 1.0) {
                double e{Solver::solve_kepler(ecc_100 / 100.0, m_100 / 100.0)};
                if(!is_close(m_100 / 100.0, e - ecc_100 / 100.0 * std::sin(e),
                             1.0e-9, 0.0)) {
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
    /*
    if(!test_coordinate()) {
        return false;
    }
     */
    /*
    if(!test_sun()) {
        return false;
    }
     */
    if(!test_solver()) {
        return false;
    }

    return true;
}
