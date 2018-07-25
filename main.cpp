#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

#include "date.h"
#include "test.h"

void ephemeris_now(void)
{
    std::time_t now;
    now =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm *ptm;
    ptm = std::gmtime(&now);

    PA::Date date;
    date.SetCalendarTT(1900 + ptm->tm_year, ptm->tm_mon + 1, ptm->tm_mday,
                       ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

    std::cout << "JD " << std::fixed << std::setprecision(6)
              << date.GetJulianDate() << std::endl;
    std::cout << date.GetTTString() << std::endl;
    std::cout << "Delta-T: " << std::showpos << std::fixed
              << std::setprecision(2) << date.GetDeltaT() << "s" << std::endl;
}

int main(void)
{
    if(test_internal()) {
        std::cout << "Internal Test OK!" << std::endl;
    } else {
        std::cout << "Error: Internal Test Failed!" << std::endl;
        return 0;
    }

    ephemeris_now();

    return 0;
}
