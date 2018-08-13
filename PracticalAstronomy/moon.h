#ifndef MOON_H_
#define MOON_H_

namespace PA
{
class Moon
{
   public:
    constexpr Moon(double julian_date) noexcept;
    constexpr double GetGeocentricLongitude() noexcept;
    constexpr double GetGeocentricLatitude() noexcept;

   private:
    double julian_date_;
};

constexpr Moon::Moon(double julian_date) noexcept : julian_date_(julian_date)
{
}

constexpr double Moon::GetGeocentricLongitude() noexcept
{
    return 0.0;
}

constexpr double Moon::GetGeocentricLatitude() noexcept
{
    return 0.0;
}

}  // namespace PA

#endif  // MOON_H_
