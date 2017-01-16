#include "OstRipPseudoRand.h"
#include "libmesh/utility.h"

template<>
InputParameters validParams<OstRipPseudoRand>()
{
  InputParameters params = validParams<InitialCondition>();
  params.addRequiredParam<unsigned int>("i", "Order parameter ID");
  return params;
}

OstRipPseudoRand::OstRipPseudoRand(const InputParameters & parameters) :
    InitialCondition(parameters),
    _i(getParam<unsigned int>("i"))
{
}

Real
OstRipPseudoRand::value(const Point & p)
{
  const Real x = p(0);
  const Real y = p(1);

  const Real en = 0.1;
  const Real psi = 1.5;

  return en * Utility::pow<2>(std::cos((0.01 * _i) * x - 4.0) * std::cos((0.007 + 0.01 * _i) * y)
         + std::cos((0.11 + 0.01 * _i) * x) * std::cos((0.11 + 0.01 * _i) * y)
         + psi * Utility::pow<2>(std::cos((0.046 + 0.001 * _i) * x + (0.0405 + 0.001 * _i) * y) * std::cos((0.031
           + 0.001 * _i) * x - (0.004 + 0.001 * _i) * y)));
}
