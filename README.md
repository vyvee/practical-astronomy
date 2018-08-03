# Practical Astronomy

Implementations of astronomical algorithms. Assumed C++17 and tested with GCC.

Aims to implement algorithms to calculate positions of solar system objects with moderate accuracy, or to about a few arcseconds.
Currently, the main references include \[Jean99\], \[Peter11\].

Does not aim to provide results with extremely high accuracy. For highly accurate results, you are recommended to refer to \[VOSP87\], \[XEphem\], etc.

## Completed

- Angle: Degree, Radian, DMS, HMS
- Date: Julian Date, Calendar (TT), Delta-T
- Earth: Obliquity, Nutation
- Sun: Position
- Solver: Kepler's equation

## TODOs

The project is still in EARLY stage.

- Change to use more algorithms in \[Jean99\] which provide more accurate results
- Moon: Position
- Time: UT
- Planets: Position
- Coordinate Transformation
- Better error handling: Invalid julian date, invalid parameters, algorithms not applicable and not available for the time interested, etc.
- C++: Better and more use of constexpr, move semantics, noexcept, etc.
- Many others...

## References

- [\[Jean99\] Jean Meeus, *Astronomical Algorithms*, 2nd Edition, 1999.](https://www.willbell.com/MATH/MC1.HTM)
- [\[Peter11\] Peter Duffett-Smith and Jonathan Zwart, *Practical Astronomy with your Calculator or Spreadsheet*, 4th Edition, 2011.](https://en.wikipedia.org/wiki/Practical_Astronomy_with_your_Calculator)
- [Polynomial Expressions for Delta T (ΔT)](https://eclipse.gsfc.nasa.gov/SEhelp/deltatpoly2004.html)
- [\[VOSP87\] Planetary Solutions VSOP87](http://cdsarc.u-strasbg.fr/viz-bin/Cat?cat=VI/81)
- [\[XEphem\] XEphem: The Cross-Platform Astronomy Software](http://www.clearskyinstitute.com/xephem/)
