#ifndef DATE_H_
#define DATE_H_

namespace PA
{
class Date
{
   public:
    static constexpr double J1900_JAN_0_5{2415020.0};
    static constexpr double J2000_JAN_1_5{2451545.0};
    static constexpr double J2010{2455196.5};

    Date();
    Date(int year, int month, double day);
    Date(double julian_date);
    ~Date(){};

    bool SetTT(int year, int month, double day);
    bool SetJulianDate(double julian_date);

    bool GetTT(int *p_year, int *p_month, double *p_day) const;
    bool GetJulianDate(double *p_julian_date) const;
    double GetJulianDate() const;

    operator double() const
    {
        return GetJulianDate();
    }

   private:
    mutable bool calendar_date_is_valid_{false};
    mutable int year_;
    mutable int month_;
    mutable double day_;
    mutable bool julian_date_is_valid_{false};
    mutable double julian_date_;
};

}  // namespace PA

#endif  // DATE_H_
