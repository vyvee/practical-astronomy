#ifndef DATE_H_
#define DATE_H_

#include <cmath>
#include <tuple>

#include "misc.h"

namespace PA
{
class Date
{
   public:
    static const double J1900_JAN_0_5;
    static const double J2000_JAN_1_5;
    static const double J2010;

    constexpr Date(){};
    constexpr Date(int year, int month = 1, double day = 1.0)
        : calendar_tt_is_valid_(true)
        , calendar_tt_year_(year)
        , calendar_tt_month_(month)
        , calendar_tt_day_(day)
    {
    }
    constexpr Date(int year,
                   int month,
                   int day,
                   int hour,
                   int minute = 0,
                   double second = 0.0)
        : calendar_tt_is_valid_(true)
        , calendar_tt_year_(year)
        , calendar_tt_month_(month)
        , calendar_tt_day_(day + hour / 24.0 + minute / (24 * 60.0) +
                           second / (24 * 60.0 * 60.0))
    {
    }
    constexpr Date(double julian_date)
        : julian_date_is_valid_(true), julian_date_(julian_date)
    {
    }

    constexpr void SetCalendarTT(int year, int month = 1, double day = 1.0);
    constexpr void SetCalendarTT(int year,
                                 int month,
                                 int day,
                                 int hour,
                                 int minute = 0,
                                 double second = 0.0);
    constexpr void SetJulianDate(double julian_date);

    constexpr bool GetCalendarTT(int *p_year,
                                 int *p_month,
                                 double *p_day) const;
    constexpr bool GetJulianDate(double *p_julian_date) const;
    constexpr double GetJulianDate() const;
    constexpr double GetDeltaT() const;

    constexpr operator double() const
    {
        return GetJulianDate();
    }

    std::string GetTTString() const;

   private:
    static constexpr bool JulianDateFromCalendar(double *p_jd,
                                                 int year,
                                                 int month,
                                                 double day);
    static constexpr bool CalendarFromJulianDate(int *p_year,
                                                 int *p_month,
                                                 double *p_day,
                                                 double jd);
    static constexpr bool DeltaTFromCalendar(double *p_delta_t,
                                             int year,
                                             int month,
                                             double day);

    constexpr void ComputeCalendarTT() const;
    mutable bool calendar_tt_is_valid_{false};
    mutable int calendar_tt_year_{-1};
    mutable int calendar_tt_month_{-1};
    mutable double calendar_tt_day_{-1.0};

    constexpr void ComputeJulianDate() const;
    mutable bool julian_date_is_valid_{false};
    mutable double julian_date_{-1.0};

    constexpr void ComputeDeltaT() const;
    mutable bool delta_t_is_valid_{false};
    mutable double delta_t_{-1.0};
};

inline const double Date::J1900_JAN_0_5{Date{1899, 12, 31.5}.GetJulianDate()};
inline const double Date::J2000_JAN_1_5{Date{2000, 1, 1.5}.GetJulianDate()};
inline const double Date::J2010{Date{2009, 12, 31.0}.GetJulianDate()};

constexpr void Date::SetCalendarTT(int year, int month, double day)
{
    calendar_tt_year_ = year;
    calendar_tt_month_ = month;
    calendar_tt_day_ = day;
    calendar_tt_is_valid_ = true;

    julian_date_is_valid_ = false;
}

constexpr void Date::SetCalendarTT(int year,
                                   int month,
                                   int day,
                                   int hour,
                                   int minute,
                                   double second)
{
    calendar_tt_year_ = year;
    calendar_tt_month_ = month;
    calendar_tt_day_ =
        day + hour / 24.0 + minute / (24 * 60.0) + second / (24 * 60.0 * 60.0);
    calendar_tt_is_valid_ = true;

    julian_date_is_valid_ = false;
}

constexpr void Date::SetJulianDate(double julian_date)
{
    julian_date_ = julian_date;
    julian_date_is_valid_ = true;

    calendar_tt_is_valid_ = false;
}

constexpr bool Date::GetCalendarTT(int *p_year,
                                   int *p_month,
                                   double *p_day) const
{
    if(!calendar_tt_is_valid_) {
        ComputeCalendarTT();
        if(!calendar_tt_is_valid_) {
            return false;
        }
    }

    *p_year = calendar_tt_year_;
    *p_month = calendar_tt_month_;
    *p_day = calendar_tt_day_;
    return true;
}

constexpr bool Date::GetJulianDate(double *p_julian_date) const
{
    if(!julian_date_is_valid_) {
        ComputeJulianDate();
        if(!julian_date_is_valid_) {
            return false;
        }
    }

    *p_julian_date = julian_date_;
    return true;
}

constexpr double Date::GetJulianDate() const
{
    double jd{0.0};
    if(!GetJulianDate(&jd)) {
        return -1.0;
    }
    return jd;
}

constexpr double PA::Date::GetDeltaT() const
{
    if(!delta_t_is_valid_) {
        ComputeDeltaT();
        if(!delta_t_is_valid_) {
            return 0.0;
        }
    }

    return delta_t_;
}

constexpr bool Date::JulianDateFromCalendar(double *p_jd,
                                            int year,
                                            int month,
                                            double day)
{
    // Reference: [Peter11] Section 5
    const auto [y, m]{(month < 3) ? std::tuple(year - 1, month + 12) :
                                    std::tuple(year, month)};
    const bool is_gregorian{
        year > 1582 ||
        (year == 1582 && (month > 10 || (month == 10 && day >= 15)))};
    const int b{[is_gregorian, y] {
        if(is_gregorian) {
            int a{static_cast<int>(trunc(y / 100))};
            return 2 - a + static_cast<int>(trunc(a / 4));
        } else {
            return 0;
        }
    }()};
    const int c{static_cast<int>((y < 0) ? trunc((365.25 * y) - 0.75) :
                                           trunc(365.25 * y))};
    const int d{static_cast<int>(trunc(30.6001 * (m + 1)))};
    *p_jd = b + c + d + day + 1720994.5;
    return (*p_jd >= 0.0);
}

constexpr bool PA::Date::CalendarFromJulianDate(int *p_year,
                                                int *p_month,
                                                double *p_day,
                                                double jd)
{
    // Reference: [Peter11] Section 5
    if(jd < 0.0) {
        return false;
    }
    jd += 0.5;
    const int i{static_cast<int>(trunc(jd))};
    const double f{jd - i};
    const int b{[i] {
        if(i > 2299160) {
            int a{static_cast<int>(trunc((i - 1867216.25) / 36524.25))};
            return static_cast<int>(i + a - trunc(a / 4.0) + 1);
        } else {
            return i;
        }
    }()};
    int c{b + 1524};
    int d{static_cast<int>(trunc((c - 122.1) / 365.25))};
    int e{static_cast<int>(trunc(365.25 * d))};
    int g{static_cast<int>(trunc((c - e) / 30.6001))};
    *p_day = c - e + f - trunc(30.6001 * g);
    *p_month = (g <= 13) ? (g - 1) : (g - 13);
    *p_year = (*p_month >= 3) ? (d - 4716) : (d - 4715);
    return true;
}

constexpr bool Date::DeltaTFromCalendar(double *p_delta_t,
                                        int year,
                                        int month,
                                        double /*day*/)
{
    // Many formulas exist. We implement the one available from NASA.
    // Error can be up to 2.0s for recent years. Not ideal but usable.
    // References:
    // - https://eclipse.gsfc.nasa.gov/SEhelp/deltatpoly2004.html
    // - [Graph: Actual and Prediction]
    //   http://asa.usno.navy.mil/SecK/DeltaT.html
    // - [Actual Data] ftp://maia.usno.navy.mil/ser7/deltat.data
    // - http://stjarnhimlen.se/comp/time.html
    // - http://astro.altspu.ru/~aw/DeltaT/
    // - https://www.hindawi.com/journals/jas/2014/480964/
    // TODO:
    // - Check possibility of doing it with Julian Date, with advantage to
    // get a
    //   more accurate decimal year.
    double y{year + (month - 0.5) / 12.0};
    // double y{static_cast<double>(year_)};
    if(y >= +2150) {
        double u{(y - 1820) / 100.0};
        *p_delta_t = -20.0 + 32 * u * u;
    } else if(y >= +2050) {
        double u{(y - 1820) / 100.0};
        *p_delta_t = -20.0 + 32 * u * u - 0.5628 * (2150 - y);
    } else if(y >= +2005) {
        *p_delta_t = horner_polynomial({62.92, 0.32217, 0.005589}, y - 2000);
    } else if(y >= +1986) {
        *p_delta_t = horner_polynomial(
            {63.86, 0.3345, -0.060374, 0.0017275, 0.000651814, 0.00002373599},
            y - 2000.0);
    } else if(y >= +1961) {
        *p_delta_t = horner_polynomial(
            {45.45, 1.067, -1.0 / 260.0, -1.0 / 718.0}, y - 1975.0);
    } else if(y >= +1941) {
        *p_delta_t = horner_polynomial(
            {29.07, 0.407, -1.0 / 233.0, 1.0 / 2547.0}, y - 1950.0);
    } else if(y >= +1920) {
        *p_delta_t = horner_polynomial({21.20, 0.84493, -0.076100, 0.0020936},
                                       y - 1920.0);
    } else if(y >= +1900) {
        *p_delta_t = horner_polynomial(
            {-2.79, 1.494119, -0.0598939, 0.0061966, -0.000197}, y - 1900.0);
    } else if(y >= +1860) {
        *p_delta_t = horner_polynomial(
            {7.62, 0.5737, -0.251754, 0.01680668, -0.0004473624}, y - 1860.0);
    } else if(y >= +1800) {
        *p_delta_t = horner_polynomial(
            {13.72, -0.332447, 0.0068612, 0.0041116, -0.00037436, 0.0000121272,
             -0.0000001699, 0.000000000875},
            y - 1800.0);
    } else if(y >= +1700) {
        *p_delta_t = horner_polynomial(
            {8.83, 0.1603, -0.0059285, 0.00013336, -1.0 / 1174000.0},
            y - 1700.0);
    } else if(y >= +1600) {
        *p_delta_t = horner_polynomial({120.0, -0.9808, -0.01532, 1 / 7129.0},
                                       y - 1600.0);
    } else if(y >= +500) {
        *p_delta_t = horner_polynomial({1574.2, -556.01, 71.23472, 0.319781,
                                        -0.8503463, -0.005050998, 0.0083572073},
                                       (y - 1000.0) / 100.0);
    } else if(y >= -500) {
        *p_delta_t = horner_polynomial({10583.6, -1014.41, 33.78311, -5.952053,
                                        -0.1798452, 0.022174192, 0.0090316521},
                                       y / 100.0);
    } else {
        double u{(y - 1820) / 100.0};
        *p_delta_t = -20.0 + 32 * u * u;
    }

    return true;
}

constexpr void Date::ComputeCalendarTT() const
{
    if(julian_date_is_valid_) {
        calendar_tt_is_valid_ =
            CalendarFromJulianDate(&calendar_tt_year_, &calendar_tt_month_,
                                   &calendar_tt_day_, julian_date_);
    }
}

constexpr void Date::ComputeJulianDate() const
{
    if(calendar_tt_is_valid_) {
        julian_date_is_valid_ =
            JulianDateFromCalendar(&julian_date_, calendar_tt_year_,
                                   calendar_tt_month_, calendar_tt_day_);
    }
}

constexpr void Date::ComputeDeltaT() const
{
    if(calendar_tt_is_valid_) {
        delta_t_is_valid_ = DeltaTFromCalendar(
            &delta_t_, calendar_tt_year_, calendar_tt_month_, calendar_tt_day_);
    }
}

}  // namespace PA

#endif  // DATE_H_
