# Practical Astronomy

Implementations of astronomical algorithms. Main source is \[Peter11\].

Aims to implement algorithms to calculate *approximate* (and NOT high precision) positions of solar system objects.
In particular, aims to use formulae or algorithms that do not involve large number of arithmetic operations.
Currently, the main reference is \[Peter11\].

For very high precision calculations, you are recommended to check \[Jean99\], \[VOSP87\], \[XEphem\], etc.

## Completed

- Angle: Degree, Radian, DMS, HMS
- Date: Julian Date, Calendar (TT), Delta-T
- Earth: Obliquity, Nutation
- Sun: Position
- Solver for Kepler's equation

## TODOs

The project is still in EARLY stage.

- Moon: Position
- Time: UT
- Planets: Position
- Coordinate Transformation
- Better error handling: Invalid julian date, invalid parameters, algorithms not applicable and not available for the time interested, etc.
- C++: Better and more use of constexpr, move semantics, noexcept, etc.
- Many others...

## References

- [\[Peter11\] Peter Duffett-Smith and Jonathan Zwart, *Practical Astronomy with your Calculator or Spreadsheet*, 4th Edition, 2011.](https://en.wikipedia.org/wiki/Practical_Astronomy_with_your_Calculator)
- [\[Jean99\] Jean Meeus, *Astronomical Algorithms*, 2nd Edition, 1999.](https://www.willbell.com/MATH/MC1.HTM)
- [Polynomial Expressions for Delta T (ΔT)](https://eclipse.gsfc.nasa.gov/SEhelp/deltatpoly2004.html)
- [\[VOSP87\] Planetary Solutions VSOP87](http://cdsarc.u-strasbg.fr/viz-bin/Cat?cat=VI/81)
- [\[XEphem\] XEphem: The Cross-Platform Astronomy Software](http://www.clearskyinstitute.com/xephem/)
