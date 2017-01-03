#include "OstRipACKernel.h"

template<>
InputParameters validParams<OstRipACKernel>()
{
  InputParameters params = ACBulk<Real>::validParams();
  params.addRequiredCoupledVar("v", "Array of order parameter names except the current one");
  params.addRequiredCoupledVar("c", "phase field variable, particle density");
  params.addParam<MaterialPropertyName>("gamma", 1.0, "Material Proprty/co-effieient for free energy term");
  params.addParam<Real>("beta", 1.0, "Co-efficient for free energy term");
  params.addParam<Real>("conc_alpha", 0.05, "Equilibrium concentration for phase alpha");
  params.addParam<Real>("epsilon", 3.0, "Co-effient for free energy term");
  return params;
}

OstRipACKernel::OstRipACKernel(const InputParameters & parameters) :
    ACBulk<Real>(parameters),
    _c(coupledValue("c")),
    _c_var(coupled("c")),
    _gamma(getMaterialProperty<Real>("gamma")),
    _beta(getParam<Real>("beta")),
    _conc_alpha(getParam<Real>("conc_alpha")),
    _epsilon(getParam<Real>("conc_alpha")),
    _ncrys(coupledComponents("v")) //determine number of grains from the number of names passed in.  Note this is the actual number -1
{
  // Array of coupled variables is created in the constructor
  _vals.resize(_ncrys); //Size variable arrays
  _vals_var.resize(_ncrys);

  //Loop through grains and load coupled variables into the arrays
  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _vals_var[i] = coupled("v", i);
  }
}

Real
OstRipACKernel::computeDFDOP(PFFunctionType type)
{
  Real c = _c[_qp];

  Real SumEtaj = 0.0;
  for (unsigned int i = 0; i < _ncrys; ++i)
    SumEtaj += (*_vals[i])[_qp]*(*_vals[i])[_qp]; //Sum all other order parameters

  //Calcualte either the residual or jacobian of the grain growth free energy
  switch (type)
  {
    case Residual:
      return - _gamma[_qp] * (_c[_qp] - _conc_alpha) * (_c[_qp] - _conc_alpha) * _u[_qp] + _beta * _u[_qp] * _u[_qp] * _u[_qp] + _epsilon * _u[_qp] * SumEtaj;

    case Jacobian:
      return (- _gamma[_qp] * (_c[_qp] - _conc_alpha) * (_c[_qp] - _conc_alpha) + 3.0 * _beta * _u[_qp] * _u[_qp] + _epsilon * SumEtaj) * _phi[_j][_qp];
  }

  mooseError("Invalid type passed in");
}

Real
OstRipACKernel::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _c_var)
    {
     Real dDFDc = - 2.0 * _gamma[_qp] * (_c[_qp] - _conc_alpha) * _u[_qp] * _phi[_j][_qp];
     return _L[_qp] * _test[_i][_qp] * dDFDc;
    }
  for (unsigned int i = 0; i < _ncrys; ++i)
    if (jvar == _vals_var[i])
    {
      Real dSumEtaj = 2.0 * (*_vals[i])[_qp] * _phi[_j][_qp]; //Derivative of SumEtaj
      Real dDFDOP = _epsilon * _u[_qp] * dSumEtaj;
      return _L[_qp] * _test[_i][_qp] * dDFDOP;
    }

  return 0.0;
}
