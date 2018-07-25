#ifndef DATE_H_
#define DATE_H_

#include <iostream>

namespace PA
{
class Date
{
   public:
    static constexpr double J1900_JAN_0_5{2415020.0};
    static constexpr double J2000_JAN_1_5{2451545.0};
    static constexpr double J2010{2455196.5};

    Date();
    Date(int year, int month = 1, double day = 1.0);
    Date(int year,
         int month,
         int day,
         int hour,
         int minute = 0,
         double second = 0.0);
    Date(double julian_date);
    ~Date(){};

    void SetCalendarTT(int year, int month = 1, double day = 1.0);
    void SetCalendarTT(int year,
                       int month,
                       int day,
                       int hour,
                       int minute = 0,
                       double second = 0.0);
    void SetJulianDate(double julian_date);

    bool GetCalendarTT(int *p_year, int *p_month, double *p_day) const;
    std::string GetTTString() const;
    bool GetJulianDate(double *p_julian_date) const;
    double GetJulianDate() const;
    double GetDeltaT() const;

    operator double() const
    {
        return GetJulianDate();
    }

   private:
    static bool JulianDateFromCalendar(double *p_jd,
                                       int year,
                                       int month,
                                       double day);
    static bool CalendarFromJulianDate(int *p_year,
                                       int *p_month,
                                       double *p_day,
                                       double jd);
    static bool DeltaTFromCalendar(double *p_delta_t,
                                   int year,
                                   int month,
                                   double day);

    void ComputeCalendarTT() const;
    mutable bool calendar_tt_is_valid_{false};
    mutable int calendar_tt_year_;
    mutable int calendar_tt_month_;
    mutable double calendar_tt_day_;

    void ComputeJulianDate() const;
    mutable bool julian_date_is_valid_{false};
    mutable double julian_date_;

    void ComputeDeltaT() const;
    mutable bool delta_t_is_valid_{false};
    mutable double delta_t_;
};

}  // namespace PA

#endif  // DATE_H_
