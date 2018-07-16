#include "date.h"
#include "test.h"

#include <cmath>

static bool double_is_close(double x1,
                            double x2,
                            double rel_tol,
                            double abs_tol)
{
    return fabs(x1 - x2) <= fmax(rel_tol * fmax(fabs(x1), fabs(x2)), abs_tol);
}

bool test_internal()
{
    PA::Date date;
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
