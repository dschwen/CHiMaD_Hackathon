#ifndef OSTRIPPSEUDORAND_H
#define OSTRIPPSEUDORAND_H

#include "InitialCondition.h"

class OstRipPseudoRand;

template<>
InputParameters validParams<OstRipPseudoRand>();

/**
 * Pseudo random initial condition for the Ostwald Ripening problem.
 * Equation (14) from http://dx.doi.org/10.1016/j.commatsci.2016.09.022
 */
class OstRipPseudoRand : public InitialCondition
{
public:
  OstRipPseudoRand(const InputParameters & parameters);

  virtual Real value(const Point & p) override;

protected:
  const Real _i;
};

#endif // OSTRIPPSEUDORAND_H
