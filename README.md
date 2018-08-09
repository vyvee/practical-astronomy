# Practical Astronomy

Implementations of astronomical algorithms with C++17.

Aims to implement algorithms to calculate positions of solar system objects with high accuracy.

The project is still in EARLY stage.

## Features

- Angle: Degree, Radian, DMS, HMS
- Date: Julian Date, Calendar (TT), Delta-T
- Sun: Position
- Earth: Obliquity, Nutation
- Planets: VSOP87 (Full)
- Solver: Kepler's equation
- Develops with GCC

## TODOs

- Apparent position of the Sun
- Coordinate Transformation
- Positions of Moon and other planets
- Make use of [VSOP2013],  [ELP/MPP02], [LEA-406], etc.
- Time: UT
- Calculation of Eclipses
- Better error handling: Invalid julian date, invalid parameters, algorithms not applicable and not available for the time interested, etc.
- C++: Better and more use move semantics, noexcept, etc.
- Many others...

## Main References

* [Jean99] - Jean Meeus, *Astronomical Algorithms*, 2nd Edition, 1999.
* [VSOP87] - Planetory Solutions
* [VSOP2013] - Planetory Solutions
* [LEA-406]- Theory of Lunar Motion
* [ELP/MPP02]- Theory of Lunar Motion
* [NASA07] - Polynomial Expressions for Delta T (ΔT)
* [XEphem] - The Cross-Platform Astronomy Software
* [Peter11] - Peter Duffett-Smith and Jonathan Zwart, *Practical Astronomy with your Calculator or Spreadsheet*, 4th Edition, 2011.

[Jean99]: https://www.willbell.com/MATH/MC1.HTM
[VSOP87]: http://cdsarc.u-strasbg.fr/viz-bin/Cat?cat=VI/81
[VSOP2013]: https://www.aanda.org/articles/aa/abs/2013/09/aa21843-13/aa21843-13.html
[LEA-406]: https://www.aanda.org/articles/aa/full/2007/33/aa7568-07/aa7568-07.html
[NASA07]: https://eclipse.gsfc.nasa.gov/SEhelp/deltatpoly2004.html
[XEphem]: http://www.clearskyinstitute.com/xephem/
[Peter11]: https://en.wikipedia.org/wiki/Practical_Astronomy_with_your_Calculator
[ELP/MPP02]: https://sourceforge.net/p/gplan/wiki/ELP%20MPP02/
