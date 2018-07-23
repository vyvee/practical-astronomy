#ifndef MISC_H_
#define MISC_H_

template <class T, int SZ>
constexpr auto horner_polynomial(const T (&coeffs)[SZ], T x)
{
    T value{0};
    for(int i = SZ - 1; i >= 0; i--)
        value = value * x + coeffs[i];
    return value;
}

#endif  // MISC_H_