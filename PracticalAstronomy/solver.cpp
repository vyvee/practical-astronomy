#include "solver.h"

#include <cmath>
#include <limits>

using namespace PA;

// #define DEBUG

#ifdef DEBUG
#include <iomanip>
#include <iostream>
#endif

Radian PA::Solver::solve_kepler(double ecc, const Radian& m)
{
    double e;
    if(ecc < 0.8) {
        e = m.Rad();
    } else {
        e = 2 * M_PI * floor(m.Rad() / (2 * M_PI)) + M_PI;
    };
    double delta, delta_prev{std::numeric_limits<double>::infinity()};
#ifdef DEBUG
    std::cout << "  " << m_radian << " = E - " << ecc << " * sin(E)"
              << std::endl;
    std::cout << "                          E: " << std::setw(10) << e
              << std::endl;
#endif
    int iteration{0};
    while(++iteration <= 35) {
        delta = e - m.Rad() - ecc * std::sin(e);
        delta /= ecc * std::cos(e) - 1.0;
        e += delta;
#ifdef DEBUG
        std::cout << '[' << std::setw(2) << iteration
                  << "] delta: " << std::setw(12) << delta
                  << "  E: " << std::setw(10) << e << std::endl;
#endif
        if((std::fabs(delta) < 1.0e-10) &&
           ((delta == 0.0 || std::fabs(delta) >= std::fabs(delta_prev)))) {
            break;
        }
        //        if(e_prev == e) {
        //            break;
        //        }
        delta_prev = delta;
    }
    return Radian(e);
}
