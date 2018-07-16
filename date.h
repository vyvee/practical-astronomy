#ifndef DATE_H_
#define DATE_H_

namespace PA
{
class Date
{
   public:
    Date();
    ~Date();

    bool SetCalendarDate(int year, int month, double day);
    bool SetJulianDate(double julian_date);
    bool GetCalendarDate(int *p_year, int *p_month, double *p_day);
    bool GetJulianDate(double *p_julian_date);

   private:
    bool calendar_date_is_valid_{false};
    int year_;
    int month_;
    double day_;
    bool julian_date_is_valid_{false};
    double julian_date_;
};

}  // namespace PA

#endif  // DATE_H_
