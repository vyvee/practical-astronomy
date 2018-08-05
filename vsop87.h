#ifndef VSOP87_H_
#define VSOP87_H_

#include "angle.h"

namespace PA
{
class VSOP87
{
   public:
    enum class Planet {
        kEarth,
    };

    constexpr VSOP87() noexcept
    {
    }

    constexpr void ComputePlanetPosition(Planet planet);

   private:
};

constexpr void VSOP87::ComputePlanetPosition(Planet /*planet*/)
{
    // [Jean99] includes VSOP87B
    // [Jean99] Chapter 25 (Solar Coordinates), p.163 (Conversion to FK5 system)
    // [Jean99] Chapter 26 (Rectangular Coordinates of the Sun), p.171
    // [Jean99] Chapter 32 (Positions of Planets), p.217 (Conversion to FK5
    // system)
    // [Jean99] Chapter 33 (Elliptic Motion), p.223 (Geocentric positions of
    // planets)
    // [Jean99] Appendix III (Planets: Periodic Terms), p.413
    // [Jean99] Chapter 21 (Precession), p.131 (FK5 system)
    //
    // References:
    // - ftp://ftp.imcce.fr/pub/ephem/planets/vsop87
    // - http://neoprogrammics.com/vsop87/
    // - https://github.com/infinet/lunar-calendar/blob/master/aa.py
    // -: Heliocentric elliptical elements J2000
    // A: Heliocentric rectangular variables J2000
    // B: (LBR) Heliocentric spherical variables J2000
    // C: Heliocentric rectangular variables of date
    // D: (LBR) Heliocentric spherical variables of date
    // E: Barycentric rectangular variables J2000
    // Lunar Theory:
    // https://www.aanda.org/articles/aa/full/2007/33/aa7568-07/aa7568-07.html
    // - https://www.cnblogs.com/noryes/p/5716673.html
    /*
    struct PeriodicTerm {
        double a;
        double b;
        double c;
    };
    PeriodicTerm Earth_L0[]{
        {175347046, 0, 0},
    };
     */
}

}  // namespace PA

#endif  // VSOP87_H_
