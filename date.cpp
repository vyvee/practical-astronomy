#include "date.h"

#include "misc.h"

#include <cmath>

constexpr double PA::Date::J1900_JAN_0_5;
constexpr double PA::Date::J2000_JAN_1_5;
constexpr double PA::Date::J2010;

PA::Date::Date()
{
}

PA::Date::Date(int year, int month, double day)
    : terrestrial_time_is_valid_(true), year_(year), month_(month), day_(day)
{
}

PA::Date::Date(double julian_date)
{
    julian_date_ = julian_date;
    julian_date_is_valid_ = true;
}

void PA::Date::SetTT(int year, int month, double day)
{
    year_ = year;
    month_ = month;
    day_ = day;
    terrestrial_time_is_valid_ = true;

    julian_date_is_valid_ = false;
}

void PA::Date::SetJulianDate(double julian_date)
{
    julian_date_ = julian_date;
    julian_date_is_valid_ = true;

    terrestrial_time_is_valid_ = false;
}

bool PA::Date::GetJulianDate(double *p_julian_date) const
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

double PA::Date::GetJulianDate() const
{
    bool status;
    double jd;
    status = GetJulianDate(&jd);
    if(!status) {
        return -1.0;
    }
    return jd;
}

bool PA::Date::GetTT(int *p_year, int *p_month, double *p_day) const
{
    if(!terrestrial_time_is_valid_) {
        ComputeTT();
        if(!terrestrial_time_is_valid_) {
            return false;
        }
    }

    *p_year = year_;
    *p_month = month_;
    *p_day = day_;
    return true;
}

void PA::Date::ComputeJulianDate() const
{
    if(terrestrial_time_is_valid_) {
        // Reference: [Peter11] Section 5
        int y, m;
        if(month_ < 3) {
            y = year_ - 1;
            m = month_ + 12;
        } else {
            y = year_;
            m = month_;
        }

        bool is_gregorian;
        is_gregorian =
            year_ > 1582 ||
            (year_ == 1582 && (month_ > 10 || (month_ == 10 && day_ >= 15)));

        int b;
        if(is_gregorian) {
            int a;
            a = (int)trunc(y / 100);
            b = 2 - a + (int)trunc(a / 4);
        } else {
            b = 0;
        }

        int c;
        if(y < 0) {
            c = (int)trunc((365.25 * y) - 0.75);
        } else {
            c = (int)trunc(365.25 * y);
        }

        int d;
        d = (int)trunc(30.6001 * (m + 1));

        julian_date_ = b + c + d + day_ + 1720994.5;
        julian_date_is_valid_ = true;
    }
}

void PA::Date::ComputeTT() const
{
    if(julian_date_is_valid_) {
        // Reference: [Peter11] Section 5
        double jd{julian_date_ + 0.5};
        int i{static_cast<int>(trunc(jd))};
        double f{jd - i};

        int b;
        if(i > 2299160) {
            int a;
            a = trunc((i - 1867216.25) / 36524.25);
            b = i + a - trunc(a / 4.0) + 1;
        } else {
            b = i;
        }

        int c{b + 1524};
        int d{static_cast<int>(trunc((c - 122.1) / 365.25))};
        int e{static_cast<int>(trunc(365.25 * d))};
        int g{static_cast<int>(trunc((c - e) / 30.6001))};
        day_ = c - e + f - trunc(30.6001 * g);
        month_ = (g <= 13) ? (g - 1) : (g - 13);
        year_ = (month_ >= 3) ? (d - 4716) : (d - 4715);
        terrestrial_time_is_valid_ = true;
    }
}

double PA::Date::GetDeltaT() const
{
    // Many formulas exist. We implement the one available from NASA.
    // References:
    // - https://eclipse.gsfc.nasa.gov/SEhelp/deltatpoly2004.html
    // - http://astro.altspu.ru/~aw/DeltaT/
    // - https://www.hindawi.com/journals/jas/2014/480964/
    // - https://www.ucolick.org/~sla/leapsecs/dutc.html

    if(!delta_t_is_valid_) {
        ComputeDeltaT();
        if(!delta_t_is_valid_) {
            return 0.0;
        }
    }

    return delta_t_;
}

void PA::Date::ComputeDeltaT() const
{
    if(terrestrial_time_is_valid_) {
        // Many formulas exist. We implement the one available from NASA.
        // Error can be up to 2.0s for recent years. Not ideal but usable.
        // References:
        // - https://eclipse.gsfc.nasa.gov/SEhelp/deltatpoly2004.html
        // - http://astro.altspu.ru/~aw/DeltaT/
        // - https://www.hindawi.com/journals/jas/2014/480964/
        // - [Actual Data] http://maia.usno.navy.mil/ser7/deltat.data
        double y{year_ + (month_ - 0.5) / 12.0};
        // double y{static_cast<double>(year_)};
        if(y >= +2150) {
            double u{(y - 1820) / 100.0};
            delta_t_ = -20.0 + 32 * u * u;
        } else if(y >= +2050) {
            double u{(y - 1820) / 100.0};
            delta_t_ = -20.0 + 32 * u * u - 0.5628 * (2150 - y);
        } else if(y >= +2005) {
            delta_t_ = horner_polynomial({62.92, 0.32217, 0.005589}, y - 2000);
        } else if(y >= +1986) {
            delta_t_ = horner_polynomial({63.86, 0.3345, -0.060374, 0.0017275,
                                          0.000651814, 0.00002373599},
                                         y - 2000.0);
        } else if(y >= +1961) {
            delta_t_ = horner_polynomial(
                {45.45, 1.067, -1.0 / 260.0, -1.0 / 718.0}, y - 1975.0);
        } else if(y >= +1941) {
            delta_t_ = horner_polynomial(
                {29.07, 0.407, -1.0 / 233.0, 1.0 / 2547.0}, y - 1950.0);
        } else if(y >= +1920) {
            delta_t_ = horner_polynomial({21.20, 0.84493, -0.076100, 0.0020936},
                                         y - 1920.0);
        } else if(y >= +1900) {
            delta_t_ = horner_polynomial(
                {-2.79, 1.494119, -0.0598939, 0.0061966, -0.000197},
                y - 1900.0);
        } else if(y >= +1860) {
            delta_t_ = horner_polynomial(
                {7.62, 0.5737, -0.251754, 0.01680668, -0.0004473624},
                y - 1860.0);
        } else if(y >= +1800) {
            delta_t_ = horner_polynomial(
                {13.72, -0.332447, 0.0068612, 0.0041116, -0.00037436,
                 0.0000121272, -0.0000001699, 0.000000000875},
                y - 1800.0);
        } else if(y >= +1700) {
            delta_t_ = horner_polynomial(
                {8.83, 0.1603, -0.0059285, 0.00013336, -1.0 / 1174000.0},
                y - 1700.0);
        } else if(y >= +1600) {
            delta_t_ = horner_polynomial({120.0, -0.9808, -0.01532, 1 / 7129.0},
                                         y - 1600.0);
        } else if(y >= +500) {
            delta_t_ =
                horner_polynomial({1574.2, -556.01, 71.23472, 0.319781,
                                   -0.8503463, -0.005050998, 0.0083572073},
                                  (y - 1000.0) / 100.0);
        } else if(y >= -500) {
            delta_t_ =
                horner_polynomial({10583.6, -1014.41, 33.78311, -5.952053,
                                   -0.1798452, 0.022174192, 0.0090316521},
                                  y / 100.0);
        } else {
            double u{(y - 1820) / 100.0};
            delta_t_ = -20.0 + 32 * u * u;
        }
        delta_t_is_valid_ = true;
    }
}
