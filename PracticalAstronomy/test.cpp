#include "test.h"

#include <cmath>

#include "date.h"
#include "moon.h"
#include "observer.h"
#include "radian.h"
#include "solver.h"

#define VERBOSE

#ifdef VERBOSE
#include <iostream>
#endif

using namespace PA;

static void _expect_abort() {
  std::cout << "Abort." << std::endl;
  std::exit(1);
}

static void _expect_double_helper(double v1, double v2, double rel_tol,
                                  double abs_tol, const char* function_name,
                                  const char* filename, int line_no) {
  if (!(fabs(v1 - v2) <= fmax(rel_tol * fmax(fabs(v1), fabs(v2)), abs_tol))) {
    std::cout << "Error: (" << filename << ":" << line_no << ":"
              << function_name << ") " << v1 << " != " << v2 << std::endl;
    _expect_abort();
  }
}

#define expect_double(v1, v2, rel_tol, abs_tol)                           \
  _expect_double_helper(v1, v2, rel_tol, abs_tol, __FUNCTION__, __FILE__, \
                        __LINE__)

static void _expect_bool_helper(bool v1, bool v2, const char* function_name,
                                const char* filename, int line_no) {
  if (v1 != v2) {
    std::cout << "Error: (" << filename << ":" << line_no << ":"
              << function_name << ")" << (v2 ? true : false) << std::endl;
    _expect_abort();
  }
}

#define expect_bool(v1, v2) \
  _expect_bool_helper(v1, v2, __FUNCTION__, __FILE__, __LINE__)

static void test_julian_date() {
  std::cout << "Date: TT <-> Julian Date... ";
  {
    Date date;

    struct {
      double julian_date_1;
      double julian_date_2;
    } const_date_test_data[] = {
        {EpochJ1900, 2415020.0},
        {EpochJ2000, 2451545.0},
    };
    for (auto& td : const_date_test_data) {
      expect_double(td.julian_date_1, td.julian_date_2, 1.0e-9, 0.0);
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

    for (auto& td : date_test_data) {
      double julian_date;
      date.SetCalendarTT(td.year, td.month, td.day);
      expect_bool(date.GetJulianDate(&julian_date), true);
      expect_double(julian_date, td.julian_date, 1.0e-9, 0.0);
      int year, month;
      double day;
      date.SetJulianDate(td.julian_date);
      expect_bool(date.GetCalendarTT(&year, &month, &day), true);
      expect_bool(year == td.year, true);
      expect_bool(month == td.month, true);
      expect_double(day, td.day, 1.0e-9, 0.0);
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
        {-400, 15530, 390},    {-300, 14080, 360},     {-200, 12790, 330},
        {-100, 11640, 290},    {0, 10580, 260},        {100, 9600, 240},
        {200, 8640, 210},      {300, 7680, 180},       {400, 6700, 160},
        {500, 5710, 140},      {600, 4740, 120},       {700, 3810, 100},
        {800, 2960, 80},       {900, 2200, 70},        {1000, 1570, 55},
        {1100, 1090, 40},      {1200, 740, 30},        {1300, 490, 20},
        {1400, 320, 20},       {1500, 200, 20},        {1600, 120, 20},
        {1700, 9, 5},          {1750, 13, 2},          {1800, 14, 1},
        {1850, 7, 1 /*<1.0*/}, {1900, -3, 1 /*<1.0*/}, {1950, 29, 0.1 /*<0.1*/},
        {1955, +31.1, 1.0},    {1960, +33.2, 1.0},     {1965, +35.7, 1.0},
        {1970, +40.2, 1.0},    {1975, +45.5, 1.0},     {1980, +50.5, 1.0},
        {1985, +54.3, 1.0},    {1990, +56.9, 1.0},     {1995, +60.8, 1.0},
        {2000, +63.8, 1.0},    {2005, +64.7, 1.0},     {2010, +66.0, 1.0},
        {2015, +67.6, 1.0},
    };
    for (auto& td : delta_t_test_data) {
      Date date{td.year, 1, 1};
      expect_double(date.GetDeltaT(), td.delta_t, 0.0, 5.0);
    }
  }
  std::cout << "OK!" << std::endl;
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

static void test_nutation_obliquity() {
  std::cout << "Earth: Nutation and Obliquity... ";

  {
    // [Jean99] p.148
    // https://www.erdrotation.de/sofa_001.html
    // https://www.astro.com/swisseph/ae/1900/ae_1987d.pdf
    Observer observer{Date{1987, 4, 10}.GetJulianDate()};

    observer.SetNutationAlgorithm(
        Observer::NutationAlgorithm::kIAU1980MeeusTruncated);
    expect_double(observer.GetNutationLongitude(), -4.0_arcsec, 0.0, 1_arcsec);

    observer.SetNutationAlgorithm(Observer::NutationAlgorithm::kIAU1980);
    expect_double(observer.GetNutationLongitude(), -3.788_arcsec, 0.0,
                  0.001_arcsec);
    expect_double(observer.GetNutationObliquity(), 9.443_arcsec, 0.0,
                  0.001_arcsec);
    expect_double(observer.GetObliquityMean(),
                  23_deg + 26_arcmin + 27.41_arcsec, 0.0, 0.01_arcsec);
    expect_double(observer.GetObliquity(), 23_deg + 26_arcmin + 36.85_arcsec,
                  0.0, 0.01_arcsec);

    // However, according to IAU2000B:
    // - Nutation Longitude: -3".781
    // - Nutation Obliquity: 9".445
  }

  {
    // [Peter11] p.77
    Observer observer{Date{1988, 9, 1}.GetJulianDate()};
    expect_double(observer.GetNutationLongitude(), 5.1_arcsec, 0.0, 0.1_arcsec);
    expect_double(observer.GetNutationObliquity(), 9.2_arcsec, 0.0, 0.1_arcsec);
  }

  {
    // [Peter11] pp.51-52
    Observer observer{Date{2009, 7, 6}.GetJulianDate()};
    expect_double(observer.GetObliquityMean(), 23_deg + 26_arcmin + 17_arcsec,
                  0.0, 0.1_arcsec);
  }

  std::cout << "OK!" << std::endl;
}

static void test_sun() {
  std::cout << "Sun: Position... ";

  {
    // [Peter11] p.105
    Observer observer{Date{2003, 7, 27.0}.GetJulianDate()};
    expect_double(observer.GetApparentRightAscension(Observer::Body::kSun),
                  8.0_h + 23.0_m + 33.0_s, 0.0, 1.0_s);
    expect_double(observer.GetApparentDeclination(Observer::Body::kSun),
                  19.0_deg + 21.0_arcmin + 16.0_arcsec, 0.0, 1.0_arcsec);
  }

  {
    // [Peter11] p.109
    Observer observer{Date{1988, 7, 27.0}.GetJulianDate()};
    expect_double(observer.GetApparentRightAscension(Observer::Body::kSun),
                  8.0_h + 26.0_m + 3.0_s, 0.0, 1.0_s);
    expect_double(observer.GetApparentDeclination(Observer::Body::kSun),
                  19.0_deg + 12.0_arcmin + 52.0_arcsec, 0.0, 1.0_arcsec);
  }

  {
    // [Jean99] p.165
    // http://www.astro.com/swisseph/ae/1900/ae_1992d.pdf
    Observer observer{Date{1992, 10, 13.0}.GetJulianDate()};
    expect_double(observer.GetLongitude(Observer::Body::kSun),
                  199.0_deg + 54.0_arcmin + 26.18_arcsec, 0.0, 0.01_arcsec);
    // - With Complete VSOP87 Theory: 199d 54' 21.56"
    // - From Swiss Ephemeris: 199d 54' 21.5909"
    expect_double(observer.GetApparentLongitude(Observer::Body::kSun),
                  199.0_deg + 54.0_arcmin + 21.56_arcsec, 0.0, 0.01_arcsec);
    expect_double(observer.GetApparentLatitude(Observer::Body::kSun),
                  0.72_arcsec, 0.0, 0.01_arcsec);
    expect_double(observer.GetRadiusVectorAU(Observer::Body::kSun), 0.99760853,
                  0.0, 0.00000001);
    expect_double(observer.GetApparentRightAscension(Observer::Body::kSun),
                  13.0_h + 13.0_m + 30.749_s, 0.0, 0.001_s);
    expect_double(observer.GetApparentDeclination(Observer::Body::kSun),
                  -(7.0_deg + 47.0_arcmin + 01.74_arcsec), 0.0, 0.01_arcsec);
  }

  std::cout << "OK!" << std::endl;
}

static void test_moon() {
  std::cout << "Moon: Position... ";

  {
    // [Peter11] p.166
    Date date{2003, 9, 1.0};
    Moon moon{date.GetJulianDate()};
    double apparent_longitude{moon.GetApparentLongitude()};
    double apparent_latitude{moon.GetApparentLatitude()};
    EarthObliquityOld earth_obliquity{date.GetJulianDate()};
    double obliquity{earth_obliquity.GetObliquity()};
    double apparent_ra{Coordinate::EclipticalToEquatorialRightAscension(
        apparent_longitude, apparent_latitude, obliquity)};
    double apparent_decl{Coordinate::EclipticalToEquatorialDeclination(
        apparent_longitude, apparent_latitude, obliquity)};
    expect_double(apparent_ra, 14.0_h + 12.0_m + 10.0_s, 0.0, 10.0_arcsec);
    expect_double(apparent_decl, -(11.0_deg + 34.0_arcmin + 52.0_arcsec), 0.0,
                  4.0_arcsec);
  }

  {
    // [Jean99] p.342
    Date date{1992, 4, 12.0};
    Moon moon{date.GetJulianDate()};
    double apparent_longitude{moon.GetApparentLongitude()};
    double apparent_latitude{moon.GetApparentLatitude()};
    expect_double(apparent_longitude, 133.0_deg + 10.0_arcmin + 0.0_arcsec, 0.0,
                  10.0_arcsec);
    expect_double(apparent_latitude, -(3.0_deg + 13.0_arcmin + 45.0_arcsec),
                  0.0, 4.0_arcsec);

    EarthObliquityOld earth_obliquity{date.GetJulianDate()};
    double obliquity{earth_obliquity.GetObliquity()};
    double apparent_ra{Coordinate::EclipticalToEquatorialRightAscension(
        apparent_longitude, apparent_latitude, obliquity)};
    double apparent_decl{Coordinate::EclipticalToEquatorialDeclination(
        apparent_longitude, apparent_latitude, obliquity)};
    expect_double(apparent_ra, 8.0_h + 58.0_m + 45.1_s, 0.0, 10.0_arcsec);
    expect_double(apparent_decl, 13.0_deg + 46.0_arcmin + 6.0_arcsec, 0.0,
                  4.0_arcsec);
  }

  std::cout << "OK!" << std::endl;
}

static void test_solver() {
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
    for (auto& td : kepler_test_data) {
      double e{Solver::solve_kepler(td.ecc, td.m)};
      expect_double(td.m, e - td.ecc * std::sin(e), 1.0e-9, 0.0);
    }
    for (double ecc_100 = 0.0; ecc_100 < 100.0; ecc_100 += 1.0) {
      for (double m_100 = 0.0; m_100 < 700.0; m_100 += 1.0) {
        double e{Solver::solve_kepler(ecc_100 / 100.0, m_100 / 100.0)};
        expect_double(m_100 / 100.0, e - ecc_100 / 100.0 * std::sin(e), 1.0e-9,
                      0.0);
      }
    }
  }
  std::cout << "OK!" << std::endl;
}

void test_internal() {
  // std::cout << RadToHMSStr(-(6_h + 7_m + 59.95_s), 0) << std::endl;
  // std::cout << RadToHMSStr(-(6_h + 7_m + 59.95_s), 1) << std::endl;
  // std::cout << RadToHMSStr(-(6_h + 7_m + 59.95_s), 2) << std::endl;
  // std::cout << RadToHMSStr(-(6_h + 7_m + 59.95_s), 3) << std::endl;
  // std::cout << RadToHMSStr(-(6_h + 59_m + 59.95_s), 1) << std::endl;

  test_julian_date();

  test_nutation_obliquity();
  test_sun();
  test_moon();
  test_solver();
}
