#ifndef MISC_H_
#define MISC_H_

#include <cmath>

template <class T, int SZ>
constexpr auto horner_polynomial(const T (&coeffs)[SZ], T x) noexcept
{
    T value{0};
    for(int i = SZ - 1; i >= 0; i--)
        value = value * x + coeffs[i];
    return value;
}

void VSOP87ProcessDataFiles();

struct PeriodicTerm {
    double a;
    double b;
    double c;
};

struct PeriodicTermTableDegree {
    const struct PeriodicTerm *terms;
    int size;
};

struct PeriodicTermTable {
    const struct PeriodicTermTableDegree *degrees;
    int size;
};

constexpr double PeriodicTermCosCompute(const PeriodicTermTable &table,
                                        double t) noexcept
{
    double value{0.0};
    int degree{table.size - 1};
    while(true) {
        for(int i = 0; i < table.degrees[degree].size; i++) {
            const PeriodicTerm &pt{table.degrees[degree].terms[i]};
            value += pt.a * std::cos(pt.b + pt.c * t);
        }
        if(degree == 0) {
            break;
        }
        value *= t;
        degree--;
    }
    return value;
}

#endif  // MISC_H_