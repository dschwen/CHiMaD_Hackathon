#include "MMSForceC.h"

registerMooseObject("ChimadHackathonApp", MMSForceC);

template <>
InputParameters
validParams<MMSForceC>()
{
  InputParameters params = MMSBase<Kernel>::validParams();
  return params;
}

MMSForceC::MMSForceC(const InputParameters & parameters) : MMSBase<Kernel>(parameters) {}

Real
MMSForceC::computeQpResidual()
{
  const Point & p = _q_point[_qp];

  const Real laplacian_c = computeLaplacianC(p);
  const Real dfdc = computeDfDc(p);

  return -(-_kappa[_qp] * laplacian_c + dfdc) * _test[_i][_qp];
}
