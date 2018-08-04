#ifndef EARTH_H_
#define EARTH_H_

#include "angle.h"
#include "date.h"

namespace PA
{
class Earth
{
   public:
    constexpr Earth(double jd) : date_(Date(jd))
    {
    }
    constexpr Earth(const Date& date) : date_(date)
    {
    }

    constexpr Degree GetObliquity() noexcept;
    constexpr Degree GetObliquityMean() noexcept;
    constexpr Degree GetNutationLongitude() noexcept;
    constexpr Degree GetNutationObliquity() noexcept;

   private:
    Date date_;

    constexpr void ComputeObliquityMean() noexcept;
    bool obliquity_mean_is_valid_{false};
    Degree obliquity_mean_{0.0};

    constexpr void ComputeNutation() noexcept;
    bool nutation_is_valid_{false};
    Degree nutation_longitude_{0.0};
    Degree nutation_obliquity_{0.0};

    bool obliquity_is_valid_{false};
    Degree obliquity_{0.0};
};

constexpr Degree Earth::GetObliquity() noexcept
{
    if(!obliquity_mean_is_valid_) {
        ComputeObliquityMean();
    }
    if(!nutation_is_valid_) {
        ComputeNutation();
    }
    return obliquity_mean_ + nutation_obliquity_;
}

constexpr Degree Earth::GetObliquityMean() noexcept
{
    if(!obliquity_mean_is_valid_) {
        ComputeObliquityMean();
    }
    return obliquity_mean_;
}

constexpr Degree Earth::GetNutationLongitude() noexcept
{
    if(!nutation_is_valid_) {
        ComputeNutation();
    }
    return nutation_longitude_;
}

constexpr Degree Earth::GetNutationObliquity() noexcept
{
    if(!nutation_is_valid_) {
        ComputeNutation();
    }
    return nutation_obliquity_;
}

constexpr void Earth::ComputeObliquityMean() noexcept
{
    // References:
    // - http://www.neoprogrammics.com/obliquity_of_the_ecliptic/
    // - [Jean99] Chapter 22, p.147
    double u{(date_.GetJulianDate() - PA::EpochJ2000) / 3652500.0};
    obliquity_mean_ =
        Degree(horner_polynomial({+84381.448, -4680.93, -1.55, +1999.25, -51.38,
                                  -249.67, -39.05, +7.12, +27.87, +5.79, +2.45},
                                 u) /
               3600.0);
    obliquity_mean_is_valid_ = true;
}

constexpr void Earth::ComputeNutation() noexcept
{
    // Implements IAU 2000B Nutation Model
    // Accuracy: 0.001" during 1995-2050
    // References:
    // - http://www.neoprogrammics.com/nutations/
    // - http://www.iausofa.org/2018_0130_F/sofa/nut00b.for
    // -
    // https://www.iers.org/SharedDocs/Publikationen/EN/IERS/Publications/tn/TechnNote32/tn32_033.pdf
    // - [Jean99] Chapter, p.143
    // double t{(date_.GetJulianDate() - PA::Date::J2000_JAN_1_5) /
    // 36525.0};
    double t{(date_.GetJulianDate() - PA::EpochJ2000) / 36525.0};
    Degree l{horner_polynomial({+485868.249036, +1717915923.2178, +31.8792,
                                +0.051635, -0.00024470},
                               t) /
             3600.0};
    Degree lp{horner_polynomial({+1287104.79305, +129596581.0481, -0.5532,
                                 +0.000136, -0.00001149},
                                t) /
              3600.0};
    Degree f{horner_polynomial({+335779.526232, +1739527262.8478, -12.7512,
                                -0.001037, +0.00000417},
                               t) /
             3600.0};
    Degree d{horner_polynomial({+1072260.70369, +1602961601.2090, -6.3706,
                                +0.006593, -0.00003169},
                               t) /
             3600.0};
    Degree om{horner_polynomial({450160.398036, -6962890.5431, +7.4722,
                                 +0.007702, -0.00005939},
                                t) /
              3600.0};
#if 0
        std::cout << "=>  L: " << l.DegStr() << " = " << l.GetUnwind().DegStr()
                  << std::endl;
        std::cout << "=> L': " << lp.DegStr() << " = "
                  << lp.GetUnwind().DegStr() << std::endl;
        std::cout << "=>  F: " << f.DegStr() << " = " << f.GetUnwind().DegStr()
                  << std::endl;
        std::cout << "=>  D: " << d.DegStr() << " = " << d.GetUnwind().DegStr()
                  << std::endl;
        std::cout << "=> Om: " << om.DegStr() << " = "
                  << om.GetUnwind().DegStr() << std::endl;
#endif
    struct {
        int m1;
        int m2;
        int m3;
        int m4;
        int m5;
        int aa;
        int bb;
        int cc;
        int dd;
        int ee;
        int ff;
    } periodic_terms[]{
        {0, 0, 0, 0, 1, -172064161, -174666, 33386, 92052331, 9086, 15377},
        {0, 0, 2, -2, 2, -13170906, -1675, 13696, 5730336, -3015, -4587},
        {0, 0, 2, 0, 2, -2276413, -234, 2796, 978459, -485, 1374},
        {0, 0, 0, 0, 2, 2074554, 207, -698, -897492, 470, -291},
        {0, 1, 0, 0, 0, 1475877, -3633, 11817, 73871, -184, -1924},
        {0, 1, 2, -2, 2, -516821, 1226, -524, 224386, -677, -174},
        {1, 0, 0, 0, 0, 711159, 73, -872, -6750, 0, 358},
        {0, 0, 2, 0, 1, -387298, -367, 380, 200728, 18, 318},
        {1, 0, 2, 0, 2, -301461, -36, 816, 129025, -63, 367},
        {0, -1, 2, -2, 2, 215829, -494, 111, -95929, 299, 132},
        {0, 0, 2, -2, 1, 128227, 137, 181, -68982, -9, 39},
        {-1, 0, 2, 0, 2, 123457, 11, 19, -53311, 32, -4},
        {-1, 0, 0, 2, 0, 156994, 10, -168, -1235, 0, 82},
        {1, 0, 0, 0, 1, 63110, 63, 27, -33228, 0, -9},
        {-1, 0, 0, 0, 1, -57976, -63, -189, 31429, 0, -75},
        {-1, 0, 2, 2, 2, -59641, -11, 149, 25543, -11, 66},
        {1, 0, 2, 0, 1, -51613, -42, 129, 26366, 0, 78},
        {-2, 0, 2, 0, 1, 45893, 50, 31, -24236, -10, 20},
        {0, 0, 0, 2, 0, 63384, 11, -150, -1220, 0, 29},
        {0, 0, 2, 2, 2, -38571, -1, 158, 16452, -11, 68},
        {0, -2, 2, -2, 2, 32481, 0, 0, -13870, 0, 0},
        {-2, 0, 0, 2, 0, -47722, 0, -18, 477, 0, -25},
        {2, 0, 2, 0, 2, -31046, -1, 131, 13238, -11, 59},
        {1, 0, 2, -2, 2, 28593, 0, -1, -12338, 10, -3},
        {-1, 0, 2, 0, 1, 20441, 21, 10, -10758, 0, -3},
        {2, 0, 0, 0, 0, 29243, 0, -74, -609, 0, 13},
        {0, 0, 2, 0, 0, 25887, 0, -66, -550, 0, 11},
        {0, 1, 0, 0, 1, -14053, -25, 79, 8551, -2, -45},
        {-1, 0, 0, 2, 1, 15164, 10, 11, -8001, 0, -1},
        {0, 2, 2, -2, 2, -15794, 72, -16, 6850, -42, -5},
        {0, 0, -2, 2, 0, 21783, 0, 13, -167, 0, 13},
        {1, 0, 0, -2, 1, -12873, -10, -37, 6953, 0, -14},
        {0, -1, 0, 0, 1, -12654, 11, 63, 6415, 0, 26},
        {-1, 0, 2, 2, 1, -10204, 0, 25, 5222, 0, 15},
        {0, 2, 0, 0, 0, 16707, -85, -10, 168, -1, 10},
        {1, 0, 2, 2, 2, -7691, 0, 44, 3268, 0, 19},
        {-2, 0, 2, 0, 0, -11024, 0, -14, 104, 0, 2},
        {0, 1, 2, 0, 2, 7566, -21, -11, -3250, 0, -5},
        {0, 0, 2, 2, 1, -6637, -11, 25, 3353, 0, 14},
        {0, -1, 2, 0, 2, -7141, 21, 8, 3070, 0, 4},
        {0, 0, 0, 2, 1, -6302, -11, 2, 3272, 0, 4},
        {1, 0, 2, -2, 1, 5800, 10, 2, -3045, 0, -1},
        {2, 0, 2, -2, 2, 6443, 0, -7, -2768, 0, -4},
        {-2, 0, 0, 2, 1, -5774, -11, -15, 3041, 0, -5},
        {2, 0, 2, 0, 1, -5350, 0, 21, 2695, 0, 12},
        {0, -1, 2, -2, 1, -4752, -11, -3, 2719, 0, -3},
        {0, 0, 0, -2, 1, -4940, -11, -21, 2720, 0, -9},
        {-1, -1, 0, 2, 0, 7350, 0, -8, -51, 0, 4},
        {2, 0, 0, -2, 1, 4065, 0, 6, -2206, 0, 1},
        {1, 0, 0, 2, 0, 6579, 0, -24, -199, 0, 2},
        {0, 1, 2, -2, 1, 3579, 0, 5, -1900, 0, 1},
        {1, -1, 0, 0, 0, 4725, 0, -6, -41, 0, 3},
        {-2, 0, 2, 0, 2, -3075, 0, -2, 1313, 0, -1},
        {3, 0, 2, 0, 2, -2904, 0, 15, 1233, 0, 7},
        {0, -1, 0, 2, 0, 4348, 0, -10, -81, 0, 2},
        {1, -1, 2, 0, 2, -2878, 0, 8, 1232, 0, 4},
        {0, 0, 0, 1, 0, -4230, 0, 5, -20, 0, -2},
        {-1, -1, 2, 2, 2, -2819, 0, 7, 1207, 0, 3},
        {-1, 0, 2, 0, 0, -4056, 0, 5, 40, 0, -2},
        {0, -1, 2, 2, 2, -2647, 0, 11, 1129, 0, 5},
        {-2, 0, 0, 0, 1, -2294, 0, -10, 1266, 0, -4},
        {1, 1, 2, 0, 2, 2481, 0, -7, -1062, 0, -3},
        {2, 0, 0, 0, 1, 2179, 0, -2, -1129, 0, -2},
        {-1, 1, 0, 1, 0, 3276, 0, 1, -9, 0, 0},
        {1, 1, 0, 0, 0, -3389, 0, 5, 35, 0, -2},
        {1, 0, 2, 0, 0, 3339, 0, -13, -107, 0, 1},
        {-1, 0, 2, -2, 1, -1987, 0, -6, 1073, 0, -2},
        {1, 0, 0, 0, 2, -1981, 0, 0, 854, 0, 0},
        {-1, 0, 0, 1, 0, 4026, 0, -353, -553, 0, -139},
        {0, 0, 2, 1, 2, 1660, 0, -5, -710, 0, -2},
        {-1, 0, 2, 4, 2, -1521, 0, 9, 647, 0, 4},
        {-1, 1, 0, 1, 1, 1314, 0, 0, -700, 0, 0},
        {0, -2, 2, -2, 1, -1283, 0, 0, 672, 0, 0},
        {1, 0, 2, 2, 1, -1331, 0, 8, 663, 0, 4},
        {-2, 0, 2, 2, 2, 1383, 0, -2, -594, 0, -2},
        {-1, 0, 0, 0, 2, 1405, 0, 4, -610, 0, 2},
        {1, 1, 2, -2, 2, 1290, 0, 0, -556, 0, 0},
    };
    double sum_dpsi{0.0};
    double sum_deps{0.0};
    for(auto& pt : periodic_terms) {
        Degree arg{l * pt.m1 + lp * pt.m2 + f * pt.m3 + d * pt.m4 + om * pt.m5};
        double dpsi{(pt.aa + pt.bb * t) * sin(arg) + pt.cc * cos(Degree(arg))};
        double deps{(pt.dd + pt.ee * t) * cos(arg) + pt.ff * sin(Degree(arg))};
        sum_dpsi += dpsi;
        sum_deps += deps;
    }
    nutation_longitude_ = Degree(sum_dpsi / 36000000000.0);
    nutation_obliquity_ = Degree(sum_deps / 36000000000.0);
    nutation_is_valid_ = true;
}

}  // namespace PA

#endif  // EARTH_H_
