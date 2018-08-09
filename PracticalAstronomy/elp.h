#ifndef ELP_H_
#define ELP_H_

namespace PA
{
class ELP
{
   public:
    constexpr ELP(double jd) noexcept : julian_date_(jd){};

   private:
    double julian_date_;
};

// References:
// - [Jean99] Chapter 47, pp.337-344
// - https://sourceforge.net/p/gplan/wiki/ELP%20MPP02/
// - https://www.aanda.org/articles/aa/full/2003/23/aa3101/aa3101.html
// - http://totaleclipse.eu/Astronomy/ELP2000.html
// - https://idlastro.gsfc.nasa.gov/ftp/pro/astro/moonpos.pro

}  // namespace PA

#endif  // ELP_H_
