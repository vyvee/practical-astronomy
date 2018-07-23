#include "date.h"

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

#if 0
double PA::Date::GetDeltaT() const
{
    return 0.0;
}
#endif
