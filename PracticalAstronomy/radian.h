#ifndef ANGLE_H_
#define ANGLE_H_

#include <cassert>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

namespace PA {

constexpr double operator"" _deg(long double deg) { return deg * M_PI / 180.0; }

constexpr double operator"" _deg(unsigned long long int deg) {
  return deg * M_PI / 180.0;
}

constexpr double operator"" _arcmin(long double arcmin) {
  return arcmin * M_PI / (180.0 * 60.0);
}

constexpr double operator"" _arcmin(unsigned long long int arcmin) {
  return arcmin * M_PI / (180.0 * 60.0);
}

constexpr double operator"" _arcsec(long double arcsec) {
  return arcsec * M_PI / (180.0 * 3600.0);
}

constexpr double operator"" _arcsec(unsigned long long int arcsec) {
  return arcsec * M_PI / (180.0 * 3600.0);
}

constexpr double operator"" _h(long double h) { return h * M_PI / 12.0; }

constexpr double operator"" _h(unsigned long long int h) {
  return h * M_PI / 12.0;
}

constexpr double operator"" _m(long double m) {
  return m * M_PI / (12.0 * 60.0);
}

constexpr double operator"" _m(unsigned long long int m) {
  return m * M_PI / (12.0 * 60.0);
}

constexpr double operator"" _s(long double s) {
  return s * M_PI / (12.0 * 3600.0);
}

constexpr double operator"" _s(unsigned long long int s) {
  return s * M_PI / (12.0 * 3600.0);
}

constexpr double RadUnwind(double rad) noexcept {
  return rad - 2 * M_PI * floor(rad / (2 * M_PI));
}

constexpr double RadNormalize(double rad) noexcept {
  return rad - 2 * M_PI * floor((rad + M_PI) / (2 * M_PI));
}

constexpr double RadToDeg(double rad) noexcept { return rad * 180.0 / M_PI; }

constexpr double DegToRad(double deg) noexcept { return deg * M_PI / 180.0; }

constexpr double RadToArcMin(double rad) noexcept {
  return rad * (180.0 * 60.0) / M_PI;
}

constexpr double RadToArcSec(double rad) noexcept {
  return rad * (180.0 * 3600.0) / M_PI;
}

constexpr double RadToHour(double rad) noexcept {
  return rad * (180.0 / 15.0) / M_PI;
}

constexpr double RadToMinute(double rad) noexcept {
  return rad * (180.0 * 60.0 / 15.0) / M_PI;
}

constexpr double RadToSecond(double rad) noexcept {
  return rad * (180.0 * 3600.0 / 15.0) / M_PI;
}

static constexpr int quick_pow10(int n) {
  constexpr int pow10[10] = {
      1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000,
  };
  return pow10[n];
}

/* Decimal String */

inline std::string ValueToStr(double value, const char *delimiter,
                              int prec) noexcept {
  assert(prec >= 0 && prec <= 9);
  bool neg = (value < 0.0);
  double v_int, v_frac;
  v_frac = modf(fabs(value), &v_int);

  if (prec == 0) {
    if (v_frac >= 0.5) v_int += 1.0;
  } else {
    v_frac = round(v_frac * quick_pow10(prec));
    if (v_frac >= quick_pow10(prec)) {
      assert(v_frac == quick_pow10(prec));
      v_frac = 0.0;
      v_int += 1.0;
    }
  }

  std::stringstream ss;
  if (neg) ss << '-';
  ss << static_cast<int>(v_int) << delimiter;
  if (prec > 0) {
    ss << '.';
    ss << std::setfill('0');
    ss << std::setw(prec) << static_cast<int>(v_frac);
  }

  return ss.str();
}

inline std::string RadToDegStr(double rad, int prec = 6) noexcept {
  return ValueToStr(RadToDeg(rad), "°", prec);
}

inline std::string RadToArcSecStr(double rad, int prec = 0) noexcept {
  return ValueToStr(RadToArcSec(rad), "″", prec);
}

inline std::string RadToHourStr(double rad, int prec = 6) noexcept {
  return ValueToStr(RadToHour(rad), "h", prec);
}

/* Sexagecimal String */

inline std::string ValueToSexagecimalStr(double value, const char **delimiters,
                                         int s_prec) noexcept {
  assert(s_prec >= 0 && s_prec <= 9);
  bool neg = (value < 0.0);
  double v_int, v_frac;
  v_frac = modf(3600.0 * fabs(value), &v_int);

  if (s_prec == 0) {
    if (v_frac >= 0.5) v_int += 1.0;
  } else {
    v_frac = round(v_frac * quick_pow10(s_prec));
    if (v_frac >= quick_pow10(s_prec)) {
      assert(v_frac == quick_pow10(s_prec));
      v_frac = 0.0;
      v_int += 1.0;
    }
  }

  int d, m, s;
  s = static_cast<int>(v_int);
  m = s / 60;
  s -= 60 * m;
  d = m / 60;
  m -= 60 * d;

  std::stringstream ss;
  if (neg) ss << '-';
  ss << static_cast<int>(d) << delimiters[0];
  ss << std::setfill('0');
  ss << std::setw(2) << static_cast<int>(m) << delimiters[1];
  ss << std::setfill('0');
  ss << std::setw(2) << static_cast<int>(s) << delimiters[2];
  if (s_prec > 0) {
    ss << '.';
    ss << std::setfill('0');
    ss << std::setw(s_prec) << static_cast<int>(v_frac);
  }

  return ss.str();
}  // namespace PA

inline std::string DegToDMSStr(double deg, int s_prec = 0) noexcept {
  static const char *delimiters[] = {"°", "′", "″"};
  return ValueToSexagecimalStr(deg, delimiters, s_prec);
}

inline std::string HourToHMSStr(double hour, int s_prec = 0) noexcept {
  static const char *delimiters[] = {"h", "m", "s"};
  return ValueToSexagecimalStr(hour, delimiters, s_prec);
}

inline std::string RadToDMSStr(double rad, int s_prec = 0) noexcept {
  return DegToDMSStr(RadToDeg(rad), s_prec);
}

inline std::string RadToHMSStr(double rad, int s_prec = 0) noexcept {
  return HourToHMSStr(RadToHour(rad), s_prec);
}

}  // namespace PA

#endif  // ANGLE_H_
