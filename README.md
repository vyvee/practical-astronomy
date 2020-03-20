# Practical Astronomy

Implementation of astronomical algorithms.

Aims to implementation methods that achieve accuracy for `practical' use. Main source material is the amazing book [Astronomical Algorithms][Jean99] by Jean Meeus. However, some useful methods (such as the full version of VSOP87) are also included.

## Features

- Date: Julian Date, Calendar (TT), Delta-T
- Sun: Position
- Earth: Obliquity, Nutation
- Moon: Position (ELP82-Abridged)
- Planets: VSOP87 (Full)
- Solver: Kepler's equation
- Equation of Time

## Compilation

```
$ make
```

The code is written in C++17 and currently depends on G++ extension to support designated initializers from C99.

## TODOs

- Sunrise, Sunset, Twilight time
- Moonrise, Moonset time
- Moon Phase
- Illuminated Fraction of the Moon's Disk
- Moon: Apparent position, Equatorial coordinate
- Correction for Parallax
- Coordinate Transformation
- Time: UT
- Calculation of Eclipses
- Better error handling (E.g., invalid julian date, invalid parameters, algorithms not applicable and not available for the time interested, etc.)
- C++: Better and more use move semantics, noexcept, etc.
- Many others...

## Main References

* [Jean99] - Jean Meeus, *Astronomical Algorithms*, 2nd Edition, 1999.
* [Peter11] - Peter Duffett-Smith and Jonathan Zwart, *Practical Astronomy with your Calculator or Spreadsheet*, 4th Edition, 2011.
* [VSOP87] - Planetory Solutions
* [NASA07] - Polynomial Expressions for Delta T (ΔT)
* [VSOP2013] - Planetory Solutions
* [LEA-406] - Theory of Lunar Motion
* [ELP/MPP02] - Theory of Lunar Motion
* [XEphem] - The Cross-Platform Astronomy Software

[Jean99]: https://www.willbell.com/MATH/MC1.HTM
[VSOP87]: http://cdsarc.u-strasbg.fr/viz-bin/Cat?cat=VI/81
[Peter11]: https://en.wikipedia.org/wiki/Practical_Astronomy_with_your_Calculator
[VSOP2013]: https://www.aanda.org/articles/aa/abs/2013/09/aa21843-13/aa21843-13.html
[LEA-406]: https://www.aanda.org/articles/aa/full/2007/33/aa7568-07/aa7568-07.html
[NASA07]: https://eclipse.gsfc.nasa.gov/SEhelp/deltatpoly2004.html
[XEphem]: http://www.clearskyinstitute.com/xephem/
[ELP/MPP02]: https://sourceforge.net/p/gplan/wiki/ELP%20MPP02/
