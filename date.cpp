#include "date.h"

#include <iomanip>
#include <sstream>
#include <string>

std::string PA::Date::GetTTString() const
{
    if(!calendar_tt_is_valid_) {
        ComputeCalendarTT();
        if(!calendar_tt_is_valid_) {
            return std::string();
        }
    }

    double d, h, m, s;
    s = 60.0 * modf(60.0 * modf(24.0 * modf(calendar_tt_day_, &d), &h), &m);

    std::ostringstream ss;
    ss << std::setfill('0');
    ss << std::setw(4) << calendar_tt_year_ << '-' << std::setw(2)
       << calendar_tt_month_ << '-' << std::setw(2) << d;
    ss << ' ';
    ss << std::setw(2) << static_cast<int>(h) << ':' << std::setw(2)
       << static_cast<int>(m) << ':' << std::fixed << std::setw(4)
       << std::setprecision(1) << s;
    ss << " TT";

    return ss.str();
}
