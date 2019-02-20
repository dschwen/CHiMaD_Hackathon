#include "MMSForceMu.h"

registerMooseObject("ChimadHackathonApp", MMSForceMu);

template <>
InputParameters
validParams<MMSForceMu>()
{
  InputParameters params = MMSBase<Kernel>::validParams();
  return params;
}

MMSForceMu::MMSForceMu(const InputParameters & parameters) : MMSBase<Kernel>(parameters) {}

Real
MMSForceMu::computeQpResidual()
{
  const Point & p = _q_point[_qp];

  const Real c = computeC(p);
  const RealGradient grad_c = computeGradC(p);
  const Real laplacian_c = computeLaplacianC(p);
  const Real laplacian2c = computeLaplacian2C(p);
  const Real d2fdc2 = computeD2fDc2(p);
  const Real d3fdc3 = computeD3fDc3(p);

  return -_M[_qp] *
         (-_kappa[_qp] * laplacian2c + d2fdc2 * laplacian_c + d3fdc3 * (grad_c * grad_c)) *
         _test[_i][_qp];
}
