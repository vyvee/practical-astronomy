#ifndef SOLVER_H_
#define SOLVER_H_

#include "angle.h"

namespace PA
{
class Solver
{
   public:
    static Radian solve_kepler(double ecc, const Radian& m);

   private:
    Solver(){};
};

}  // namespace PA

#endif  // SOLVER_H_
