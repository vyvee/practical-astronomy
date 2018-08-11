#ifndef EARTH_NUTATION_H_
#define EARTH_NUTATION_H_

// - IAU 2000B Nutation Model
//   - Accuracy: <= 0.001" difference with respect to IAU2000A model
//     during 1995-2050
//
// Comparison:
// - IAU 2000A Nutation Model
//   - Accuracy: 0.0002"

#include "date.h"
#include "radian.h"
#include "utils.h"

namespace PA
{
class EarthNutation
{
   public:
    constexpr EarthNutation(double jd) noexcept : julian_date_(jd)
    {
    }

    constexpr double GetNutationLongitude() noexcept;
    constexpr double GetNutationObliquity() noexcept;

   private:
    double julian_date_;

    constexpr void ComputeNutation() noexcept;
    bool nutation_is_valid_{false};
    double nutation_longitude_{0.0};
    double nutation_obliquity_{0.0};
};

constexpr double EarthNutation::GetNutationLongitude() noexcept
{
    if(!nutation_is_valid_) {
        ComputeNutation();
    }
    return nutation_longitude_;
}

constexpr double EarthNutation::GetNutationObliquity() noexcept
{
    if(!nutation_is_valid_) {
        ComputeNutation();
    }
    return nutation_obliquity_;
}

constexpr void EarthNutation::ComputeNutation() noexcept
{
    // IAU 2000B Nutation Model
    // Accuracy: 0.001" during 1995-2050
    // References:
    // - http://www.neoprogrammics.com/nutations/
    // - http://www.iausofa.org/2018_0130_F/sofa/nut00b.for
    // -
    // https://www.iers.org/SharedDocs/Publikationen/EN/IERS/Publications/tn/TechnNote32/tn32_033.pdf
    // - [Jean99] Chapter, p.143
    struct {
        int m1;
        int m2;
        int m3;
        int m4;
        int m5;
        double aa;
        double bb;
        double cc;
        double dd;
        double ee;
        double ff;
    } periodic_terms[]{
        {0, 0, 0, 0, 1, -172064161_arcsec, -174666_arcsec, 33386_arcsec,
         92052331_arcsec, 9086_arcsec, 15377_arcsec},
        {0, 0, 2, -2, 2, -13170906_arcsec, -1675_arcsec, 13696_arcsec,
         5730336_arcsec, -3015_arcsec, -4587_arcsec},
        {0, 0, 2, 0, 2, -2276413_arcsec, -234_arcsec, 2796_arcsec,
         978459_arcsec, -485_arcsec, 1374_arcsec},
        {0, 0, 0, 0, 2, 2074554_arcsec, 207_arcsec, -698_arcsec, -897492_arcsec,
         470_arcsec, -291_arcsec},
        {0, 1, 0, 0, 0, 1475877_arcsec, -3633_arcsec, 11817_arcsec,
         73871_arcsec, -184_arcsec, -1924_arcsec},
        {0, 1, 2, -2, 2, -516821_arcsec, 1226_arcsec, -524_arcsec,
         224386_arcsec, -677_arcsec, -174_arcsec},
        {1, 0, 0, 0, 0, 711159_arcsec, 73_arcsec, -872_arcsec, -6750_arcsec,
         0_arcsec, 358_arcsec},
        {0, 0, 2, 0, 1, -387298_arcsec, -367_arcsec, 380_arcsec, 200728_arcsec,
         18_arcsec, 318_arcsec},
        {1, 0, 2, 0, 2, -301461_arcsec, -36_arcsec, 816_arcsec, 129025_arcsec,
         -63_arcsec, 367_arcsec},
        {0, -1, 2, -2, 2, 215829_arcsec, -494_arcsec, 111_arcsec, -95929_arcsec,
         299_arcsec, 132_arcsec},
        {0, 0, 2, -2, 1, 128227_arcsec, 137_arcsec, 181_arcsec, -68982_arcsec,
         -9_arcsec, 39_arcsec},
        {-1, 0, 2, 0, 2, 123457_arcsec, 11_arcsec, 19_arcsec, -53311_arcsec,
         32_arcsec, -4_arcsec},
        {-1, 0, 0, 2, 0, 156994_arcsec, 10_arcsec, -168_arcsec, -1235_arcsec,
         0_arcsec, 82_arcsec},
        {1, 0, 0, 0, 1, 63110_arcsec, 63_arcsec, 27_arcsec, -33228_arcsec,
         0_arcsec, -9_arcsec},
        {-1, 0, 0, 0, 1, -57976_arcsec, -63_arcsec, -189_arcsec, 31429_arcsec,
         0_arcsec, -75_arcsec},
        {-1, 0, 2, 2, 2, -59641_arcsec, -11_arcsec, 149_arcsec, 25543_arcsec,
         -11_arcsec, 66_arcsec},
        {1, 0, 2, 0, 1, -51613_arcsec, -42_arcsec, 129_arcsec, 26366_arcsec,
         0_arcsec, 78_arcsec},
        {-2, 0, 2, 0, 1, 45893_arcsec, 50_arcsec, 31_arcsec, -24236_arcsec,
         -10_arcsec, 20_arcsec},
        {0, 0, 0, 2, 0, 63384_arcsec, 11_arcsec, -150_arcsec, -1220_arcsec,
         0_arcsec, 29_arcsec},
        {0, 0, 2, 2, 2, -38571_arcsec, -1_arcsec, 158_arcsec, 16452_arcsec,
         -11_arcsec, 68_arcsec},
        {0, -2, 2, -2, 2, 32481_arcsec, 0_arcsec, 0_arcsec, -13870_arcsec,
         0_arcsec, 0_arcsec},
        {-2, 0, 0, 2, 0, -47722_arcsec, 0_arcsec, -18_arcsec, 477_arcsec,
         0_arcsec, -25_arcsec},
        {2, 0, 2, 0, 2, -31046_arcsec, -1_arcsec, 131_arcsec, 13238_arcsec,
         -11_arcsec, 59_arcsec},
        {1, 0, 2, -2, 2, 28593_arcsec, 0_arcsec, -1_arcsec, -12338_arcsec,
         10_arcsec, -3_arcsec},
        {-1, 0, 2, 0, 1, 20441_arcsec, 21_arcsec, 10_arcsec, -10758_arcsec,
         0_arcsec, -3_arcsec},
        {2, 0, 0, 0, 0, 29243_arcsec, 0_arcsec, -74_arcsec, -609_arcsec,
         0_arcsec, 13_arcsec},
        {0, 0, 2, 0, 0, 25887_arcsec, 0_arcsec, -66_arcsec, -550_arcsec,
         0_arcsec, 11_arcsec},
        {0, 1, 0, 0, 1, -14053_arcsec, -25_arcsec, 79_arcsec, 8551_arcsec,
         -2_arcsec, -45_arcsec},
        {-1, 0, 0, 2, 1, 15164_arcsec, 10_arcsec, 11_arcsec, -8001_arcsec,
         0_arcsec, -1_arcsec},
        {0, 2, 2, -2, 2, -15794_arcsec, 72_arcsec, -16_arcsec, 6850_arcsec,
         -42_arcsec, -5_arcsec},
        {0, 0, -2, 2, 0, 21783_arcsec, 0_arcsec, 13_arcsec, -167_arcsec,
         0_arcsec, 13_arcsec},
        {1, 0, 0, -2, 1, -12873_arcsec, -10_arcsec, -37_arcsec, 6953_arcsec,
         0_arcsec, -14_arcsec},
        {0, -1, 0, 0, 1, -12654_arcsec, 11_arcsec, 63_arcsec, 6415_arcsec,
         0_arcsec, 26_arcsec},
        {-1, 0, 2, 2, 1, -10204_arcsec, 0_arcsec, 25_arcsec, 5222_arcsec,
         0_arcsec, 15_arcsec},
        {0, 2, 0, 0, 0, 16707_arcsec, -85_arcsec, -10_arcsec, 168_arcsec,
         -1_arcsec, 10_arcsec},
        {1, 0, 2, 2, 2, -7691_arcsec, 0_arcsec, 44_arcsec, 3268_arcsec,
         0_arcsec, 19_arcsec},
        {-2, 0, 2, 0, 0, -11024_arcsec, 0_arcsec, -14_arcsec, 104_arcsec,
         0_arcsec, 2_arcsec},
        {0, 1, 2, 0, 2, 7566_arcsec, -21_arcsec, -11_arcsec, -3250_arcsec,
         0_arcsec, -5_arcsec},
        {0, 0, 2, 2, 1, -6637_arcsec, -11_arcsec, 25_arcsec, 3353_arcsec,
         0_arcsec, 14_arcsec},
        {0, -1, 2, 0, 2, -7141_arcsec, 21_arcsec, 8_arcsec, 3070_arcsec,
         0_arcsec, 4_arcsec},
        {0, 0, 0, 2, 1, -6302_arcsec, -11_arcsec, 2_arcsec, 3272_arcsec,
         0_arcsec, 4_arcsec},
        {1, 0, 2, -2, 1, 5800_arcsec, 10_arcsec, 2_arcsec, -3045_arcsec,
         0_arcsec, -1_arcsec},
        {2, 0, 2, -2, 2, 6443_arcsec, 0_arcsec, -7_arcsec, -2768_arcsec,
         0_arcsec, -4_arcsec},
        {-2, 0, 0, 2, 1, -5774_arcsec, -11_arcsec, -15_arcsec, 3041_arcsec,
         0_arcsec, -5_arcsec},
        {2, 0, 2, 0, 1, -5350_arcsec, 0_arcsec, 21_arcsec, 2695_arcsec,
         0_arcsec, 12_arcsec},
        {0, -1, 2, -2, 1, -4752_arcsec, -11_arcsec, -3_arcsec, 2719_arcsec,
         0_arcsec, -3_arcsec},
        {0, 0, 0, -2, 1, -4940_arcsec, -11_arcsec, -21_arcsec, 2720_arcsec,
         0_arcsec, -9_arcsec},
        {-1, -1, 0, 2, 0, 7350_arcsec, 0_arcsec, -8_arcsec, -51_arcsec,
         0_arcsec, 4_arcsec},
        {2, 0, 0, -2, 1, 4065_arcsec, 0_arcsec, 6_arcsec, -2206_arcsec,
         0_arcsec, 1_arcsec},
        {1, 0, 0, 2, 0, 6579_arcsec, 0_arcsec, -24_arcsec, -199_arcsec,
         0_arcsec, 2_arcsec},
        {0, 1, 2, -2, 1, 3579_arcsec, 0_arcsec, 5_arcsec, -1900_arcsec,
         0_arcsec, 1_arcsec},
        {1, -1, 0, 0, 0, 4725_arcsec, 0_arcsec, -6_arcsec, -41_arcsec, 0_arcsec,
         3_arcsec},
        {-2, 0, 2, 0, 2, -3075_arcsec, 0_arcsec, -2_arcsec, 1313_arcsec,
         0_arcsec, -1_arcsec},
        {3, 0, 2, 0, 2, -2904_arcsec, 0_arcsec, 15_arcsec, 1233_arcsec,
         0_arcsec, 7_arcsec},
        {0, -1, 0, 2, 0, 4348_arcsec, 0_arcsec, -10_arcsec, -81_arcsec,
         0_arcsec, 2_arcsec},
        {1, -1, 2, 0, 2, -2878_arcsec, 0_arcsec, 8_arcsec, 1232_arcsec,
         0_arcsec, 4_arcsec},
        {0, 0, 0, 1, 0, -4230_arcsec, 0_arcsec, 5_arcsec, -20_arcsec, 0_arcsec,
         -2_arcsec},
        {-1, -1, 2, 2, 2, -2819_arcsec, 0_arcsec, 7_arcsec, 1207_arcsec,
         0_arcsec, 3_arcsec},
        {-1, 0, 2, 0, 0, -4056_arcsec, 0_arcsec, 5_arcsec, 40_arcsec, 0_arcsec,
         -2_arcsec},
        {0, -1, 2, 2, 2, -2647_arcsec, 0_arcsec, 11_arcsec, 1129_arcsec,
         0_arcsec, 5_arcsec},
        {-2, 0, 0, 0, 1, -2294_arcsec, 0_arcsec, -10_arcsec, 1266_arcsec,
         0_arcsec, -4_arcsec},
        {1, 1, 2, 0, 2, 2481_arcsec, 0_arcsec, -7_arcsec, -1062_arcsec,
         0_arcsec, -3_arcsec},
        {2, 0, 0, 0, 1, 2179_arcsec, 0_arcsec, -2_arcsec, -1129_arcsec,
         0_arcsec, -2_arcsec},
        {-1, 1, 0, 1, 0, 3276_arcsec, 0_arcsec, 1_arcsec, -9_arcsec, 0_arcsec,
         0_arcsec},
        {1, 1, 0, 0, 0, -3389_arcsec, 0_arcsec, 5_arcsec, 35_arcsec, 0_arcsec,
         -2_arcsec},
        {1, 0, 2, 0, 0, 3339_arcsec, 0_arcsec, -13_arcsec, -107_arcsec,
         0_arcsec, 1_arcsec},
        {-1, 0, 2, -2, 1, -1987_arcsec, 0_arcsec, -6_arcsec, 1073_arcsec,
         0_arcsec, -2_arcsec},
        {1, 0, 0, 0, 2, -1981_arcsec, 0_arcsec, 0_arcsec, 854_arcsec, 0_arcsec,
         0_arcsec},
        {-1, 0, 0, 1, 0, 4026_arcsec, 0_arcsec, -353_arcsec, -553_arcsec,
         0_arcsec, -139_arcsec},
        {0, 0, 2, 1, 2, 1660_arcsec, 0_arcsec, -5_arcsec, -710_arcsec, 0_arcsec,
         -2_arcsec},
        {-1, 0, 2, 4, 2, -1521_arcsec, 0_arcsec, 9_arcsec, 647_arcsec, 0_arcsec,
         4_arcsec},
        {-1, 1, 0, 1, 1, 1314_arcsec, 0_arcsec, 0_arcsec, -700_arcsec, 0_arcsec,
         0_arcsec},
        {0, -2, 2, -2, 1, -1283_arcsec, 0_arcsec, 0_arcsec, 672_arcsec,
         0_arcsec, 0_arcsec},
        {1, 0, 2, 2, 1, -1331_arcsec, 0_arcsec, 8_arcsec, 663_arcsec, 0_arcsec,
         4_arcsec},
        {-2, 0, 2, 2, 2, 1383_arcsec, 0_arcsec, -2_arcsec, -594_arcsec,
         0_arcsec, -2_arcsec},
        {-1, 0, 0, 0, 2, 1405_arcsec, 0_arcsec, 4_arcsec, -610_arcsec, 0_arcsec,
         2_arcsec},
        {1, 1, 2, -2, 2, 1290_arcsec, 0_arcsec, 0_arcsec, -556_arcsec, 0_arcsec,
         0_arcsec},
    };

    double t{(julian_date_ - PA::EpochJ2000) / 36525.0};
    double l{horner_polynomial(
        {+485868.249036_arcsec, +1717915923.2178_arcsec, +31.8792_arcsec,
         +0.051635_arcsec, -0.00024470_arcsec},
        t)};
    double lp{horner_polynomial(
        {+1287104.79305_arcsec, +129596581.0481_arcsec, -0.5532_arcsec,
         +0.000136_arcsec, -0.00001149_arcsec},
        t)};
    double f{horner_polynomial(
        {+335779.526232_arcsec, +1739527262.8478_arcsec, -12.7512_arcsec,
         -0.001037_arcsec, +0.00000417_arcsec},
        t)};
    double d{horner_polynomial(
        {+1072260.70369_arcsec, +1602961601.2090_arcsec, -6.3706_arcsec,
         +0.006593_arcsec, -0.00003169_arcsec},
        t)};
    double om{horner_polynomial(
        {450160.398036_arcsec, -6962890.5431_arcsec, +7.4722_arcsec,
         +0.007702_arcsec, -0.00005939_arcsec},
        t)};

    double sum_dpsi{0.0};
    double sum_deps{0.0};
    for(auto& pt : periodic_terms) {
        double arg{l * pt.m1 + lp * pt.m2 + f * pt.m3 + d * pt.m4 + om * pt.m5};
        double dpsi{(pt.aa + pt.bb * t) * std::sin(arg) +
                    pt.cc * std::cos(arg)};
        double deps{(pt.dd + pt.ee * t) * std::cos(arg) +
                    pt.ff * std::sin(arg)};
        sum_dpsi += dpsi;
        sum_deps += deps;
    }
    nutation_longitude_ = sum_dpsi / 10000000.0;
    nutation_obliquity_ = sum_deps / 10000000.0;
    nutation_is_valid_ = true;
}

}  // namespace PA

#endif  // EARTH_NUTATION_H_
